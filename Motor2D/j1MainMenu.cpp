#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1MainMenu.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "PLAYER.h"
#include "j1Gui.h"
#include "Image.h"
#include "Button.h"
#include "Label.h"
#include "j1Url.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("menu");
}

// Destructor
j1MainMenu::~j1MainMenu()
{}

// Called before render is available

bool j1MainMenu::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");

	lvl0.create(conf.child("level_0").child_value());
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	bool ret = true;

	App->gui->CreateScreen();

	App->map->Load(lvl0.GetString());

	App->render->CameraInitPos();

	move_camera = false;
	move_camera_back = false;

	camera_step_move = 20;
	
	button_limit = 403;
	button_origin = 756;

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

	title1 = App->gui->CreateLabel(win_width/6, 30, "KITTY", false, false, App->gui->screen, RED, 160, "fonts/04B_30__.ttf");

	title2 = App->gui->CreateLabel(win_width/6 + 100, 180, "LEE", false, false,App->gui->screen, WHITE, 160, "fonts/04B_30__.ttf");
	press_space = App->gui->CreateLabel(win_width / 3 + 30, win_height - 70, "PRESS SPACE TO START", false, false, App->gui->screen, WHITE, 32, "fonts/Munro.ttf");


	new_game_btn = App->gui->CreateButton(win_width / 2 - 80, win_height / 2 - 25 + 250, { 182,148,190,49 }, App->gui->screen, {181,92,190,49 }, { 181,38,190,49 });
	buttons.PushBack(new_game_btn);
	new_game_btn->AddListener(this);
	new_game_lbl = App->gui->CreateLabel(0,0, "PLAY",false,false,new_game_btn, WHITE, 20, "fonts/Munro.ttf");
	labels.PushBack(new_game_lbl);


	credits_btn = App->gui->CreateButton(win_width / 2 - 80, win_height / 2 - 25 + 250 + 225, { 182,148,190,49 }, App->gui->screen, { 181,92,190,49 }, { 181,38,190,49 });
	buttons.PushBack(credits_btn);
	credits_btn->AddListener(this);
	credits_lbl = App->gui->CreateLabel(0, 0, "CREDITS", false, false, credits_btn, WHITE, 20, "fonts/Munro.ttf");
	labels.PushBack(credits_lbl);

	continue_btn = App->gui->CreateButton(win_width / 2 - 80, win_height / 2 - 25 + 50 + 275, { 182,148,190,49 }, App->gui->screen, { 181,92,190,49 }, { 181,38,190,49 });
	buttons.PushBack(continue_btn);
	continue_lbl = App->gui->CreateLabel(0, 0, "CONTINUE", false, false, continue_btn, WHITE, 20, "fonts/Munro.ttf");
	labels.PushBack(continue_lbl);

	settings_btn = App->gui->CreateButton(win_width / 2 - 80, win_height / 2 - 25 + 250 + 150, { 182,148,190,49 }, App->gui->screen, { 181,92,190,49 }, { 181,38,190,49 });
	buttons.PushBack(settings_btn);
	settings_btn->AddListener(this);
	settings_lbl = App->gui->CreateLabel(0, 0, "SETTINGS", false, false, settings_btn, WHITE, 20, "fonts/Munro.ttf");
	labels.PushBack(settings_lbl);


	quit_game_btn = App->gui->CreateButton(10, win_height + 160, { 6,309,49,49 }, App->gui->screen, { 64,309,49,49 }, { 123,309,49,49 });
	buttons.PushBack(quit_game_btn);
	quit_game_btn->AddListener(this);
	github_btn = App->gui->CreateButton(win_width - 60, win_height + 100, { 5, 191, 49, 49 }, App->gui->screen, { 63, 191, 49, 49 }, { 122,191,49,49 });
	github_btn->AddListener(this);
	buttons.PushBack(github_btn);
	website_btn = App->gui->CreateButton(win_width - 60, win_height + 160, { 6,429,50,49 }, App->gui->screen, { 65,429,49,49 }, { 124,432,49,45 });
	website_btn->AddListener(this);
	buttons.PushBack(website_btn);

	back_btn = App->gui->CreateButton(10, win_height - 100, { 746,502,57,48 }, App->gui->screen, { 746,502,57,48 }, { 746,502,57,48 });
	back_btn->AddListener(this);
	back_btn->drawable = false;
	back_btn->interactable = false;

	back_from_credits_btn = App->gui->CreateButton(10, win_height - 100, { 746,502,57,48 }, App->gui->screen, { 746,502,57,48 }, { 746,502,57,48 });
	back_from_credits_btn->AddListener(this);
	back_from_credits_btn->drawable = false;
	back_from_credits_btn->interactable = false;

	panel_img = App->gui->CreateImage(win_width/2 - 350, 0, {1306,1035,703,707}, App->gui->screen, false, false, false);

	panel_credits = App->gui->CreateImage(win_width / 2 - 215, win_height, { 14,498,431,512 }, App->gui->screen, false, false, false);


	for (int i = 0; i < settings.Count(); i++)
	{
		settings[i]->interactable = false;
		settings[i]->drawable = false;
	}

	for (int i = 0; i < buttons.Count(); i++)
	{
		buttons[i]->interactable = false;
		buttons[i]->drawable = false;
	}

	for (int i = 0; i < labels.Count(); i++)
	{
		labels[i]->drawable = false;
		labels[i]->SetPosRespectParent(CENTERED);
	}



	return ret;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		press_space->drawable = false;

	}


	if (!press_space->drawable)
	{

		//-----------CAMERA ON MENU---------------//
		if (App->render->camera.x >= camera_origin) {

			move_camera_back = false;
		

			for (int i = 0; i < buttons.Count(); i++)
				buttons[i]->drawable = true;

			for (int i = 0; i < labels.Count(); i++)
				labels[i]->drawable = true;

			if (new_game_btn->GetLocalPosition().x < 432 && !new_game_btn->interactable) {
				for (int i = 0; i < buttons.Count(); i++)
					buttons[i]->SetPos(buttons[i]->GetLocalPosition().x + camera_step_move, buttons[i]->GetLocalPosition().y);
			}

			if (new_game_btn->GetLocalPosition().y > button_limit && new_game_btn->GetLocalPosition().x >= 432  && !new_game_btn->interactable)
			{
				for (int i = 0; i < buttons.Count(); i++)
					buttons[i]->SetPos(buttons[i]->GetLocalPosition().x, buttons[i]->GetLocalPosition().y - camera_step_move);
			}
		}

		//-----------MENU TO SETTINGS---------------//
		else {


			for (int i = 0; i < buttons.Count(); i++)
			{
				buttons[i]->interactable = false;

			}

			if (move_camera) {
				title1->SetPos(title1->GetLocalPosition().x - camera_step_move, title1->GetLocalPosition().y);
				title2->SetPos(title2->GetLocalPosition().x - camera_step_move, title2->GetLocalPosition().y);
			}

			if (new_game_btn->GetLocalPosition().y < button_origin && !new_game_btn->interactable)
			{
				for (int i = 0; i < buttons.Count(); i++)
					buttons[i]->SetPos(buttons[i]->GetLocalPosition().x - camera_step_move, buttons[i]->GetLocalPosition().y + camera_step_move);
			}


		}

		//-----------CAMERA ON SETTINGS---------------//
		if (-App->render->camera.x + App->win->GetWindowWidth() >= (App->map->data.width-1)*App->map->data.tile_width*App->win->GetScale()) {

			move_camera = false;
			back_btn->interactable = true;
			back_btn->drawable = true;

			panel_img->drawable = true;


			for (int i = 0; i < settings.Count(); i++)
			{
				settings[i]->interactable = true;
				settings[i]->drawable = true;

			}
			if (panel_img->GetLocalPosition().x > 162)
			{
				panel_img->SetPos(panel_img->GetLocalPosition().x - camera_step_move*2, panel_img->GetLocalPosition().y);
			}

			if (panel_img->GetLocalPosition().y < button_limit/12 && panel_img->GetLocalPosition().x <= 162)
			{
					panel_img->SetPos(panel_img->GetLocalPosition().x, panel_img->GetLocalPosition().y + camera_step_move);
			}
		}
		//-----------SETTINGS TO MENU---------------//
		else {

			back_btn->interactable = false;
			back_btn->drawable = false;

			if (move_camera_back) {
				title1->SetPos(title1->GetLocalPosition().x + camera_step_move, title1->GetLocalPosition().y);
				title2->SetPos(title2->GetLocalPosition().x + camera_step_move, title2->GetLocalPosition().y);
			}

			for (int i = 0; i < settings.Count(); i++)
			{
				settings[i]->interactable = false;
				settings[i]->drawable = false;

			}
			if (panel_img->GetLocalPosition().y > 0 - panel_img->position.h)
			{
				panel_img->SetPos(panel_img->GetLocalPosition().x + camera_step_move, panel_img->GetLocalPosition().y - camera_step_move);

				
			}
		}

		if (App->render->camera.y >= camera_origin) {
			move_camera_up = false;
		}

		if (App->render->camera.y < camera_origin && -App->render->camera.y + App->win->GetWindowHeight() < (App->map->data.height - 1)*App->map->data.tile_height*App->win->GetScale()) {
			
			if (move_camera_down) {
				for (int i = 0; i < buttons.Count(); i++)
					buttons[i]->SetPos(buttons[i]->GetLocalPosition().x, buttons[i]->GetLocalPosition().y - camera_step_move);

				title1->SetPos(title1->GetLocalPosition().x, title1->GetLocalPosition().y - camera_step_move);
				title2->SetPos(title2->GetLocalPosition().x, title2->GetLocalPosition().y - camera_step_move);
			}

			if (move_camera_up) {
				back_from_credits_btn->interactable = false;
				back_from_credits_btn->drawable = false;

				for (int i = 0; i < buttons.Count(); i++)
					buttons[i]->SetPos(buttons[i]->GetLocalPosition().x, buttons[i]->GetLocalPosition().y + camera_step_move);

				title1->SetPos(title1->GetLocalPosition().x, title1->GetLocalPosition().y + camera_step_move);
				title2->SetPos(title2->GetLocalPosition().x, title2->GetLocalPosition().y + camera_step_move);

				if (panel_credits->GetLocalPosition().y < win_height)
				{
					panel_credits->SetPos(panel_credits->GetLocalPosition().x, panel_credits->GetLocalPosition().y + camera_step_move);
				}
			}

		}


		if (-App->render->camera.y + App->win->GetWindowHeight() >= (App->map->data.height - 1)*App->map->data.tile_height*App->win->GetScale()) {
			move_camera_down = false;
			back_from_credits_btn->interactable = true;
			back_from_credits_btn->drawable = true;

			panel_credits->drawable = true;

			if (panel_credits->GetLocalPosition().y > button_limit/3)
			{
				panel_credits->SetPos(panel_credits->GetLocalPosition().x, panel_credits->GetLocalPosition().y - camera_step_move);
			}
		}

	}

	//--------CAMERA MOVEMENT-----------//
	if (move_camera)
		App->render->camera.x -= camera_step_move;

	if (move_camera_back)
		App->render->camera.x += camera_step_move;

	if (move_camera_up)
		App->render->camera.y += camera_step_move;

	if (move_camera_down)
		App->render->camera.y -= camera_step_move;

	if (new_game_btn->GetLocalPosition().y <= button_limit)
	{
		for (int i = 0; i < buttons.Count(); i++)
			buttons[i]->interactable = true;
	}



	App->map->Draw();
	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing main_menu");

	App->gui->DeleteAllUIElements();

	title1 = nullptr;
	title2 = nullptr;

	new_game_btn = nullptr;
	continue_btn = nullptr;
	settings_btn = nullptr;
	quit_game_btn = nullptr;
	credits_btn = nullptr;
	github_btn = nullptr;
	website_btn = nullptr;

	new_game_lbl = nullptr;
	continue_lbl = nullptr;
	credits_lbl = nullptr;
	settings_lbl = nullptr;
	press_space = nullptr;

	for (int i = 0; i < buttons.Count(); i++)
		buttons[i] = nullptr;

	buttons.Clear();

	for (int i = 0; i < labels.Count(); i++)
		labels[i] = nullptr;

	labels.Clear();

	return true;
}

void j1MainMenu::UI_Events(UIElement* element) {

	switch (element->current_state) {

	case Mouse_Event::CLICKED_DOWN:

		if (element == (UIElement*)new_game_btn)
		{
			App->current_lvl = Levels::FIRST_LEVEL;
			App->fade->FadeToBlack();
		}

		if (element == (UIElement*)github_btn)
		{
			open_url("https://github.com/CarlosUPC/Kitty-Lee");
		}

		if (element == (UIElement*)website_btn)
		{
			open_url("https://carlosupc.github.io/Kitty-Lee/");
		}

		if (element == (UIElement*)quit_game_btn)
		{
			App->quit_game = true;
		}

		if (element == (UIElement*)settings_btn)
		{
			move_camera = true;
		}

		if (element == (UIElement*)back_btn)
		{
			move_camera_back = true;

		}

		if (element == (UIElement*)back_from_credits_btn)
		{
			move_camera_up = true;

		}

		if (element == (UIElement*)credits_btn)
		{
			move_camera_down = true;

		}
		break;
	}

}

bool j1MainMenu::Load(pugi::xml_node& load)
{
	bool ret = true;



	return ret;
}

bool j1MainMenu::Save(pugi::xml_node& save) const
{
	bool ret = true;


	return ret;
}
