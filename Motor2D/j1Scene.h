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




enum Stages {
	LEVEL_1 = 0,
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
	
	void UI_Events(UIElement* element, Mouse_Event action);

public:
	p2SString lvl1;
	p2SString lvl2;
	Stages stg;
	bool isLevel1;
private:

	iPoint cameraOffset;
	uint win_width;
	uint win_height;
	SDL_Texture* debug_tex;
	Player* player;

	UIImage* banner;
	UILabel* text;
	UIImage* coin;
	
	
	Button* button;

};

#endif // __j1SCENE_H__