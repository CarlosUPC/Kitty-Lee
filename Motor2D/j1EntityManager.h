#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "p2DynArray.h"

#define MAX_ENEMIES 10

struct EntityData
{
	j1Entity::Types type = j1Entity::Types::UNKNOWN;
	int x, y, path_type;
	p2SString tsx_file;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();
	
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool UpdateAll(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	j1Entity* CreateEntity(j1Entity::Types type);
	void DestroyEntity(j1Entity* entity);

private:

	//void SpawnEnemy(const EntityData& info);
	//SDL_Texture* sprites;
	//SDL_Texture* gladiatorSprite = nullptr;

	

private:

	bool do_logic = true;
	uint32 accumulated_time = 0;
	uint32 update_ms_cycle = 500;
	p2DynArray<j1Entity*> entities;
	EntityData queue[MAX_ENEMIES];

};

#endif // __J1ENEMIES_H__
