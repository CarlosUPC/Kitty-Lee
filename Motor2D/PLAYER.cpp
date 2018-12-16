#include "j1App.h"
#include "PLAYER.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2List.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "j1Gui.h"
#include "Label.h"
#include "Image.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"

Player::Player(int x, int y) : j1Entity(Types::PLAYER, x, y) {

	LoadEntityData("player.tsx");

}

Player::~Player()
{
}

bool Player::Start()
{

	state = IDLE;

	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;

	speed.SetToZero();

	AddColliders();

	App->audio->LoadFx(walkingSound);
	App->audio->LoadFx(jumpingSound);
	App->audio->LoadFx(crashingSound);


	//UI
	App->gui->CreateScreen();

	coin_label = App->gui->CreateLabel(100, 100, "coins   ", false, false, App->gui->screen, WHITE);
	coin_count = 0;

	life = 3;
	int margin = 30;
	life1 = App->gui->CreateImage(margin, margin, { 30,1040,42,47 }, App->gui->screen);
	life2 = App->gui->CreateImage(life1->position.x + life1->position.w + margin, margin, { 30,1040,42,47 }, App->gui->screen);
	life3 = App->gui->CreateImage(life2->position.x + life2->position.w + margin, margin, { 30,1040,42,47 }, App->gui->screen);

	return true;
}

bool Player::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		App->collider->GhostMode(ghost);
		ghost = !ghost;
		ChangeState();
	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		App->collider->GodMode(god_mode);
		god_mode = !god_mode;
	}

	//Player collider update
	SetCollidersPos();

	CheckState();
	current_animation->GetCurrentFrame(dt);

	return true;
}

// Called before quitting
bool Player::CleanUp()
{
	bool ret = false;
	ret = App->tex->UnLoad(data.tileset.texture);
	collider.collider->to_delete = true;
	colliderPlayer_down.collider->to_delete = true;
	colliderPlayer_up.collider->to_delete = true;
	colliderPlayer_right.collider->to_delete = true;
	colliderPlayer_left.collider->to_delete = true;

	App->gui->DeleteAllUIElements();

	return ret;
}

void Player::Move(float dt) {
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && speed.x < maxSpeedX) {
			speed.x += incrementSpeedX;
			if (!air)
				App->audio->PlayFx(1); //Walk fx
			
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && speed.x > -maxSpeedX) {
			speed.x -= incrementSpeedX;
			if (!air)
				App->audio->PlayFx(1); //Walk fx
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
		speed.x = 0.0f; 
		App->audio->StopFx(1); //Walk fx
	}

	if (god_mode) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			speed.y -= incrementSpeedX;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			speed.y += incrementSpeedX;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_UP) {
			speed.y = 0.0f;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT && App->collider->Check(colliderPlayer_down.collider,COLLIDER_PLATFORM)) {
		if (!platformOverstep)
			platformOverstep = true;
		App->audio->StopFx(1); //Walk fx
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && air == false) {
		speed.y = jumpSpeed;
		air = true;
		App->audio->PlayFx(2); //Jump fx
	}

	if (!god_mode) {
		speed.y += App->map->data.properties.gravity * dt;
	}
	if (speed.y > App->map->data.properties.maxAccelerationY)
		speed.y = App->map->data.properties.maxAccelerationY;
	else if (speed.y < -App->map->data.properties.maxAccelerationY)
		speed.y = -App->map->data.properties.maxAccelerationY;

	position.x += ceil(speed.x * dt);
	position.y += ceil(speed.y * dt);
	SetCollidersPos();
}

void Player::SetCoins(const int &coins)
{
	coin_count = coins;
}

void Player::SetLife(const int &l)
{
	life = l;
}

void Player::UpdateUI()
{
	coin_label->SetText(p2SString("coins %i", coin_count).GetString());
	//life_label;
}

bool Player::Save(pugi::xml_node &node) const
{
	pugi::xml_node e_node = node.append_child("entity");

	e_node.append_attribute("x") = (int)position.x;
	e_node.append_attribute("y") = (int)position.y;
	e_node.append_attribute("spawn_x") = (int)spawn_position.x;
	e_node.append_attribute("spawn_y") = (int)spawn_position.y;
	e_node.append_attribute("type") = (int)type;
	e_node.append_attribute("coins") = coin_count;
	e_node.append_attribute("lifes") = life;

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2, float dt) {
	switch (c2->type) {
	case COLLIDER_FLOOR:
		if (c1 == colliderPlayer_down.collider) {
			speed.y = 0.0f;
			if (!god_mode)
				speed.y -= App->map->data.properties.gravity * dt;
			if (air)
				air = false;
			if (c1->rect.y >= c2->rect.y)
				position.y = c2->rect.y - collider.height - collider.offset.y;
		}

		else if (c1 == colliderPlayer_up.collider) {
			speed.y = 0.0f;
			if (c2->rect.y + c2->rect.h >= c1->rect.y)
				position.y = c2->rect.y + c2->rect.h - collider.offset.y + c1->rect.h;
		}
		if (c1 == colliderPlayer_left.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x + c2->rect.w >= c1->rect.x)
				position.x = c2->rect.x + c2->rect.w - collider.offset.x + 1;
		}

		else if (c1 == colliderPlayer_right.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x <= c1->rect.x)
				position.x = c2->rect.x - collider.width - collider.offset.x + 1;

		}
		break;
	case COLLIDER_GHOST:
		if (c1 == colliderPlayer_left.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x + c2->rect.w >= c1->rect.x)
				position.x = c2->rect.x + c2->rect.w - collider.offset.x + 1;
		}

		else if (c1 == colliderPlayer_right.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x <= c1->rect.x)
				position.x = c2->rect.x - collider.width - collider.offset.x + 1;

		}
		break;
	case COLLIDER_PLATFORM:
		if (c1 == colliderPlayer_down.collider) {
			if (speed.y >= 0 && c2->rect.y + c2->rect.h * 0.5f >= c1->rect.y && !platformOverstep && !ghost) {
				speed.y = 0.0f;
				if (!god_mode)
					speed.y -= App->map->data.properties.gravity * dt;
				if (air)
					air = false;
				if (c1->rect.y >= c2->rect.y)
					position.y = c2->rect.y - collider.height - collider.offset.y;
			}
			else if (c2->rect.y + c2->rect.h * 0.5f < c1->rect.y && platformOverstep) {
				platformOverstep = false;
			}
		}
		break;
	case COLLIDER_DEATH:
		App->LoadGame();
		SubstractLife();
		break;
	case COLLIDER_ENEMY:
		if (!death) {
			death = true;
			state = DEAD;
			ChangeState();
			SubstractLife();
		}
		break;
	case COLLIDER_SCENE:
		if (!App->fade->IsFading())
			App->fade->FadeToBlack();
		break;
	case COLLIDER_WIN:
		if (!App->fade->IsFading())
			App->fade->FadeToBlack();
		break;
	case COLLIDER_COIN:
		coin_count++;
		App->entities->DestroyEntity(c2->callback);
		UpdateUI();
		break;
	default:
		break;
	}
	SetCollidersPos();
}

void Player::PushBack() {

	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING:
				anim_walking.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::JUMP:
				anim_jump.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::FALL:
				anim_fall.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::LAND:
				anim_land.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::IDLE_GHOST:
				anim_idle_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING_GHOST:
				anim_walking_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::JUMP_GHOST:
				anim_jump_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::FALL_GHOST:
				anim_fall_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::LAND_GHOST:
				anim_land_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::DEAD:
				anim_death.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}

	anim_jump.loop = false;
	anim_land.loop = false;
	anim_death.loop = false;
}

void Player::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function
	
	actual_collider = &collider;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &colliderPlayer_down;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &colliderPlayer_up;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &colliderPlayer_left;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &colliderPlayer_right;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

}

void Player::SetCollidersPos() {
	COLLIDER_INFO* actual_collider;
	
	actual_collider = &collider;
	actual_collider->collider->SetPos((int)position.x + actual_collider->offset.x, (int)position.y + actual_collider->offset.y);

	actual_collider = &colliderPlayer_down;
	actual_collider->collider->SetPos((int)position.x + actual_collider->offset.x, (int)position.y + actual_collider->offset.y);

	actual_collider = &colliderPlayer_up;
	actual_collider->collider->SetPos((int)position.x + actual_collider->offset.x, (int)position.y + actual_collider->offset.y);

	actual_collider = &colliderPlayer_left;
	actual_collider->collider->SetPos((int)position.x + actual_collider->offset.x, (int)position.y + actual_collider->offset.y);

	actual_collider = &colliderPlayer_right;
	actual_collider->collider->SetPos((int)position.x + actual_collider->offset.x, (int)position.y + actual_collider->offset.y);

}

void Player::IdAnimToEnum()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		case 16:
			data.animations[i].animType = EntityState::WALKING;
			break;
		case 32:
			data.animations[i].animType = EntityState::JUMP;
			break;
		case 35:
			data.animations[i].animType = EntityState::FALL;
			break;
		case 36:
			data.animations[i].animType = EntityState::LAND;
			break;
		case 64:
			data.animations[i].animType = EntityState::DEAD;
			break;
		case 80:
			data.animations[i].animType = EntityState::HADOUKEN;
			break;
		case 96:
			data.animations[i].animType = EntityState::PUNCH;
			break;
		case 4:
			data.animations[i].animType = EntityState::IDLE_GHOST;
			break;
		case 24:
			data.animations[i].animType = EntityState::WALKING_GHOST;
			break;
		case 40:
			data.animations[i].animType = EntityState::JUMP_GHOST;
			break;
		case 43:
			data.animations[i].animType = EntityState::FALL_GHOST;
			break;
		case 44:
			data.animations[i].animType = EntityState::LAND_GHOST;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}

void Player::LoadProperties(pugi::xml_node &node)
{
	p2SString nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "animationSpeed")
			animationSpeed = node.attribute("value").as_float();

		else if (nameIdentificator == "incrementSpeedX")
			incrementSpeedX = node.attribute("value").as_float();

		else if (nameIdentificator == "jumpSpeed")
			jumpSpeed = node.attribute("value").as_float();

		else if (nameIdentificator == "maxSpeedX")
			maxSpeedX = node.attribute("value").as_float();

		//Load audio fx data
		else if (nameIdentificator == "walkingSound")
			walkingSound = node.attribute("value").as_string();

		else if (nameIdentificator == "jumpSound")
			jumpingSound = node.attribute("value").as_string();

		else if (nameIdentificator == "crashSound")
			crashingSound = node.attribute("value").as_string();

		node = node.next_sibling();
	}
}

void Player::LoadCollider(pugi::xml_node &node)
{
	p2SString nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "Collider") {
			collider.offset.x = node.attribute("x").as_int();
			collider.offset.y = node.attribute("y").as_int();
			collider.width = node.attribute("width").as_uint();
			collider.height = node.attribute("height").as_uint();
			collider.type = COLLIDER_TYPE::COLLIDER_PLAYER;
		}

		else if (nameIdentificator == "ColliderDown") {
			colliderPlayer_down.offset.x = node.attribute("x").as_int();
			colliderPlayer_down.offset.y = node.attribute("y").as_int();
			colliderPlayer_down.width = node.attribute("width").as_uint();
			colliderPlayer_down.height = node.attribute("height").as_uint();
			colliderPlayer_down.type = COLLIDER_TYPE::COLLIDER_ENTITY_DOWN;
		}
		else if (nameIdentificator == "ColliderLeft") {
			colliderPlayer_left.offset.x = node.attribute("x").as_int();
			colliderPlayer_left.offset.y = node.attribute("y").as_int();
			colliderPlayer_left.width = node.attribute("width").as_uint();
			colliderPlayer_left.height = node.attribute("height").as_uint();
			colliderPlayer_left.type = COLLIDER_TYPE::COLLIDER_PLAYER_LEFT;
		}
		else if (nameIdentificator == "ColliderRight") {
			colliderPlayer_right.offset.x = node.attribute("x").as_int();
			colliderPlayer_right.offset.y = node.attribute("y").as_int();
			colliderPlayer_right.width = node.attribute("width").as_uint();
			colliderPlayer_right.height = node.attribute("height").as_uint();
			colliderPlayer_right.type = COLLIDER_TYPE::COLLIDER_PLAYER_RIGHT;
		}
		else if (nameIdentificator == "ColliderUp") {
			colliderPlayer_up.offset.x = node.attribute("x").as_int();
			colliderPlayer_up.offset.y = node.attribute("y").as_int();
			colliderPlayer_up.width = node.attribute("width").as_uint();
			colliderPlayer_up.height = node.attribute("height").as_uint();
			colliderPlayer_up.type = COLLIDER_TYPE::COLLIDER_PLAYER_UP;
		}

		node = node.next_sibling();
	}
}

void Player::Draw(float dt)
{
	App->render->Blit(data.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(dt), 1.0F, flip);
}

void Player::CheckState() {
	
	PlayerState prevState = state;
	switch (state) {
	case IDLE:
		if (speed.x != 0.0f)
			state = WALKING;
		if (air)
			state = JUMP;
		if (speed.y != 0.0f) {
			state = FALL;
			air = true;
		}
		break;
	case WALKING:
		if (speed.x == 0.0f)
			state = IDLE;
		if (air)
			state = JUMP;
		if (speed.y != 0.0f) {
			state = FALL;
			air = true;
		}
		break;
	case JUMP:
		if (current_animation->Finished() && (current_animation == &anim_jump || current_animation == &anim_jump_ghost))
			state = FALL;
		else if (!air)
			state = LAND;
		break;
	case FALL:
		if (!air)
			state = LAND;
		break;
	case LAND:
		if (current_animation->Finished() && (current_animation == &anim_land || current_animation == &anim_land_ghost))
			state = IDLE;
		else if (air)
			state = JUMP;
		break;
	case DEAD:
		if (current_animation->Finished() && current_animation == &anim_death) {
			state = IDLE;
			App->scene->ReturnToSpawnPositionEntities();
			//App->LoadGame();
			/*App->entities->CleanUp();
			App->scene->CreateEntities();
			App->render->CameraInitPos();*/
		}
		speed.x = 0.0F;
		break;
	default:
		break;
	}

	if (prevState != state)//only will change animation when change state
		ChangeState();

	if (speed.x > 0 && flip != SDL_FLIP_NONE)
		flip = SDL_FLIP_NONE;
	if (speed.x < 0 && flip != SDL_FLIP_HORIZONTAL)
		flip = SDL_FLIP_HORIZONTAL;
}

void Player::SubstractLife()
{
	switch (life--)
	{
	case 3:
		life3->SetRect(dead);
		break;
	case 2:
		life2->SetRect(dead);
		break;
	case 1:
		life1->SetRect(dead);
		break;
	default:
		break;
	}

}

void Player::ChangeState() {

	switch (state)
	{
	case IDLE:
		if (!ghost)
			current_animation = &anim_idle;
		else  current_animation = &anim_idle_ghost;
		break;
	case WALKING:
		if (!ghost)
			current_animation = &anim_walking;
		else current_animation = &anim_walking_ghost;
		current_animation->reset();
		break;
	case JUMP:
		if (!ghost)
			current_animation = &anim_jump;
		else current_animation = &anim_jump_ghost;
		current_animation->reset();
		break;
	case FALL:
		if (!ghost)
			current_animation = &anim_fall;
		else current_animation = &anim_fall_ghost;
		break;
	case LAND:
		if (!ghost)
			current_animation = &anim_land;
		else current_animation = &anim_land_ghost;
		current_animation->reset();
		break;
	case DEAD:
		current_animation = &anim_death;
		current_animation->reset();
		
		break;
	default:
		break;
	}

	current_animation->speed = animationSpeed;

}