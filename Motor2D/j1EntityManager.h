#ifndef __J1ENEMIES_H__
#define __J1ENEMIES_H__

#include "j1Module.h"

#define MAX_ENEMIES 100


enum ENTITY_TYPES
{
	NO_TYPE,
	PLAYER,
	GLADIATOR
};

class j1Entity;

struct EnemyData
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
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
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENTITY_TYPES type, int x, int y, p2SString tsx_file, int path_type = 0);

public:

	
	j1Entity* enemies[MAX_ENEMIES];

	p2SString tsx1;
	p2SString tsx2;

private:

	void SpawnEnemy(const EnemyData& info);
	SDL_Texture* sprites;
	//SDL_Texture* gladiatorSprite = nullptr;

	

private:

	EnemyData queue[MAX_ENEMIES];


};

#endif // __J1ENEMIES_H__
