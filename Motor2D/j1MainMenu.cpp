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
#include "CheckBox.h"
#include "Label.h"
#include "Slider.h"
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

	move_camera_forward = false;
	move_camera_backward = false;

	camera_step_move = 20;

	button_limit = 403;
	button_origin = 756;

	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;

	//--------------------------------------------MAIN MENU UI----------------------------------------------------//
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
	continue_btn->AddListener(this);
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

	//------------------------------------------------------SETTINGS UI------------------------------------------------------------//
	panel_settings = App->gui->CreateImage(win_width/2 - 350, 0, {1306,1035,703,707}, App->gui->screen, false, false, false);

	back_from_settings_btn = App->gui->CreateButton(10, win_height - 100, { 746,502,57,48 }, App->gui->screen, { 746,502,57,48 }, { 746,502,57,48 });
	back_from_settings_btn->AddListener(this);
	back_from_settings_btn->drawable = false;
	back_from_settings_btn->interactable = false;

	sound_lbl = App->gui->CreateLabel(110, 95, "SOUND", false, false, panel_settings,YELLOW, 60, "fonts/Munro.ttf");
	settings_labels.PushBack(sound_lbl);
	graphics_lbl = App->gui->CreateLabel(110, 395, "GRAPHICS", false, false, panel_settings, YELLOW, 60, "fonts/Munro.ttf");
	settings_labels.PushBack(graphics_lbl);
	volume_lbl = App->gui->CreateLabel(160, 200, "Volume", false, false, panel_settings, WHITE, 40, "fonts/Munro.ttf");
	settings_labels.PushBack(volume_lbl);
	fx_lbl = App->gui->CreateLabel(160, 290, "Fx", false, false, panel_settings, WHITE, 40, "fonts/Munro.ttf");
	settings_labels.PushBack(fx_lbl);
    fps_lbl = App->gui->CreateLabel(160, 500, "Cap to 30 FPS", false, false, panel_settings, WHITE, 40, "fonts/Munro.ttf");
	settings_labels.PushBack(fps_lbl);
	full_screen_lbl = App->gui->CreateLabel(160, 590, "Full Screen", false, false, panel_settings, WHITE, 40, "fonts/Munro.ttf");
	settings_labels.PushBack(full_screen_lbl);

	option_fps = App->gui->CreateCheckBox(450, 490, { 586,747,57,55 }, panel_settings, { 586,747,57,55 }, { 587,818,55,56 });
	option_fps->is_option = true;
	option_fps->draggable = false;
	option_fps->drawable = false;
	option_fps->interactable = false;
	option_fps->AddListener(this);
	option_fps->Clicked();

	option_full_screen = App->gui->CreateCheckBox(450, 580, { 586,747,57,55 }, panel_settings, { 586,747,57,55 }, { 587,818,55,56 });
	option_full_screen->is_option = true;
	option_full_screen->draggable = false;
	option_full_screen->drawable = false;
	option_full_screen->interactable = false;
	option_full_screen->AddListener(this);

	volume_sld = App->gui->CreateSlider(295, 200, { 1571,1798,372,56 }, Slider_TYPE::X, panel_settings);
	volume_sld->AddThumb(App->gui->CreateButton(App->audio->GetVolume() / SDL_MIX_MAXVOLUME * volume_sld->position.w, 0, { 663,594,40,56 }, volume_sld, { 663,594,40,56 }, { 663,594,40,56 }));
	volume_sld->GetSliderButton()->AddListener(this);
	volume_sld->drawable = false;
	volume_sld->interactable = false;
	volume_sld->GetSliderButton()->drawable = false;
	volume_sld->GetSliderButton()->interactable = false;

	fx_sld = App->gui->CreateSlider(295, 290, { 1571,1798,372,56 }, Slider_TYPE::X, panel_settings);
	fx_sld->AddThumb(App->gui->CreateButton(App->audio->GetVolume() / SDL_MIX_MAXVOLUME * fx_sld->position.w, 0, { 663,594,40,56 }, fx_sld, { 663,594,40,56 }, { 663,594,40,56 }));
	fx_sld->GetSliderButton()->AddListener(this);
	fx_sld->drawable = false;
	fx_sld->interactable = false;
	fx_sld->GetSliderButton()->drawable = false;
	fx_sld->GetSliderButton()->interactable = false;
	//-----------------------------------------------------CREDITS UI----------------------------------------------------------------//
	panel_credits = App->gui->CreateImage(win_width / 2 - 280, win_height, { 1075,451,561,556 }, App->gui->screen, false, false, false);

	back_from_credits_btn = App->gui->CreateButton(10, win_height - 100, { 746,502,57,48 }, App->gui->screen, { 746,502,57,48 }, { 746,502,57,48 });
	back_from_credits_btn->AddListener(this);
	back_from_credits_btn->drawable = false;
	back_from_credits_btn->interactable = false;

	panel_credits = App->gui->CreateImage(win_width / 2 - 280, win_height, { 1075,451,561,556 }, App->gui->screen, false, false, false);

	clip_credits = App->gui->CreateImage(10, 10, { 0,0,panel_credits->position.w - 10,panel_credits->position.y - 10 }, panel_credits,false,false,false);
	p2SString license;
	license.create("MIT License\nCopyright(c) 2017\n\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the ''Software''), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : \n\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. \n\nTHE SOFTWARE IS PROVIDED ''AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\nKITTY LEE'S DEVELOPERS:\nCarlos Penya Hernando: https://github.com/CarlosUPC \nChristian Martinez de la Rosa: https://github.com/christt105");

	license_lbl = App->gui->CreateLabel(10,10, license.GetString(), false, false, clip_credits, BLACK, 20, "fonts/Munro.ttf",550U);
	license_lbl->drawable = false;


	//-----------------SET-UP UI-------------------//
	for (int i = 0; i < settings.Count(); i++)
	{
		settings[i]->interactable = false;
		settings[i]->drawable = false;
	}

	for (int i = 0; i < settings_labels.Count(); i++)
	{
		settings_labels[i]->drawable = false;
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

			move_camera_backward = false;


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

			if (move_camera_forward) {
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

			move_camera_forward = false;
			back_from_settings_btn->interactable = true;
			back_from_settings_btn->drawable = true;

			panel_settings->drawable = true;

			option_fps->drawable = true;
			option_fps->interactable = true;

			option_full_screen->drawable = true;
			option_full_screen->interactable = true;

			volume_sld->drawable = true;
			volume_sld->interactable = true;

			volume_sld->GetSliderButton()->drawable = true;
			volume_sld->GetSliderButton()->interactable = true;

			fx_sld->drawable = true;
			fx_sld->interactable = true;

			fx_sld->GetSliderButton()->drawable = true;
			fx_sld->GetSliderButton()->interactable = true;

			for (int i = 0; i < settings.Count(); i++)
			{
				settings[i]->interactable = true;
				settings[i]->drawable = true;

			}
			for (int i = 0; i < settings_labels.Count(); i++)
			{
				settings_labels[i]->drawable = true;
			}

			if (panel_settings->GetLocalPosition().x > 162)
			{
				panel_settings->SetPos(panel_settings->GetLocalPosition().x - camera_step_move*2, panel_settings->GetLocalPosition().y);
			}

			if (panel_settings->GetLocalPosition().y < button_limit/12 && panel_settings->GetLocalPosition().x <= 162)
			{
					panel_settings->SetPos(panel_settings->GetLocalPosition().x, panel_settings->GetLocalPosition().y + camera_step_move);
			}
		}
		//-----------SETTINGS TO MENU---------------//
		else {

			back_from_settings_btn->interactable = false;
			back_from_settings_btn->drawable = false;

			if (move_camera_backward) {
				title1->SetPos(title1->GetLocalPosition().x + camera_step_move, title1->GetLocalPosition().y);
				title2->SetPos(title2->GetLocalPosition().x + camera_step_move, title2->GetLocalPosition().y);
			}

			for (int i = 0; i < settings.Count(); i++)
			{
				settings[i]->interactable = false;
				settings[i]->drawable = false;

			}
			if (panel_settings->GetLocalPosition().y > 0 - panel_settings->position.h)
			{
				panel_settings->SetPos(panel_settings->GetLocalPosition().x + camera_step_move, panel_settings->GetLocalPosition().y - camera_step_move);


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
			license_lbl->drawable = true;

			if (panel_credits->GetLocalPosition().y > button_limit/3)
			{
				panel_credits->SetPos(panel_credits->GetLocalPosition().x, panel_credits->GetLocalPosition().y - camera_step_move);
			}
		}

	}

	//--------CAMERA MOVEMENT-----------//
	if (move_camera_forward)
		App->render->camera.x -= camera_step_move;

	if (move_camera_backward)
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
	press_space = nullptr;

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
	

	back_from_settings_btn = nullptr;
	back_from_credits_btn = nullptr;
	panel_settings = nullptr;
	panel_credits = nullptr;

	sound_lbl = nullptr;
	graphics_lbl = nullptr;
	volume_lbl = nullptr;
	fx_lbl = nullptr;
	fps_lbl = nullptr;
	full_screen_lbl = nullptr;

	option_fps = nullptr;
	option_full_screen = nullptr;

	clip_credits = nullptr;
	license_lbl = nullptr;
	tasks_lbl = nullptr;

	for (int i = 0; i < buttons.Count(); i++)
		buttons[i] = nullptr;

	buttons.Clear();

	for (int i = 0; i < labels.Count(); i++)
		labels[i] = nullptr;

	labels.Clear();

	for (int i = 0; i < settings_labels.Count(); i++)
		settings_labels[i] = nullptr;

	settings_labels.Clear();

	for (int i = 0; i < settings.Count(); i++)
		settings[i] = nullptr;

	settings.Clear();

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

		if (element == (UIElement*)continue_btn)
		{
			//App->current_lvl = Levels::LOADING;
			App->LoadGame();

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
			move_camera_forward = true;
		}

		if (element == (UIElement*)back_from_settings_btn)
		{
			move_camera_backward = true;

		}

		if (element == (UIElement*)back_from_credits_btn)
		{
			move_camera_up = true;

		}

		if (element == (UIElement*)credits_btn)
		{
			move_camera_down = true;

		}

		if (element == (UIElement*)option_fps)
		{
			option_fps->Clicked();
			App->GetFrameRate();
		}

		if (element == (UIElement*)option_full_screen)
		{
			option_full_screen->Clicked();

			if (App->win->fullscreen) {
				App->win->fullscreen = false;
				SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_SHOWN);
				break;
			}
			else {
				App->win->fullscreen = true;
				SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN);
				break;
			}

		}
		break;

	case Mouse_Event::CLICKED_REPEAT:

		if (element == (UIElement*)volume_sld->GetSliderButton()) {

			App->audio->SetVolume(volume_sld->GetSliderValue());
		}

		if (element == (UIElement*)fx_sld->GetSliderButton()) {

			App->audio->SetFx(fx_sld->GetSliderValue());
		}
		break;
	}
				

}

bool j1MainMenu::Load(pugi::xml_node& data)
{
	
	bool ret = true;
	App->fade->num_level = data.child("levels").attribute("level").as_int();
	//App->scene->start_time = (data.child("levels").attribute("time").as_int() + SDL_GetTicks()) * 1000;

	if (App->fade->num_level == 1 && !App->scene->playerOnLvl1) {
		App->scene->stg = LEVEL_2;
		
		ret = App->fade->SwitchingLevel(App->scene->lvl1.GetString());
		
	}
	else if (App->fade->num_level == 2 && App->scene->playerOnLvl1) {
		
		App->scene->stg = LEVEL_1;
		ret = App->fade->SwitchingLevel(App->scene->lvl2.GetString());
	}
	else ret = true;

	return ret;
}

bool j1MainMenu::Save(pugi::xml_node& data) const
{
	bool ret = true;

	pugi::xml_node node_stage = data.append_child("levels");

	node_stage.append_attribute("level") = App->fade->num_level;
	//node_stage.append_attribute("time") = (SDL_GetTicks() - App->scene->start_time) / 1000;
	return ret;
}
