#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;

enum PlayerState {
	IDLE = 0,
	WALKING,
	JUMP,
	FALL,
	LAND,
	DEAD,
	HADOUKEN,
	PUNCH,
	IDLE_GHOST,
	WALKING_GHOST,
	JUMP_GHOST,
	FALL_GHOST,
	LAND_GHOST,
	UNKNOWN
};

class j1Player : public j1Entity
{	

public:

	j1Player();

	~j1Player();

	//void Init();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	//bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	bool LoadPlayer(const char*);

	void OnCollision(Collider*, Collider*);

private:

	void PushBack();
	void AddColliders();
	void Movement(float dt);
	void CheckState();
	void ChangeState();
	void SetCollidersPos();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);
	void LoadCollider(pugi::xml_node&);

public:

	PlayerState state = IDLE;

	fPoint		speed;

	float		maxSpeedX;
	float		incrementSpeedX;

	float		jumpSpeed;

	bool		air = false;
	bool		platformOverstep = false;
	bool		death = false;

	const char* walkingSound = nullptr;
	const char* jumpingSound = nullptr;
	const char* crashingSound = nullptr;

	pugi::xml_document	player_file;

	SDL_RendererFlip flip = (SDL_RendererFlip) SDL_FLIP_NONE;

	Animation* current_animation = nullptr;
	float animationSpeed;

	bool ghost = false;

	Animation anim_idle;
	Animation anim_walking;
	Animation anim_jump;
	Animation anim_fall;
	Animation anim_land;
	Animation anim_death;
	Animation anim_idle_ghost;
	Animation anim_walking_ghost;
	Animation anim_jump_ghost;
	Animation anim_fall_ghost;
	Animation anim_land_ghost;
	Animation anim_default;

	COLLIDER_INFO colliderPlayer_down;
	COLLIDER_INFO colliderPlayer_up;
	COLLIDER_INFO colliderPlayer_left;
	COLLIDER_INFO colliderPlayer_right;
};

#endif 
