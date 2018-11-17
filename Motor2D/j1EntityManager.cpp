#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "GLADIATOR.h"

#include "Brofiler\Brofiler.h"

//Include all enemies

#define SPAWN_MARGIN 50
#define SCREEN_SIZE 1

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{
}


bool j1EntityManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading TSX files");
	bool ret = true;

	// TSX of each enemy
	pugi::xml_node node = conf.child("entity");
	for (int i = 0; node; node = node.next_sibling()) {
		queue[i].tsx_file.create(node.attribute("file").as_string());
		++i;
	}
	
	return ret;
}


bool j1EntityManager::Start()
{
	LOG("loading enemies");

	return true;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateEntityManager", Profiler::Color::Yellow);
	// check camera position to decide what to spawn
	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (queue[i].type != j1Entity::Types::UNKNOWN)
	//	{
	//		if (queue[i].x * SCREEN_SIZE < App->render->camera.x + (App->render->camera.w * SCREEN_SIZE) + SPAWN_MARGIN) 
	//		{
	//			//SpawnEnemy(queue[i]);
	//			queue[i].type = j1Entity::Types::UNKNOWN;
	//			LOG("Spawning enemy at %d", queue[i].x * SCREEN_SIZE);
	//		}
	//	}
	//}

	return true;
}

// Called before render is available
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEntityManager", Profiler::Color::Red);
	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;
	UpdateAll(dt, do_logic);
	if (do_logic == true) {
		accumulated_time = 0.0f;
		do_logic = false;
	}
	return true;
	/*for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (entities[i] != nullptr)
		{
			entities[i]->Move(dt);
		}
	}

	for (uint i = 0; i < MAX_ENEMIES;++i) {
		if (entities[i] != nullptr)
		{
			entities[i]->Draw(dt);
		}
	}

	return true;*/
}

bool j1EntityManager::UpdateAll(float dt, bool do_logic)
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); ++i) {
		entities[i]->Move(dt);
		entities[i]->Update(dt);

		if (do_logic) {
			entities[i]->CreatePath();
		}
	}
	
	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::Green);

	for (int i = 0; i < entities.Count(); ++i)
		entities[i]->Draw();

	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	for (int i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr) {
			entities[i]->CleanUp();
			RELEASE(entities[i]);
		}
	}

	return true;
}

j1Entity* j1EntityManager::CreateEntity(j1Entity::Types type, int PositionX, int PositionY)
{
	static_assert(j1Entity::Types::UNKNOWN == (j1Entity::Types)2, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
		case j1Entity::Types::GLADIATOR: ret = new Gladiator(PositionX, PositionY); break;
		case j1Entity::Types::PLAYER: ret = new j1Player(PositionX, PositionY); break;
	}
	if (ret != nullptr) {
		entities.PushBack(ret);
		ret->Start();
	}
	
	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity * entity)
{
	if (entity != nullptr) {
		entity->CleanUp();
		entities.Pop(entity);
		delete entity;
		entity = nullptr;
	}
}



//void j1EntityManager::SpawnEnemy(const EntityData& info)
//{
//	// find room for the new enemy
//	uint i = 0;
//	for (; entities[i] != nullptr && i < MAX_ENEMIES; ++i);
//
//	if (i != MAX_ENEMIES)
//	{
//		switch (info.type)
//		{
//		case j1Entity::Types::GLADIATOR:
//			entities[i] = new Gladiator();
//			break;
//		}
//	}
//}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{

}