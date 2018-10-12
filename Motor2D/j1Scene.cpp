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

	map.create(conf.child("map").child_value());

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(map.GetString());
	App->audio->PlayMusic(App->map->data.musicEnvironment);
	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;
	camPos = win_width;
	App->map->ColliderPrint();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
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


	/*if (App->player->position.x > camPos / 3 && App->player->position.x <= camPos) {

		App->render->camera.x = -(App->player->position.x + camPos / 1.5);
		camPos = camPos + win_width;
		camPosMin = camPosMin + win_width;
	}*/


	if (App->player->position.x >= 0 && App->player->position.x <= win_width)
		App->render->camera.x = -(App->player->position.x + (App->player->speed.x));

	


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

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
