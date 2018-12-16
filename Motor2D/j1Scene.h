#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "PLAYER.h"

struct SDL_Texture;
class UIImage;
class UILabel;

class Image;
class Label;
class Button;
class Slider;

enum Stages {
	LEVEL_0 = 0,
	LEVEL_1,
	LEVEL_2
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& map);

	// Called before the first frame
	bool Start();

	void SetWalkabilityMap();

	void CreateEntities();
	void CreateEntitiesFromXML(pugi::xml_node & node);
	void ReturnToSpawnPositionEntities();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void CheckLevel();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;
	
	void UI_Events(UIElement* element);

private:
	void CreateMenu();
	void DestroyMenu();

public:
	p2SString lvl1;
	p2SString lvl2;
	Stages stg = LEVEL_1;
	bool isLevel1;

private:

	iPoint cameraOffset;
	uint win_width = 0;
	uint win_height = 0;
	SDL_Texture* debug_tex = nullptr;
	Player* player = nullptr;	

	//ui esc menu objects
	Image* panel = nullptr;

	Button* button_resume = nullptr;
	Label* label_resume = nullptr;

	Button* button_main_menu = nullptr;
	Label* label_main_menu = nullptr;

	Image* panel_save_load = nullptr;
	Button* button_save = nullptr;
	Label* label_save = nullptr;
	Button* button_load = nullptr;
	Label* label_load = nullptr;

	Image* panel_volume = nullptr;
	Slider* slider_volume = nullptr;
	Image* speaker = nullptr;
	Image* speaker_cross = nullptr;
	Image* speaker_sound = nullptr;
	Image* volume_level = nullptr;
	Image* clip_volume_level = nullptr;
	

	Label* label_timer = nullptr;
	p2SString str_time;
	int start_time = 0;
};

#endif // __j1SCENE_H__