#ifndef __J1MAINMENU_H__
#define __J1MAINMENU_H__

#include "j1Module.h"
#include "p2DynArray.h"

class Button;
class Label;
class Image;


class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	// Destructor
	virtual ~j1MainMenu();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void UI_Events(UIElement* element, Mouse_Event action);

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;


public:
	uint win_width;
	uint win_height;

private:
	p2SString lvl0;
	Label* title1;
	Label* title2;
	Label* press_space;

	Button* new_game_btn = nullptr;
	Button* continue_btn = nullptr;
	Button* settings_btn = nullptr;
	Button* quit_game_btn = nullptr;
	Button* credits_btn = nullptr;
	Button* github_btn = nullptr;
	Button* website_btn = nullptr;

	p2DynArray<Button*> buttons;
	
	Label* new_game_lbl = nullptr;
	Label* continue_lbl = nullptr;
	Label* credits_lbl = nullptr;
	Label* settings_lbl = nullptr;

	p2DynArray<Label*> labels;

	bool move_camera;
	int camera_limit = 0;
	int camera_step_move = 0;
	int button_limit = 0;
	
};
#endif // __j1MAINMENU_H__
