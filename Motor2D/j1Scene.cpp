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
	App->map->Load(lvl1.GetString());
	//App->map->Load(App->map->sceneName.GetString());

	App->map->AddCollidersMap();

	SetWalkabilityMap();

	CreateEntities();

	debug_tex = App->tex->Load("maps/path.png");

	/*SDL_Rect r = { 0,0,100,100 };
	banner = App->gui->CreateImage(fPoint(0, 0), r);
	text = App->gui->CreateLabel(fPoint(0, 0), "hey buenas a todos aqui williyrex comentando", 32);
	SDL_Rect c = { 1619,892,22,24 };
	coin = App->gui->CreateImage(fPoint(0, 0), c);*/
	


	button = (Button*)App->gui->CreateButton(0, 0, { 648,173,218,57 });
	button->SetRects({ 648,173,218,57 }, { 6,117,218,57 }, { 417,173,218,57 });
	button->AddListener(this);

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

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
			if ((j1Entity::Types)n.attribute("type").as_int() == j1Entity::Types::PLAYER)
				player = (Player*)ent;
		}
	}
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
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->fade->FadeToBlack();

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
	

	/*if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	bool ret = false;
	LOG("Freeing scene");
	ret = App->tex->UnLoad(debug_tex);

	return true;
}

void j1Scene::UI_Events(UIElement* element, Mouse_Event action) {

	switch (action) {
	case MOUSE_ENTER:
		element->current_state = Mouse_Event::MOUSE_IDLE;
		break;
	case MOUSE_LEAVE:
		element->current_state = Mouse_Event::NONE;
		break;
	case RIGHT_CLICK_DOWN:
		element->current_state = Mouse_Event::NONE;
		break;
	case LEFT_CLICK_DOWN:
		element->current_state = Mouse_Event::LEFT_CLICK_DOWN;
		break;
	case RIGHT_CLICK_UP:
		element->current_state = Mouse_Event::NONE;
		break;
	case LEFT_CLICK_UP:
		element->current_state = Mouse_Event::MOUSE_IDLE;
		break;
	case TAB:
		break;
	case NONE:
		break;
	default:
		break;
	}
}

void j1Scene::CheckLevel()
{
	switch (stg)
	{
	case LEVEL_1:
		App->current_lvl = Levels::FIRST_LEVEL;
		/*App->fade->level1 = true;
		App->fade->level2 = false;*/

		isLevel1 = true;
		break;
	case LEVEL_2:
		App->current_lvl = Levels::SECOND_LEVEL;
		/*App->fade->level2 = true;
		App->fade->level1 = false;*/
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
	pugi::xml_node player_node = data.append_child("levels");

	player_node.append_attribute("level") = App->fade->num_level;
	
	return true;
}