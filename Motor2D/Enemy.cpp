
#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Textures.h"

#include "SDL/include/SDL_timer.h"
#include "j1Audio.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2List.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"

#include <cmath>


Enemy::Enemy(int x, int y, p2SString tsx, int type) : position(x, y), enemyTSX(tsx)
{

}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
	{
		App->render->Blit(sprites, position.x, position.y , &(animation->GetCurrentFrame()));
	}

}

void Enemy::OnCollision(Collider* collider)
{


}

bool Enemy::LoadEnemy(const char* file) {

	bool ret = true;

	pugi::xml_parse_result result = enemy_file.load_file(file);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}

	//fill tileset info
	pugi::xml_node node = enemy_file.child("tileset");

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
	data.animations = new EnemyAnim[data.num_animations];

	//count how many frames for each animation, assign memory for those frames and set id frame start
	node = enemy_file.child("tileset").child("tile");
	for (uint i = 0; i < data.num_animations; ++i) {
		data.animations[i].FrameCount(node.child("animation").child("frame"));
		data.animations[i].frames = new SDL_Rect[data.animations[i].num_frames];
		data.animations[i].id = node.attribute("id").as_uint();
		node = node.next_sibling("tile");
	}

	//fill frame array with current information
	node = enemy_file.child("tileset").child("tile");
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



	//Convert id animations to enum
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EnemyState::E_IDLE;
			break;
		case 15:
			data.animations[i].animType = EnemyState::E_WALKING;
			break;
		case 16:
			data.animations[i].animType = EnemyState::E_HIT;
			break;
		case 24:
			data.animations[i].animType = EnemyState::E_DETECTING;
			break;
		case 32:
			data.animations[i].animType = EnemyState::E_DEAD;
			break;
		default:
			data.animations[i].animType = EnemyState::E_UNKNOWN;
			break;
		}
	}

	return ret;
}

//Functions to help loading data in xml-------------------------------------
//Get the rect info of an id of tileset
SDL_Rect TileSetEnemy::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}

//Return how many frames are in one animation
uint EnemyAnim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}



/*
void Enemy::DeadAnim()
{
	animation = nullptr;
}


void Enemy::Drop()
{

}
*/