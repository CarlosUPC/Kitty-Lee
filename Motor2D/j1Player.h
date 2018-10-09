#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"	
#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"
#include "SDL_image/include/SDL_image.h"

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
	const pugi::char_t*  name;
	uint FrameCount(pugi::xml_node&);
};

enum PlayerState {
	IDLE = 0,
	WALKING_RIGHT = 1,
	WALKING_LEFT = 2,
	JUMP = 3,
	FALL = 4,
	LAND = 5,
	RUN = 6
};

struct PlayerInfo {
	TileSetPlayer tileset; //will only use one for the player
	Anim* animations = nullptr;

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

	void PushBack(const int);

	void Movement();
	void CheckState(fPoint);
	void Actions();

public:

	PlayerInfo player;

	PlayerState state = IDLE;

	fPoint		position;
	fPoint		speed;
<<<<<<< HEAD
	
	float		incrementSpeed = 0.005f;
	float		maxSpeedX = 0.25f;
=======
	float		incrementSpeedX;
	float		gravity;
	float		maxSpeedX;
	float		jump;
>>>>>>> f38e71ebb0b7b7a147ec0c70a0fb5a43df9308be

	bool		air = false;
	bool		land = false;

	

	pugi::xml_document	player_file;

	Animation animation;

	SDL_RendererFlip flip = (SDL_RendererFlip) SDL_FLIP_NONE;

	Animation* current_animation = nullptr;

	Animation* idle = nullptr;
	Animation* walking_right = nullptr;
	Animation* walking_left = nullptr;
	Animation* jumping = nullptr;
	Animation* falling = nullptr;

	Collider* collPlayer;
	iPoint collider;
	iPoint offset;
	

};

#endif 
