#include "j1App.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "p2List.h"
#include "j1Input.h"
#include "j1Audio.h"


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

	player.tileset.texture = App->tex->Load(player.tileset.imagePath.GetString());

	//This method returns player object's position
	position = App->map->GetInitialPosition();
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;
	AddColliders();
	//Speed of player
	speed = { 0,0 };

	App->audio->LoadFx(walkingSound);
	App->audio->LoadFx(jumpingSound);

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
	Movement();

	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	
	CheckState();

	//Player collider update
	SetCollidersPos();

	App->render->Blit(player.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), 1.0F, flip);
	
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{

	App->tex->UnLoad(player.tileset.texture);
	return true;
}

void j1Player::Movement() {
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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && air == false) {
		speed.y = jumpSpeed;
		air = true;
		App->audio->PlayFx(2); //Jump fx
	}

	speed.y += App->map->data.gravity;



	position += speed;
}
void j1Player::PushBack() {

	for (uint i = 0; i < player.num_animations; ++i) {
		for (uint j = 0; j < player.animations[i].num_frames; ++j) {
			switch (player.animations[i].animType) {
			case IDLE:
				anim_idle.PushBack(player.animations[i].frames[j]);
				break;
			case WALKING:
				anim_walking.PushBack(player.animations[i].frames[j]);
				break;
			case JUMP:
				anim_jump.PushBack(player.animations[i].frames[j]);
				break;
			case FALL:
				anim_fall.PushBack(player.animations[i].frames[j]);
				break;
			case LAND:
				anim_land.PushBack(player.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}

	anim_jump.loop = false;
	anim_land.loop = false;

	//deleting player animation data already loaded in its corresponding animation variables
	for (uint i = 0; i < player.num_animations; ++i) {		//this block of code delete animation data loaded of xml,
		if (player.animations[i].frames != nullptr) {		//is in PushBack() because when load all animation in its
			delete[] player.animations[i].frames;			//corresponding variables, that data is useless
			player.animations[i].frames = nullptr;
		}
	}
	if (player.animations != nullptr) {
		delete[] player.animations;
		player.animations = nullptr;
	}
}

void j1Player::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function
	
	actual_collider = &playerColliders.colliderPlayer;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &playerColliders.colliderPlayer_ground;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &playerColliders.colliderPlayer_up;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &playerColliders.colliderPlayer_left;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

	actual_collider = &playerColliders.colliderPlayer_right;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y,	actual_collider->width,	actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

}

void j1Player::SetCollidersPos() {
	COLLIDER_INFO* actual_collider;

	actual_collider = &playerColliders.colliderPlayer;
	actual_collider->collider->SetPos(position.x + actual_collider->offset.x, position.y + actual_collider->offset.y);

	actual_collider = &playerColliders.colliderPlayer_ground;
	actual_collider->collider->SetPos(position.x + actual_collider->offset.x, position.y + actual_collider->offset.y);

	actual_collider = &playerColliders.colliderPlayer_up;
	actual_collider->collider->SetPos(position.x + actual_collider->offset.x, position.y + actual_collider->offset.y);

	actual_collider = &playerColliders.colliderPlayer_left;
	actual_collider->collider->SetPos(position.x + actual_collider->offset.x, position.y + actual_collider->offset.y);

	actual_collider = &playerColliders.colliderPlayer_right;
	actual_collider->collider->SetPos(position.x + actual_collider->offset.x, position.y + actual_collider->offset.y);

}

// Load / Save
bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

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
		break;
	case WALKING:
		if (speed.x == 0.0f)
			state = IDLE;
		if (air)
			state = JUMP;
		break;
	case JUMP:
		if (current_animation->Finished() && current_animation == &anim_jump)
			state = FALL;
		else if (!air)
			state = LAND;
		break;
	case FALL:
		if (!air)
			state = LAND;
		break;
	case LAND:
		if (current_animation->Finished() && current_animation == &anim_land)
			state = IDLE;
		else if (air)
			state = JUMP;
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
		current_animation = &anim_idle;
		break;
	case WALKING:
		current_animation = &anim_walking;
		current_animation->reset();
		break;
	case JUMP:
		current_animation = &anim_jump;
		current_animation->reset();
		break;
	case FALL:
		current_animation = &anim_fall;
		break;
	case LAND:
		current_animation = &anim_land;
		current_animation->reset();
		break;
	default:
		break;
	}

	current_animation->speed = animationSpeed;

}

void j1Player::OnCollision(Collider* c1, Collider* c2) {
	if (c1 == playerColliders.colliderPlayer_ground.collider && c2->type == COLLIDER_FLOOR) {
		speed.y = 0.0f;
		speed.y -= App->map->data.gravity;
		if (air)
			air = false;
		if (c1->rect.y >= c2->rect.y)
			position.y = c2->rect.y - playerColliders.colliderPlayer.height - c1->rect.h;
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
	player.tileset.name.create(node.attribute("name").as_string());
	player.tileset.tilewidth = node.attribute("tilewidth").as_uint();
	player.tileset.tileheight = node.attribute("tileheight").as_uint();
	player.tileset.spacing = node.attribute("spacing").as_uint();
	player.tileset.margin = node.attribute("margin").as_uint();
	player.tileset.tilecount = node.attribute("tilecount").as_uint();
	player.tileset.columns = node.attribute("columns").as_uint();
	player.tileset.imagePath = node.child("image").attribute("source").as_string();
	player.tileset.width = node.child("image").attribute("width").as_uint();
	player.tileset.height = node.child("image").attribute("height").as_uint();

	//count how many animations are in file
	node = node.child("tile");
	player.num_animations = 0;
	while (node != NULL) {
		player.num_animations++;
		node = node.next_sibling("tile");
	}
	//reserve memory for all animations
	player.animations = new Anim[player.num_animations];

	//count how many frames for each animation, assign memory for those frames and set id frame start
	node = player_file.child("tileset").child("tile");
	for (uint i = 0; i < player.num_animations; ++i) {
		player.animations[i].FrameCount(node.child("animation").child("frame"));
		player.animations[i].frames = new SDL_Rect[player.animations[i].num_frames];
		player.animations[i].id = node.attribute("id").as_uint();
		node = node.next_sibling("tile");
	}

	//fill frame array with current information
	node = player_file.child("tileset").child("tile");
	pugi::xml_node node_frame;
	for (uint i = 0; i < player.num_animations; ++i) {
		node_frame = node.child("animation").child("frame");
		for (uint j = 0; j < player.animations[i].num_frames; ++j) {
			player.animations[i].frames[j] = player.tileset.GetTileRect(node_frame.attribute("tileid").as_uint());
			node_frame = node_frame.next_sibling("frame");
		}
		node = node.next_sibling("tile");
	}
	//LOG all animation information
	for (uint i = 0; i < player.num_animations; ++i) {
		LOG("Animation %i--------", player.animations[i].id);
		for (uint j = 0; j < player.animations[i].num_frames; ++j) {
			LOG("frame %i x: %i y: %i w: %i h: %i",
				j, player.animations[i].frames[j].x, player.animations[i].frames[j].y,
				player.animations[i].frames[j].w, player.animations[i].frames[j].h);
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

		node = node.next_sibling();
	}

	//Load all colliders saved in the first tile
	node = player_file.child("tileset").child("tile").child("objectgroup").child("object");
	while (node) {
		nameIdentificator = node.attribute("name").as_string();
		
		if (nameIdentificator == "Collider") {
			playerColliders.colliderPlayer.offset.x = node.attribute("x").as_int();
			playerColliders.colliderPlayer.offset.y = node.attribute("y").as_int();
			playerColliders.colliderPlayer.width = node.attribute("width").as_uint();
			playerColliders.colliderPlayer.height = node.attribute("height").as_uint();
			playerColliders.colliderPlayer.type = COLLIDER_TYPE::COLLIDER_PLAYER;
		}

		else if (nameIdentificator == "ColliderGround") {
			playerColliders.colliderPlayer_ground.offset.x = node.attribute("x").as_int();
			playerColliders.colliderPlayer_ground.offset.y = node.attribute("y").as_int();
			playerColliders.colliderPlayer_ground.width = node.attribute("width").as_uint();
			playerColliders.colliderPlayer_ground.height = node.attribute("height").as_uint();
			playerColliders.colliderPlayer_ground.type = COLLIDER_TYPE::COLLIDER_PLAYER_GROUND;
		}
		else if (nameIdentificator == "ColliderLeft") {
			playerColliders.colliderPlayer_left.offset.x = node.attribute("x").as_int();
			playerColliders.colliderPlayer_left.offset.y = node.attribute("y").as_int();
			playerColliders.colliderPlayer_left.width = node.attribute("width").as_uint();
			playerColliders.colliderPlayer_left.height = node.attribute("height").as_uint();
			playerColliders.colliderPlayer_left.type = COLLIDER_TYPE::COLLIDER_PLAYER_LEFT;
		}
		else if (nameIdentificator == "ColliderRight") {
			playerColliders.colliderPlayer_right.offset.x = node.attribute("x").as_int();
			playerColliders.colliderPlayer_right.offset.y = node.attribute("y").as_int();
			playerColliders.colliderPlayer_right.width = node.attribute("width").as_uint();
			playerColliders.colliderPlayer_right.height = node.attribute("height").as_uint();
			playerColliders.colliderPlayer_right.type = COLLIDER_TYPE::COLLIDER_PLAYER_RIGHT;
		}
		else if (nameIdentificator == "ColliderUp") {
			playerColliders.colliderPlayer_up.offset.x = node.attribute("x").as_int();
			playerColliders.colliderPlayer_up.offset.y = node.attribute("y").as_int();
			playerColliders.colliderPlayer_up.width = node.attribute("width").as_uint();
			playerColliders.colliderPlayer_up.height = node.attribute("height").as_uint();
			playerColliders.colliderPlayer_up.type = COLLIDER_TYPE::COLLIDER_PLAYER_UP;
		}

		node = node.next_sibling();
	}

	
	//Convert id animations to enum
	for (uint i = 0; i < player.num_animations; ++i) {
		switch (player.animations[i].id) {
		case 0:
			player.animations[i].animType = PlayerState::IDLE;
			break;
		case 16:
			player.animations[i].animType = PlayerState::WALKING;
			break;
		case 32:
			player.animations[i].animType = PlayerState::JUMP;
			break;
		case 35:
			player.animations[i].animType = PlayerState::FALL;
			break;
		case 36:
			player.animations[i].animType = PlayerState::LAND;
			break;
		case 64:
			player.animations[i].animType = PlayerState::DEAD;
			break;
		case 80:
			player.animations[i].animType = PlayerState::HADOUKEN;
			break;
		case 96:
			player.animations[i].animType = PlayerState::PUNCH;
			break;
		default:
			player.animations[i].animType = PlayerState::UNKNOWN;
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

