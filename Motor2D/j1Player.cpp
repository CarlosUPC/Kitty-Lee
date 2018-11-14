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

#include <cmath>


j1Player::j1Player() {
	name.create("player");
}



j1Player::~j1Player()
{
}

void j1Player::Init()
{
	active = true;
}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& node)
{
	bool ret = true;
	LOG("Loading Player Module");

	
	ret = LoadPlayer(node.child("file").text().as_string());

	state = IDLE;
	
	// Load animations
	PushBack();

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{

	data.tileset.texture = App->tex->Load(data.tileset.imagePath.GetString());

	//This method returns player object's position
	//position = App->map->GetInitialPosition();
	
	position = App->map->queue[PLAYER].initialPos;
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;
	AddColliders();
	//Speed of player
	speed = { 0,0 };

	App->audio->LoadFx(walkingSound);
	App->audio->LoadFx(jumpingSound);
	App->audio->LoadFx(crashingSound);

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	Movement(dt);

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		App->collider->GhostMode(ghost);
		ghost = !ghost;
		Actions();
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		death = !death;
		state = DEAD;
		Actions();
		
	}
	//Player collider update
	SetCollidersPos();

	CheckState();

	App->render->Blit(data.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(dt), 1.0F, flip);

	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{

	/*CheckState();

	App->render->Blit(data.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(dt), 1.0F, flip);*/
	
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{

	App->tex->UnLoad(data.tileset.texture);
	return true;
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



	position += speed * ceil(dt);
}
void j1Player::PushBack() {

	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].animType) {
			case IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			case WALKING:
				anim_walking.PushBack(data.animations[i].frames[j]);
				break;
			case JUMP:
				anim_jump.PushBack(data.animations[i].frames[j]);
				break;
			case FALL:
				anim_fall.PushBack(data.animations[i].frames[j]);
				break;
			case LAND:
				anim_land.PushBack(data.animations[i].frames[j]);
				break;
			case IDLE_GHOST:
				anim_idle_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case WALKING_GHOST:
				anim_walking_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case JUMP_GHOST:
				anim_jump_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case FALL_GHOST:
				anim_fall_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case LAND_GHOST:
				anim_land_ghost.PushBack(data.animations[i].frames[j]);
				break;
			case DEAD:
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

	//deleting player animation data already loaded in its corresponding animation variables
	for (uint i = 0; i < data.num_animations; ++i) {		//this block of code delete animation data loaded of xml,
		if (data.animations[i].frames != nullptr) {		//is in PushBack() because when load all animation in its
			delete[] data.animations[i].frames;			//corresponding variables, that data is useless
			data.animations[i].frames = nullptr;
		}
	}
	if (data.animations != nullptr) {
		delete[] data.animations;
		data.animations = nullptr;
	}
}

void j1Player::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function
	
	actual_collider = &colliderPlayer;
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

	actual_collider = &colliderPlayer;
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

	App->player->speed.SetToZero();

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
		Actions();

	if (speed.x > 0 && flip != SDL_FLIP_NONE)
		flip = SDL_FLIP_NONE;
	if (speed.x < 0 && flip != SDL_FLIP_HORIZONTAL)
		flip = SDL_FLIP_HORIZONTAL;
}

void j1Player::Actions() {

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
				position.y = c2->rect.y - colliderPlayer.height - colliderPlayer.offset.y;
		}

		else if (c1 == colliderPlayer_up.collider) {
			speed.y = 0.0f;
			if (c2->rect.y + c2->rect.h >= c1->rect.y)
				position.y = c2->rect.y + c2->rect.h - colliderPlayer.offset.y + c1->rect.h;
		}
		else if (c1 == colliderPlayer_left.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x + c2->rect.w >= c1->rect.x)
				position.x = c2->rect.x + c2->rect.w - colliderPlayer.offset.x;
		}

		else if (c1 == colliderPlayer_right.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x <= c1->rect.x)
				position.x = c2->rect.x - colliderPlayer.width - colliderPlayer.offset.x;

		}
		break;
	case COLLIDER_GHOST:
		if (c1 == colliderPlayer_left.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x + c2->rect.w >= c1->rect.x)
				position.x = c2->rect.x + c2->rect.w - colliderPlayer.offset.x;
		}

		else if (c1 == colliderPlayer_right.collider) {
			speed.x = 0.0f;
			App->audio->StopFx(1);
			if (c2->rect.x <= c1->rect.x)
				position.x = c2->rect.x - colliderPlayer.width - colliderPlayer.offset.x;

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
					position.y = c2->rect.y - colliderPlayer.height - colliderPlayer.offset.y;
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

//Load all initial information of player saved in a xml file
bool j1Player::LoadPlayer(const char* file) {
	bool ret = true;

	pugi::xml_parse_result result = player_file.load_file(file);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}

	//fill tileset info
	pugi::xml_node node = player_file.child("tileset");
	data.tileset.name.create(node.attribute("name").as_string());
	data.tileset.tilewidth = node.attribute("tilewidth").as_uint();
	data.tileset.tileheight = node.attribute("tileheight").as_uint();
	data.tileset.spacing = node.attribute("spacing").as_uint();
	data.tileset.margin = node.attribute("margin").as_uint();
	data.tileset.tilecount = node.attribute("tilecount").as_uint();
	data.tileset.columns = node.attribute("columns").as_uint();
	data.tileset.imagePath = node.child("image").attribute("source").as_string();
	data.tileset.width = node.child("image").attribute("width").as_uint();
	data.tileset.height = node.child("image").attribute("height").as_uint();

	//count how many animations are in file
	node = node.child("tile");
	data.num_animations = 0;
	while (node != NULL) {
		data.num_animations++;
		node = node.next_sibling("tile");
	}
	//reserve memory for all animations
	data.animations = new Anim[data.num_animations];

	//count how many frames for each animation, assign memory for those frames and set id frame start
	node = player_file.child("tileset").child("tile");
	for (uint i = 0; i < data.num_animations; ++i) {
		data.animations[i].FrameCount(node.child("animation").child("frame"));
		data.animations[i].frames = new SDL_Rect[data.animations[i].num_frames];
		data.animations[i].id = node.attribute("id").as_uint();
		node = node.next_sibling("tile");
	}

	//fill frame array with current information
	node = player_file.child("tileset").child("tile");
	pugi::xml_node node_frame;
	for (uint i = 0; i < data.num_animations; ++i) {
		node_frame = node.child("animation").child("frame");
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			data.animations[i].frames[j] = data.tileset.GetTileRect(node_frame.attribute("tileid").as_uint());
			node_frame = node_frame.next_sibling("frame");
		}
		node = node.next_sibling("tile");
	}
	//LOG all animation information
	for (uint i = 0; i < data.num_animations; ++i) {
		LOG("Animation %i--------", data.animations[i].id);
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			LOG("frame %i x: %i y: %i w: %i h: %i",
				j, data.animations[i].frames[j].x, data.animations[i].frames[j].y,
				data.animations[i].frames[j].w, data.animations[i].frames[j].h);
		}
	}
	
	//Load data
	node = player_file.child("tileset").child("properties").child("property");
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

	//Load all colliders saved in the first tile
	node = player_file.child("tileset").child("tile").child("objectgroup").child("object");
	while (node) {
		nameIdentificator = node.attribute("name").as_string();
		
		if (nameIdentificator == "Collider") {
			colliderPlayer.offset.x = node.attribute("x").as_int();
			colliderPlayer.offset.y = node.attribute("y").as_int();
			colliderPlayer.width = node.attribute("width").as_uint();
			colliderPlayer.height = node.attribute("height").as_uint();
			colliderPlayer.type = COLLIDER_TYPE::COLLIDER_PLAYER;
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

	
	//Convert id animations to enum
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = PlayerState::IDLE;
			break;
		case 16:
			data.animations[i].animType = PlayerState::WALKING;
			break;
		case 32:
			data.animations[i].animType = PlayerState::JUMP;
			break;
		case 35:
			data.animations[i].animType = PlayerState::FALL;
			break;
		case 36:
			data.animations[i].animType = PlayerState::LAND;
			break;
		case 64:
			data.animations[i].animType = PlayerState::DEAD;
			break;
		case 80:
			data.animations[i].animType = PlayerState::HADOUKEN;
			break;
		case 96:
			data.animations[i].animType = PlayerState::PUNCH;
			break;
		case 4:
			data.animations[i].animType = PlayerState::IDLE_GHOST;
			break;
		case 24:
			data.animations[i].animType = PlayerState::WALKING_GHOST;
			break;
		case 40:
			data.animations[i].animType = PlayerState::JUMP_GHOST;
			break;
		case 43:
			data.animations[i].animType = PlayerState::FALL_GHOST;
			break;
		case 44:
			data.animations[i].animType = PlayerState::LAND_GHOST;
			break;
		default:
			data.animations[i].animType = PlayerState::UNKNOWN;
			break;
		}
	}

	return ret;
}

//Functions to help loading data in xml-------------------------------------
//Get the rect info of an id of tileset
SDL_Rect TileSetPlayer::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}

//Return how many frames are in one animation
uint Anim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}

