#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL_timer.h"
#include "p2SString.h"
#include "j1Module.h"	
#include "p2List.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
struct Collider;


struct TileSetEnemy {

	SDL_Rect GetTileRect(int id) const;

	p2SString name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	p2SString imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

enum EnemyState {
	E_IDLE = 0,
	E_WALKING,
	E_HIT,
	E_DETECTING,
	E_DEAD,
	E_UNKNOWN
};

struct EnemyAnim {
	uint id = 0;
	uint num_frames = 0;
	SDL_Rect* frames = nullptr;
	EnemyState  animType;
	uint FrameCount(pugi::xml_node&);
};

struct EnemyInfo {
	TileSetEnemy tileset; //will only use one for the player
	EnemyAnim* animations = nullptr;
	uint num_animations = 0;
};



class j1Entity
{
protected:

	float e_animationSpeed;
	
	Animation* e_animation = nullptr;

	Animation e_anim_idle;
	Animation e_anim_walking;
	Animation e_anim_hit;
	Animation e_anim_detecting;
	Animation e_anim_death;
	
	void PushBack();
public:

	Collider* collider = nullptr;
	fPoint position;
	fPoint original_pos;
	p2SString enemyTSX;
	SDL_Texture* sprite = nullptr;

	pugi::xml_document	enemy_file;

	EnemyInfo data;


public:
	j1Entity(int x, int y, p2SString tsx, int type);
	virtual ~j1Entity();

	const Collider* GetCollider() const;

	bool LoadEnemy(const char*);
	
	virtual void Move(float dt) {};
	virtual void Draw() {};
	virtual void OnCollision(Collider* collider);

	//virtual void ExtraAnim(SDL_Texture* texture) {};
	//virtual void DeadAnim();
	//virtual void Drop();

private:
	
};

#endif // __ENTITY_H__
