#include "j1App.h"
#include "j1Player.h"
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
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"

j1Player::j1Player() : j1Entity(Types::PLAYER) {

	LoadEntityData("player.tsx");

}



j1Player::~j1Player()
{
}

bool j1Player::Start()
{

	state = IDLE;

	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;

	speed.SetToZero();

	AddColliders();

	acceleration.y = 1;

	App->audio->LoadFx(walkingSound);
	App->audio->LoadFx(jumpingSound);
	App->audio->LoadFx(crashingSound);

	return true;
}

bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);

	Movement(dt);

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		App->collider->GhostMode(ghost);
		ghost = !ghost;
		ChangeState();
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		death = !death;
		state = DEAD;
		ChangeState();
		
	}
	//Player collider update
	SetCollidersPos();

	CheckState();

	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	return App->tex->UnLoad(data.tileset.texture);
}

void j1Player::Movement(float dt) {
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && speed.x < maxSpeedX) {
			speed.x += incrementSpeedX;
			App->audio->PlayFx(1, -1); //Walk fx
			
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && speed.x > -maxSpeedX) {
			speed.x -= incrementSpeedX;
			App->audio->PlayFx(1, -1); //Walk fx
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
		speed.x = 0.0f; 
		App->audio->StopFx(1); //Walk fx
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT && App->collider->Check(colliderPlayer_down.collider,COLLIDER_PLATFORM)) {
		if (!platformOverstep)
			platformOverstep = true;
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && air == false) {
		speed.y = jumpSpeed;
		air = true;
		App->audio->PlayFx(2); //Jump fx
	}

	
	if (speed.y < App->map->data.properties.Get("maxAccelerationY"))
		speed.y += App->map->data.properties.Get("gravity");
	
	
	
	position += speed * dt;

}
void j1Player::PushBack() {

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

void j1Player::AddColliders() {
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

void j1Player::SetCollidersPos() {
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

void j1Player::IdAnimToEnum()
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

void j1Player::LoadProperties(pugi::xml_node &node)
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

void j1Player::LoadCollider(pugi::xml_node &node)
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
			colliderPlayer_down.type = COLLIDER_TYPE::COLLIDER_PLAYER_DOWN;
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

void j1Player::Draw(float dt)
{
	App->render->Blit(data.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(dt), 1.0F, flip);
}

// Load / Save
bool j1Player::Load(pugi::xml_node& data)
{
	

	if (App->fade->num_level == 1) {
		if(App->fade->level1==false)
			App->fade->SwitchingLevel(App->scene->lvl1.GetString());

		position.x = data.child("player").attribute("x").as_int();
		position.y = data.child("player").attribute("y").as_int();
	}

	if (App->fade->num_level == 2) {
		App->scene->stg = LEVEL_2;
		App->fade->SwitchingLevel(App->scene->lvl2.GetString());
		position.x = data.child("player").attribute("x").as_float();
		position.y = data.child("player").attribute("y").as_float();
	}

	speed.SetToZero();

	return true;

}
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node player_node = data.append_child("player");

	player_node.append_attribute("x") = position.x;
	player_node.append_attribute("y") = position.y;

	return true;
}

void j1Player::CheckState() {
	
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
			App->LoadGame();
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

void j1Player::ChangeState() {

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

void j1Player::OnCollision(Collider* c1, Collider* c2) {
	switch (c2->type) {
	case COLLIDER_FLOOR:
		if (c1 == colliderPlayer_down.collider) {
			speed.y = 0.0f;
			speed.y -= App->map->data.properties.Get("gravity");
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
		else if (c1 == colliderPlayer_left.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x + c2->rect.w >= c1->rect.x)
				position.x = c2->rect.x + c2->rect.w - collider.offset.x;
		}

		else if (c1 == colliderPlayer_right.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x <= c1->rect.x)
				position.x = c2->rect.x - collider.width - collider.offset.x;

		}
		break;
	case COLLIDER_GHOST:
		if (c1 == colliderPlayer_left.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x + c2->rect.w >= c1->rect.x)
				position.x = c2->rect.x + c2->rect.w - collider.offset.x;
		}

		else if (c1 == colliderPlayer_right.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x <= c1->rect.x)
				position.x = c2->rect.x - collider.width - collider.offset.x;

		}
		break;
	case COLLIDER_PLATFORM:
		if (c1 == colliderPlayer_down.collider) {
			if (speed.y >= 0 && c2->rect.y + c2->rect.h * 0.5f >= c1->rect.y && !platformOverstep && !ghost) {
				speed.y = 0.0f;
				speed.y -= App->map->data.properties.Get("gravity");
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
		break;
	case COLLIDER_SCENE:
		if (!App->fade->IsFading())
			App->fade->FadeToBlack();
		break;
	case COLLIDER_WIN:

		break;
	default:
		break;
	}
}