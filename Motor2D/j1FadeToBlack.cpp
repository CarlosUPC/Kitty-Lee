#include <math.h>
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "PLAYER.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1MainMenu.h"

#include "Brofiler/Brofiler.h"


j1FadeToBlack::j1FadeToBlack()
{
	screen = { 0, 0, 1920, 1080 };
}

j1FadeToBlack::~j1FadeToBlack()
{}

bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool j1FadeToBlack::PostUpdate()
{
	BROFILER_CATEGORY("UpdateFade", Profiler::Color::Red);
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now >= total_time)
		{

			if (to_fade == MODULE) {
				fade_out->CleanUp();
				fade_out->active = false;
				fade_in->Start();
				fade_in->active = true;
			}

			else if (to_fade == STAGE) {
				//App->map->CleanUp();
				App->entities->CleanUp();
				/*App->collider->CleanUp();
				App->entities->CleanUp();*/
				if (STAGE1) {
					App->map->Load(App->scene->lvl2.GetString());
					App->scene->stg = STAGE2;
				}
				else {
					App->map->Load(App->scene->lvl1.GetString());
					App->scene->stg = STAGE1;
				}
				App->map->AddCollidersMap();
				App->scene->SetWalkabilityMap();
				App->scene->CreateEntities();
			}

			fade_out = nullptr;
			fade_in = nullptr;

			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = fade_step::fade_from_black;

			fading = false;
		}
	} break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
		{
			current_step = fade_step::none;
		}


	} break;
	default:
		break;
	}
	

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);
	return true;
}

bool j1FadeToBlack::CleanUp()
{
	return true;
}

bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{

		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);

		fade_out = module_off;
		fade_in = module_on;

		to_fade = MODULE;
	
		fading = true;
		ret = true;
	}

	return ret;
	
}

void j1FadeToBlack::ChangeStage()
{
	if (current_step == fade_step::none)
	{
		App->map->CleanUp();
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(1 * 0.5f * 1000.0f);

		fade_out = nullptr;
		fade_in = nullptr;
		to_fade = STAGE;

		fading = true;
	}
}


//bool j1FadeToBlack::SwitchingLevel(const char* tmx_map)
//{
//	bool ret = true;
//
//	if (App->menu->active) {
//		App->menu->CleanUp();
//		App->menu->active = false;
//		App->scene->active = true;
//	}
//
//	App->collider->EraseMapCollider();
//	App->entities->CleanUp();
//	App->map->CleanUp();
//
//	//App->map->Load(tmx_map);
//	//App->map->AddCollidersMap();
//
//	App->scene->Start();
//
//	/*App->scene->SetWalkabilityMap();
//	App->scene->CreateEntities();*/
//
//	return ret;
//}

bool j1FadeToBlack::IsFading()
{
	return fading;
}