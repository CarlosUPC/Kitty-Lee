#include "j1App.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"


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
	LOG("Loading Player Module");
	
	LoadPlayer(node.child("file").text().as_string());
	PushBack();
	animation.speed = 0.025f;

	return true;
}

// Called before the first frame
bool j1Player::Start()
{

	player.tileset.texture = App->tex->Load(player.tileset.imagePath.GetString());

	//This method returns player object's position
	position = App->map->GetInitialPosition();

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
	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	App->render->Blit(player.tileset.texture, position.x, position.y, &animation.GetCurrentFrame());
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	for (int i = 0; i < player.num_animations; ++i) {
		delete[] player.animations[i].frames;
		player.animations[i].frames = nullptr;
	}
	delete[] player.animations;
	player.animations = nullptr;

	App->tex->UnLoad(player.tileset.texture);
	return true;
}

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
	for (int i = 0; i < player.num_animations; ++i) {
		player.animations[i].FrameCount(node.child("animation").child("frame"));
		player.animations[i].frames = new SDL_Rect[player.animations[i].num_frames];
		player.animations[i].id = node.attribute("id").as_uint();
		node = node.next_sibling("tile");
	}

	//fill frame array with current information
	node = player_file.child("tileset").child("tile");
	pugi::xml_node node_frame;
	for (int i = 0; i < player.num_animations; ++i) {
		node_frame = node.child("animation").child("frame");
		for (int j = 0; j < player.animations[i].num_frames; ++j) {
			player.animations[i].frames[j] = player.tileset.GetTileRect(node_frame.attribute("tileid").as_uint());
			node_frame = node_frame.next_sibling("frame");
		}
		node = node.next_sibling("tile");
	}
	//LOG all information in animations
	for (int i = 0; i < player.num_animations; ++i) {
		LOG("Animation %i--------", player.animations[i].id);
			for (int j = 0; j < player.animations[i].num_frames; ++j) {
				LOG("frame %i x: %i y: %i w: %i h: %i",
					j, player.animations[i].frames[j].x, player.animations[i].frames[j].y,
					player.animations[i].frames[j].w, player.animations[i].frames[j].h);
			}
	}

	return ret;
}

void j1Player::PushBack() {
	for (int i = 0; i < player.animations[0].num_frames; ++i) {
		animation.PushBack(player.animations[0].frames[i]);
	}
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
	uint ret = 0;
	pugi::xml_node node = n;
	for (node; node != NULL; node = node.next_sibling("frame")) {
		ret++;
	}
	num_frames = ret;

	return ret;
}