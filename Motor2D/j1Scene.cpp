#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"

#include "Brofiler\Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");
	bool ret = true;

	lvl1.create(conf.child("level_1").child_value());
	lvl2.create(conf.child("level_2").child_value());

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(lvl1.GetString());

	App->map->AddCollidersMap();

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);

	p2List_item<ColliderObject*>* position = App->map->data.colliders.start; //iterate all objects of tile to find entities
	j1Entity* ent = nullptr;

	for (; position; position = position->next) {
		if (position->data->name == "Gladiator")
			ent = App->entities->CreateEntity(j1Entity::Types::GLADIATOR, position->data->coll_x, position->data->coll_y);
		else if (position->data->name == "Player") {
			ent = App->entities->CreateEntity(j1Entity::Types::PLAYER, position->data->coll_x, position->data->coll_y);
			player = (j1Player*)ent;
		}
		else ent = nullptr;

		if (ent != nullptr) {
			ent->data.tileset.texture = App->tex->Load(ent->data.tileset.imagePath.GetString());
		}
	}

	debug_tex = App->tex->Load("maps/path.png");

	//App->audio->PlayMusic(App->map->data.musicEnvironment);

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

	cameraOffset.x = win_width * 0.5f / App->win->GetScale() - App->render->camera.x;
	cameraOffset.y = win_height * 0.5f / App->win->GetScale() - App->render->camera.y;

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateScene", Profiler::Color::Yellow);
	CheckLevel();
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("UpdateScene", Profiler::Color::Red);
	//----------------------DEBUG KEYS-------------------------//
	

	//F1 - Start from the very first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (!isLevel1) App->fade->FadeToBlack();
		else {
			player->speed.SetToZero();
			App->render->CameraInitPos();
		}
	}

	//F2 - Start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || player->position.y > App->map->data.height*App->map->data.tile_height){
		player->position = player->spawn_position;
		player->speed.SetToZero();
		//App->render->camera = App->render->CameraInitPos();
		}
	
	//F3 - Increase music volume
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->audio->RaiseVolume();

	//F4 - Decrease music volume
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->audio->DecreaseVolume();

	//F5 - Save the currant state
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	//F6 - Load the previous state (even across levels)
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	//F8 - Switch between levels
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->fade->FadeToBlack();
	
	//F10 - God Mode
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->collider->GodMode();


	
	

	App->map->Draw();

	if (App->collider->debug) {

		SDL_Rect path_rect = { 0,0,16,16 };

		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y, &path_rect);
		}
	}
	
	//iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateScene", Profiler::Color::Green);
	bool ret = true;

	int offsetPlayerPositionX = player->collider.width - player->data.tileset.tilewidth;
	int offsetPlayerPositionY = player->collider.height - player->data.tileset.tileheight;
	
	if ((cameraOffset.x - player->position.x + offsetPlayerPositionX) * App->win->GetScale() < 0 &&
		(cameraOffset.x + player->position.x - offsetPlayerPositionX) * App->win->GetScale() < App->map->data.width*App->map->data.tile_width*App->win->GetScale()) {
		App->render->camera.x = (cameraOffset.x - player->position.x + offsetPlayerPositionX) * App->win->GetScale();
	}
	if ((cameraOffset.y - player->position.y + offsetPlayerPositionY) * App->win->GetScale() < 0 &&
		(cameraOffset.y + player->position.y - offsetPlayerPositionY) * App->win->GetScale() < App->map->data.height*App->map->data.tile_height*App->win->GetScale()) {
		App->render->camera.y = (cameraOffset.y - player->position.y + offsetPlayerPositionY) * App->win->GetScale();
	}
	

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::CheckLevel()
{
	switch (stg)
	{
	case LEVEL_1:
		App->fade->level1 = true;
		App->fade->level2 = false;
		isLevel1 = true;
		break;
	case LEVEL_2:
		App->fade->level2 = true;
		App->fade->level1 = false;
		isLevel1 = false;
		break;
	default:
		break;
	}
}


bool j1Scene::Load(pugi::xml_node& data)
{
	App->fade->num_level = data.child("levels").attribute("level").as_int();
	return true;

}
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node player_node = data.append_child("levels");

	player_node.append_attribute("level") = App->fade->num_level;
	
	return true;
}

