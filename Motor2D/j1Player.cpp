#include "j1App.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "p2List.h"
#include "j1Input.h"
#include <cstring>


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
	
	// Load animation pointers
	PushBack(IDLE);
	PushBack(WALKING_RIGHT);
	PushBack(WALKING_LEFT);

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{

	player.tileset.texture = App->tex->Load(player.tileset.imagePath.GetString());

	//This method returns player object's position
	position = App->map->GetInitialPosition();
	
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
	
	CheckState(speed);
	Actions();

	App->render->Blit(player.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame());

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
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
		if (speed.x < maxSpeedX) {
			speed.x += incrementSpeed;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
		if (speed.x > -1 * maxSpeedX) {
			speed.x -= incrementSpeed;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
		speed.x = 0.0f;
	}

	position.x += speed.x;

}
void j1Player::PushBack(const int anim_type) {

	
	Animation* aux_anim = new Animation;
	
	aux_anim->name = player.animations[anim_type].name;

	if (!strcmp(aux_anim->name, "idle"))
		idle = aux_anim;
	
	else if (!strcmp(aux_anim->name, "walking_right"))
		walking_right = aux_anim;
		
	else if (!strcmp(aux_anim->name, "walking_left"))
		walking_left = aux_anim;
		
	else if (!strcmp(aux_anim->name, "jumping"))
		jumping = aux_anim;

	else if (!strcmp(aux_anim->name, "falling"))
		falling = aux_anim;

	for (uint i = 0; i < player.animations[anim_type].num_frames; ++i) {
		aux_anim->PushBack(player.animations[anim_type].frames[i]);
	}
	
	//delete aux_anim;
}

bool j1Player::Load(pugi::xml_node&)
{
	return true;
}

bool j1Player::Save(pugi::xml_node&) const
{
	return true;
}

SDL_Rect TileSetPlayer::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}

uint Anim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}

void j1Player::CheckState(fPoint speed) {

	//IN PROGRESS 

	if (speed.x != 0)
	{
		if (speed.x > 0)
		{
			state = WALKING_RIGHT;

		}
		else if (speed.x < 0)
		{

			state = WALKING_LEFT;
		}
	}

	else
	{
		state = IDLE;
	}
}

void j1Player::Actions() {

	switch (state)
	{
	case IDLE:
		current_animation = idle;
		current_animation->speed = 0.025f;
		break;
	case WALKING_RIGHT:
		current_animation = walking_right;
		current_animation->speed = 0.025f;
		break;
	case WALKING_LEFT:
		current_animation = walking_right; //it doesnt exist :s
		current_animation->speed = 0.025f;
		break;

	case JUMP:
		break;
	case FALL:
		break;
	case LAND:
		break;
	case RUN:
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
		player.animations[i].name = node.child("animation").attribute("name").value();
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
	//LOG all information in animations
	for (uint i = 0; i < player.num_animations; ++i) {
		LOG("Animation %i--------", player.animations[i].id);
		for (uint j = 0; j < player.animations[i].num_frames; ++j) {
			LOG("frame %i x: %i y: %i w: %i h: %i",
				j, player.animations[i].frames[j].x, player.animations[i].frames[j].y,
				player.animations[i].frames[j].w, player.animations[i].frames[j].h);
		}
	}

	return ret;
}