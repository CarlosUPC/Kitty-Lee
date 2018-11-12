#ifndef __J1ENEMIES_H__
#define __J1ENEMIES_H__

#include "j1Module.h"

#define MAX_ENEMIES 100


enum ENEMY_TYPES
{
	NO_TYPE,
	GLADIATOR
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y, path_type;
};

class j1Enemies : public j1Module
{
public:

	j1Enemies();
	~j1Enemies();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y, int path_type = 0);

public:

	
	Enemy* enemies[MAX_ENEMIES];
	

private:

	void SpawnEnemy(const EnemyInfo& info);
	SDL_Texture* sprites;
	SDL_Texture* gladiatorSprite = nullptr;

private:

	EnemyInfo queue[MAX_ENEMIES];


};

#endif // __J1ENEMIES_H__
