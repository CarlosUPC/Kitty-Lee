#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"	
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

class j1Player : public j1Module
{	

public:

	enum PlayerState {
		IDLE,
		JUMP,
		FALL,
		LAND,
		RUN
	};

	j1Player();

	~j1Player();

	void Init();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

private:

	PlayerState state = IDLE;

	float		scale = 1.0f;
	iPoint		position;

	pugi::xml_document	player_file;
	SDL_Texture* texture = nullptr;
	
	Animation* current_animation = nullptr;

	Animation idle;
	Animation jump;
	Animation fall;
	Animation land;
	Animation walk;
	
	
};

#endif 
