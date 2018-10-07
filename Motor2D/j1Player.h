#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"	
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

struct TileSetPlayer {
	SDL_Rect GetTileRect(int id) const;

	p2SString name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	p2SString imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

struct Anim {
	uint id = 0;
	uint num_frames = 0;
	SDL_Rect* frames = nullptr;

	uint FrameCount(pugi::xml_node&);
};

enum PlayerState {
	IDLE,
	JUMP,
	FALL,
	LAND,
	RUN
};

struct PlayerInfo {
	TileSetPlayer tileset; //will only use one for the player
	Anim* animations = nullptr;
	PlayerState state = IDLE;

	uint num_animations = 0;
};

class j1Player : public j1Module
{	

public:

	
	

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

	bool LoadPlayer(const char*);

private:

	PlayerInfo player;

	float		scale = 1.0f;
	iPoint		position;

	pugi::xml_document	player_file;
	
	Animation animation;
	
	//Animation* current_animation = nullptr;

	//Animation jump;
	//Animation fall;
	//Animation land;
	//Animation walk;
	
	
};

#endif 
