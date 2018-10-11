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
	
	// Load animation
	PushBack();

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{

	player.tileset.texture = App->tex->Load(player.tileset.imagePath.GetString());

	//This method returns player object's position
	position = App->map->GetInitialPosition();
	collPlayer = App->collider->AddCollider({ (int)position.x + offset.x, (int)position.y + offset.y, collider.x, collider.y }, COLLIDER_PLAYER, this);
	//Speed of player
	speed = { 0,0 };

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
	Actions();

	//Player collider update
	collPlayer->SetPos(position.x + offset.x, position.y + offset.y);
	if(App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) 
		LOG("Player position: (%.2f, %.2f)", position.x, position.y);

	App->render->Blit(player.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(), 1.0F, flip);
	
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	for (uint i = 0; i < player.num_animations; ++i) {
		delete[] player.animations[i].frames;
		player.animations[i].frames = nullptr;
	}
	delete[] player.animations;
	player.animations = nullptr;

	App->tex->UnLoad(player.tileset.texture);
	return true;
}

void j1Player::Movement() {
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && speed.x < maxSpeedX) {
			speed.x += incrementSpeedX;
			App->audio->PlayFx(3, -1); //Walk fx
			
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && speed.x > -maxSpeedX) {
			speed.x -= incrementSpeedX;
			App->audio->PlayFx(3, -1); //Walk fx
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
		speed.x = 0.0f;
		App->audio->FadeOutFx(3, 200); //Walk fx
		
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN && air == false) {
		speed.y = jumpSpeed;
		air = true;
		App->audio->PlayFx(2); //Jump fx
	}
	if (air)
		speed.y += App->map->data.gravity;

	position.x += speed.x;
	

	if (position.y <= App->map->GetInitialPosition().y && air == true) { //just while we don't have collision system player will stop in initial y
		position.y += speed.y;
	}
	else { position.y = App->map->GetInitialPosition().y;  air = false; }
	

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
	pugi::xml_node player = data.append_child("player");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}

void j1Player::CheckState() {

	//IN PROGRESS 

	if (speed.x != 0)
	{
		state = WALKING;
		if (speed.x > 0 && flip != SDL_FLIP_NONE)
		{
			flip = SDL_FLIP_NONE;
		}
		else if (speed.x < 0 && flip != SDL_FLIP_HORIZONTAL)
		{
			flip = SDL_FLIP_HORIZONTAL;
		}
	}

	if (air) {
		state = FALL;
	}

	if (!air&&speed.x == 0)
	{
		state = IDLE;
	}
}

void j1Player::Actions() {

	switch (state)
	{
	case IDLE:
		current_animation = &anim_idle;
		break;
	case WALKING:
		current_animation = &anim_walking;
		break;
	case JUMP:
		current_animation = &anim_jump;
		break;
	case FALL:
		current_animation = &anim_fall;
		break;
	case LAND:
		current_animation = &anim_land;
		break;
	default:
		break;
	}

	current_animation->speed = animationSpeed;

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
	p2SString nameProperty;
	while (node) {
		nameProperty = node.attribute("name").as_string();

		if (nameProperty == "animationSpeed")
			animationSpeed = node.attribute("value").as_float();

		else if (nameProperty == "colliderWidth")
			collider.x = node.attribute("value").as_int();

		else if (nameProperty == "colliderHeight")
			collider.y = node.attribute("value").as_int();

		else if (nameProperty == "colliderOffsetX")
			offset.x = node.attribute("value").as_int();

		else if (nameProperty == "colliderOffsetY")
			offset.y = node.attribute("value").as_int();

		else if (nameProperty == "incrementSpeedX")
			incrementSpeedX = node.attribute("value").as_float();

		else if (nameProperty == "jumpSpeed")
			jumpSpeed = node.attribute("value").as_float();

		else if (nameProperty == "maxSpeedX")
			maxSpeedX = node.attribute("value").as_float();

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

