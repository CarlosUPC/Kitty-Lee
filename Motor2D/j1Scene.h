#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

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

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void CheckLevel();
	

public:
	p2SString lvl1;
	p2SString lvl2;
	Stages stg;
	
private:

	iPoint cameraOffset;
	uint win_width;
	uint win_height;

	bool isLevel1;
	
};

#endif // __j1SCENE_H__