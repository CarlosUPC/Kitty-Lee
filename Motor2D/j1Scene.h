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
	void SwitchingLevel();

private:
	p2SString lvl1;
	p2SString lvl2;

	iPoint cameraOffset;
	uint win_width;
<<<<<<< HEAD
	uint win_height;

	int camPos;
	
	//STAGES stage = LEVEL_1;
	bool level1;
	bool level2;

public:
	Stages stg;
=======
	uint win_height;	
	
>>>>>>> 5ac7332694828e21c4d2b291a7ded9dea5fb69f7
};

#endif // __j1SCENE_H__