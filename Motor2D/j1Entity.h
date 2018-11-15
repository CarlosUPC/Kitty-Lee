#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL_timer.h"
#include "p2SString.h"
#include "j1Module.h"	
#include "p2List.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
struct Collider;

struct TileSetEntity {

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

enum class EntityState {
	IDLE = 0,
	WALKING,
	HIT,
	DETECTING,
	DEAD,
	JUMP,
	FALL,
	LAND,
	HADOUKEN,
	PUNCH,
	IDLE_GHOST,
	WALKING_GHOST,
	JUMP_GHOST,
	FALL_GHOST,
	LAND_GHOST,
	UNKNOWN
};

struct EntityAnim {
	uint id = 0;
	uint num_frames = 0;
	SDL_Rect* frames = nullptr;
	EntityState animType;

	uint FrameCount(pugi::xml_node&);
};

struct EntityInfo {
	TileSetEntity tileset;
	EntityAnim* animations = nullptr;
	uint num_animations = 0;
};

struct COLLIDER_INFO {
	Collider* collider = nullptr;
	iPoint offset;
	int width = 0;
	int height = 0;
	COLLIDER_TYPE type;
};

class j1Entity
{
public:

	enum class Types
	{
		PLAYER,
		GLADIATOR,
		UNKNOWN
	};

public:

	j1Entity(Types type);
	virtual ~j1Entity();

	const Collider* GetCollider() const;

	bool LoadEntityData(const char*);
	virtual void LoadProperties(pugi::xml_node&);
	virtual void LoadCollider(pugi::xml_node&);
	virtual void IdAnimToEnum();
	virtual void Pushback() {};

	virtual bool Start() { return true; }
	virtual bool Update(float dt);
	virtual void Move(float dt) {}
	virtual void Draw(float dt);
	virtual void CreatePath() {}
	virtual void OnCollision(Collider* collider);
	virtual bool CleanUp();
	void DeleteAnimation();

	//virtual void ExtraAnim(SDL_Texture* texture) {};
	//virtual void DeadAnim();
	//virtual void Drop();	
	

	fPoint position;

	Types type;

	COLLIDER_INFO collider;

	EntityInfo data;

	pugi::xml_document	entity_file;
	
	Animation* current_animation = nullptr;
	float animationSpeed;

	const p2DynArray<iPoint>* entityPath;
	uint entityPathSize = 0;

	//void PushBack();
};
#endif // __ENTITY_H__