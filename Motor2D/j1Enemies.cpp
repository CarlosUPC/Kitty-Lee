#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Enemies.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Enemy.h"
#include "Gladiator.h"

//Include all enemies


#define SPAWN_MARGIN 50
#define SCREEN_SIZE 1

j1Enemies::j1Enemies()
{
	name.create("enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{
}


bool j1Enemies::Awake(pugi::xml_node& conf)
{
	LOG("Loading TSX files");
	bool ret = true;

	// TSX of each enemy
	tsx1.create(conf.child("enemy_1").text().as_string());
	

	return ret;
}


bool j1Enemies::Start()
{
	LOG("loading enemies");
	gladiatorSprite = App->tex->Load("textures/enemies/Gladiator.png");

	return true;
}

bool j1Enemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Enemies::Update(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] != nullptr)
		{
			enemies[i]->Move();

		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] != nullptr) {
			enemies[i]->Draw(gladiatorSprite);


		}
	}

	return true;
}

bool j1Enemies::PostUpdate()
{
	// check camera position to decide what to despawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->position.x * SCREEN_SIZE < (App->render->camera.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->position.x * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}

	return true;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i] != nullptr)
			{
				delete enemies[i];
				enemies[i] = nullptr;
			}
			if (queue[i].type != ENEMY_TYPES::NO_TYPE)
			{
				queue[i].type = ENEMY_TYPES::NO_TYPE;
			}
		}
	}

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y, p2SString tsx_file, int path_type)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			queue[i].tsx_file = tsx_file;
			queue[i].path_type = path_type;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyData& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::GLADIATOR:
			enemies[i] = new Gladiator(info.x, info.y,info.tsx_file,info.path_type);
			break;
		}
	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{

}