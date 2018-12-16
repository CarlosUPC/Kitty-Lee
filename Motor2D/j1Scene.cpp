#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "PLAYER.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "Image.h"
#include "Label.h"
#include "Slider.h"
#include "Button.h"
#include "j1Scene.h"


#include "Brofiler/Brofiler.h"

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
	if(App->current_lvl == Levels::FIRST_LEVEL)
		App->map->Load(lvl1.GetString());
	else App->map->Load(lvl2.GetString());

	App->map->AddCollidersMap();

	SetWalkabilityMap();

	CreateEntities();

	debug_tex = App->tex->Load("maps/path.png");

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

	App->gui->CreateScreen();
	start_time = SDL_GetTicks();
	str_time.create("00:00");
	label_timer = App->gui->CreateLabel(0, 0, str_time.GetString(), false, false, App->gui->screen,WHITE,58);
	label_timer->SetPosRespectParent(CENTERED_UP, 30);

	if (App->GetPause())
		App->Pause();

	/*new_game_btn = (Button*)App->gui->CreateButton(10 , 10, { 182,148,189,49 }, App->gui->screen, { 181,92,191,49 }, { 181,42,190,45 });
	
	new_game_lbl = (Label*)App->gui->CreateLabel(20, 5, "PLAY", false, false, new_game_btn, WHITE, 20, "fonts/Munro.ttf");*/

	App->render->camera = App->render->CameraInitPos();

	cameraOffset.x = (int)(win_width * 0.5f / App->win->GetScale() - App->render->camera.x);
	cameraOffset.y = (int)(win_height * 0.5f / App->win->GetScale() - App->render->camera.y);

	return true;
}

void j1Scene::SetWalkabilityMap()
{
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);
}

void j1Scene::CreateEntities()
{
	p2List_item<ColliderObject*>* position = App->map->data.colliders.start; //iterate all objects of tile to find entities
	j1Entity* ent = nullptr;

	for (; position; position = position->next) {
		if (position->data->name == "Gladiator")
			ent = App->entities->CreateEntity(j1Entity::Types::GLADIATOR, position->data->coll_x, position->data->coll_y);
		else if (position->data->name == "Flying Tongue")
			ent = App->entities->CreateEntity(j1Entity::Types::FLYING_TONGUE, position->data->coll_x, position->data->coll_y);
		else if (position->data->name == "Player") {
			ent = App->entities->CreateEntity(j1Entity::Types::PLAYER, position->data->coll_x, position->data->coll_y);
			player = (Player*)ent;
		}
		else if (position->data->name == "Coin") {
			ent = App->entities->CreateEntity(j1Entity::Types::COIN, position->data->coll_x, position->data->coll_y);
		}
		else ent = nullptr;

		if (ent != nullptr) {
			ent->data.tileset.texture = App->tex->Load(ent->data.tileset.imagePath.GetString());
			
		}
	}
}

void j1Scene::CreateEntitiesFromXML(pugi::xml_node& node)
{
	j1Entity* ent = nullptr;

	for (pugi::xml_node n = node.child("entity"); n; n = n.next_sibling()) {
		ent = App->entities->CreateEntity((j1Entity::Types)n.attribute("type").as_int(), n.attribute("spawn_x").as_float(), n.attribute("spawn_y").as_float());
		if (ent != nullptr) {
			ent->position.create(n.attribute("x").as_float(), n.attribute("y").as_float());
			ent->data.tileset.texture = App->tex->Load(ent->data.tileset.imagePath.GetString());
			if ((j1Entity::Types)n.attribute("type").as_int() == j1Entity::Types::PLAYER) {
				player = (Player*)ent;
				player->SetCoins(n.attribute("coins").as_int());
				player->SetLife(n.attribute("lifes").as_int());
				player->UpdateUI();
			}
		}
	}
}

void j1Scene::ReturnToSpawnPositionEntities()
{
	for (int i = 0; i < App->entities->entities.Count(); ++i) {
		if (App->entities->entities[i] != nullptr)
			App->entities->entities[i]->position = App->entities->entities[i]->spawn_position;
	}
	App->render->CameraInitPos();
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
		App->render->CameraInitPos();
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
	/*if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->fade->FadeToBlack();*/

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		stg = LEVEL_0;
		//App->current_lvl = Levels::MENU;
		//App->fade->FadeToBlack();
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		(App->Pause()) ? CreateMenu() : DestroyMenu();
	}

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

	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateScene", Profiler::Color::Green);
	bool ret = true;

	str_time.create("%.2i:%.2i", (SDL_GetTicks() - start_time) / 60000, (SDL_GetTicks() - start_time)/1000%60);
	label_timer->SetText(str_time.GetString());

	int offsetPlayerPositionX = player->collider.width - player->data.tileset.tilewidth;
	int offsetPlayerPositionY = player->collider.height - player->data.tileset.tileheight;
	
	if ((cameraOffset.x - player->position.x + offsetPlayerPositionX) * App->win->GetScale() < 0 &&
		(cameraOffset.x + player->position.x - offsetPlayerPositionX) * App->win->GetScale() < App->map->data.width*App->map->data.tile_width*App->win->GetScale()) {
		App->render->camera.x = (cameraOffset.x - (int)player->position.x + offsetPlayerPositionX) * App->win->GetScale();
	}
	if ((cameraOffset.y - player->position.y + offsetPlayerPositionY) * App->win->GetScale() < 0 &&
		(cameraOffset.y + player->position.y - offsetPlayerPositionY) * App->win->GetScale() < App->map->data.height*App->map->data.tile_height*App->win->GetScale()) {
		App->render->camera.y = (cameraOffset.y - (int)player->position.y + offsetPlayerPositionY) * App->win->GetScale();
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	bool ret = false;
	LOG("Freeing scene");
	ret = App->tex->UnLoad(debug_tex);
	App->entities->CleanUp();
	App->gui->DeleteAllUIElements();
	
	return true;
}

void j1Scene::UI_Events(UIElement* element) {

	if (element == button_resume && element->current_state == CLICKED_DOWN) {
		DestroyMenu();
	}

	if (element == button_main_menu && element->current_state == CLICKED_DOWN) {
		stg = LEVEL_0;
	}

	if (element == button_save && element->current_state == CLICKED_DOWN) {
		App->SaveGame();
	}

	if (element == button_load && element->current_state == CLICKED_DOWN) {
		App->LoadGame();
		DestroyMenu();
	}

	if (element == slider_volume->GetSliderButton() && element->current_state == CLICKED_REPEAT) {
		if (App->audio->SetVolume(slider_volume->GetSliderValue()) < 10) {
			speaker_cross->drawable = true;
			speaker_sound->drawable = false;
		}
		else {
			speaker_cross->drawable = false;
			speaker_sound->drawable = true;
		}
	}
	
}

void j1Scene::CreateMenu()
{
	int margin = 30;
	panel = App->gui->CreateImage(0, 0, { 14,499,431,512 }, App->gui->screen);
	panel->SetPos((App->win->GetWindowWidth() - panel->position.w )/2, (App->win->GetWindowHeight() - panel->position.h) / 2);
	
	button_resume = App->gui->CreateButton(0, 0, { 181,311,190,49 }, panel, { 181,255,190,49 }, { 181,204,190,49 });
	button_resume->SetPosRespectParent(CENTERED_UP, margin);
	button_resume->AddListener(this);
	label_resume = App->gui->CreateLabel(0,0,"RESUME",false,false,button_resume, WHITE, 20, "fonts/Munro.ttf");
	label_resume->SetPosRespectParent(CENTERED);

	button_main_menu = App->gui->CreateButton(button_resume->position.x, button_resume->position.y + button_resume->position.h + margin/2, { 181,311,190,49 }, panel, { 181,255,190,49 }, { 181,204,190,49 });
	button_main_menu->AddListener(this);
	label_main_menu = App->gui->CreateLabel(0, 0, "MAIN MENU", false, false, button_main_menu, WHITE, 20, "fonts/Munro.ttf");
	label_main_menu->SetPosRespectParent(CENTERED);


	panel_save_load = App->gui->CreateImage(0, 0, { 0,0,190 * 2 + margin / 2, 49 }, panel, false, false, false);
	panel_save_load->SetPos(margin / 2, button_main_menu->position.y + button_main_menu->position.h + 15);

	button_save = App->gui->CreateButton(0, 0, { 181,311,190,49 }, panel_save_load, { 181,255,190,49 }, { 181,204,190,49 });
	button_save->SetPosRespectParent(LEFT_CENTERED);
	button_save->AddListener(this);
	label_save = App->gui->CreateLabel(0, 0, "SAVE", false, false, button_save, WHITE, 20, "fonts/Munro.ttf");
	label_save->SetPosRespectParent(CENTERED);

	button_load = App->gui->CreateButton(0, 0, { 181,311,190,49 }, panel_save_load, { 181,255,190,49 }, { 181,204,190,49 });
	button_load->SetPosRespectParent(RIGHT_CENTERED);
	button_load->AddListener(this);
	label_load = App->gui->CreateLabel(0, 0, "LOAD", false, false, button_load, WHITE, 20, "fonts/Munro.ttf");
	label_load->SetPosRespectParent(CENTERED);

	panel_volume = App->gui->CreateImage(margin, panel_save_load->position.y + panel_save_load->position.h + margin, { 0,0,388,197 }, panel, false, false, false);
	speaker = App->gui->CreateImage(0, 0, { 612,914,44,80 }, panel_volume);
	speaker->SetPosRespectParent(LEFT_UP);
	speaker_sound = App->gui->CreateImage(speaker->position.x + speaker->position.w + margin / 2, speaker->position.y, { 676,885,44,80 }, panel_volume);
	speaker_cross = App->gui->CreateImage(speaker->position.x + speaker->position.w + margin, (speaker->position.h-32)/2, { 679,972,32,32 }, panel_volume);
	speaker_cross->SetPos(speaker_sound->position.x + 10, speaker_cross->position.y);
	speaker_cross->drawable = false;
	clip_volume_level = App->gui->CreateImage(0, 0, { 0,0,149,100 }, panel_volume, false, false, true);
	clip_volume_level->SetPosRespectParent(RIGHT_UP, margin / 2);
	volume_level = App->gui->CreateImage(0, 0, { 739,897,149,100 }, clip_volume_level, true, true, true, true);
	slider_volume = App->gui->CreateSlider(0, 0, { 1571,1798,372,56 }, Slider_TYPE::X, panel_volume);
	slider_volume->SetPosRespectParent(CENTERED_DOWN);
	slider_volume->AddThumb(App->gui->CreateButton(0, 0, { 663,594,40,56 }, slider_volume, { 663,594,40,56 }, { 663,594,40,56 }));
	slider_volume->GetSliderButton()->AddListener(this);
}

void j1Scene::DestroyMenu()
{
	App->gui->DeleteUIElement(panel);
	if (App->GetPause())
		App->Pause();
}

void j1Scene::CheckLevel()
{
	switch (stg)
	{
	case LEVEL_0:
		App->current_lvl = Levels::MENU;
		App->fade->FadeToBlack();
		break;

	case LEVEL_1:
		App->current_lvl = Levels::FIRST_LEVEL;
		
		isLevel1 = true;
		break;
	case LEVEL_2:
		App->current_lvl = Levels::SECOND_LEVEL;
		
		isLevel1 = false;
		break;
	default:
		break;
	}
}

bool j1Scene::Load(pugi::xml_node& data)
{
	bool ret = false;
	App->fade->num_level = data.child("levels").attribute("level").as_int();
	start_time = (data.child("levels").attribute("time").as_int() + SDL_GetTicks()) * 1000;

	if (App->fade->num_level == 1 && !isLevel1) {
		App->scene->stg = LEVEL_1;
		ret = App->fade->SwitchingLevel(App->scene->lvl1.GetString());
	}
	else if (App->fade->num_level == 2 && isLevel1) {
		App->scene->stg = LEVEL_2;
		ret = App->fade->SwitchingLevel(App->scene->lvl2.GetString());
	}
	else ret = true;

	return ret;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node node_stage = data.append_child("levels");

	node_stage.append_attribute("level") = App->fade->num_level;
	node_stage.append_attribute("time") = (SDL_GetTicks() - start_time) / 1000;

	
	return true;
}