#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"


j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene2");
}

// Destructor
j1Scene2::~j1Scene2()
{}


// Called before render is available
bool j1Scene2::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	if (App->scene->active == true)
	{
		map.create(config.child("map").child_value());
		active = false;
	}
	

	return ret;
}


// Called before the first frame
bool j1Scene2::Start()
{
	if (active)
	{
		App->map->Load(map.GetString());
		App->audio->PlayMusic(App->map->data.musicEnvironment);
		
		win_width = App->win->screen_surface->w;
		win_height = App->win->screen_surface->h;
		cameraOffset.x = win_width * 0.5f / App->win->GetScale() - App->map->WorldToMap(App->render->camera.x, App->render->camera.y).x;
		LOG("%d %d", App->player->position.x, App->player->position.y);
		cameraOffset.y = -App->render->camera.y - App->player->position.y;

		App->map->ColliderPrint();
	}

	return true;
}
// Called each loop iteration
bool j1Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;


	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);



	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d MousePos:%d,%d PlayerPos:%.2f,%.2f, CameraPos: %d,%d",

		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y, x, y,
		App->player->position.x, App->player->position.y, App->render->camera.x, App->render->camera.y);


	App->win->SetTitle(title.GetString());
	return true;
}


// Called each loop iteration
bool j1Scene2::PostUpdate()
{
	bool ret = true;

	App->render->camera.x = cameraOffset.x * App->win->GetScale() - App->player->position.x* App->win->GetScale();
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}


// Called before quitting
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");
	
	return true;
}