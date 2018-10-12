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
	Stages stg = LEVEL_1;
	App->map->Load(lvl1.GetString());
	App->audio->PlayMusic(App->map->data.musicEnvironment);
	
	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

	cameraOffset.x = win_width * 0.5f / App->win->GetScale() - App->render->camera.x;
	cameraOffset.y = win_height * 0.5f / App->win->GetScale() - App->render->camera.y;

	App->map->AddCollidersMap();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	CheckLevel();
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		SwitchingLevel();

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->player->position = App->map->GetInitialPosition();


	App->map->Draw();
	


	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d MousePos:%d,%d PlayerPos:%.2f,%.2f, CameraPos: %d,%d",

		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y, x, y,
		App->player->position.x, App->player->position.y, App->render->camera.x,App->render->camera.y);


	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	int offsetPlayerPositionX = App->player->colliderInfo.x - App->player->player.tileset.tilewidth;

	if ((cameraOffset.x - App->player->position.x + offsetPlayerPositionX) * App->win->GetScale() < 0)
		App->render->camera.x = (cameraOffset.x - App->player->position.x + offsetPlayerPositionX) * App->win->GetScale();
	if ((cameraOffset.y - App->player->position.y) * App->win->GetScale() < 0)
		App->render->camera.y = (cameraOffset.y - App->player->position.y) * App->win->GetScale();
	

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
		level1 = true;
		level2 = false;
		break;
	case LEVEL_2:
		level2 = true;
		level1 = false;
		break;
	default:
		break;
	}
}

void j1Scene::SwitchingLevel() {

	if (level1) {
		App->player->position = App->map->GetInitialPosition();
	}

	else if (level2) {
		//Switch to level 1
		App->map->CleanUp();
		App->collider->EraseMapCollider();
		App->map->Load(lvl1.GetString());
		App->player->position = App->map->GetInitialPosition();
	}


}


