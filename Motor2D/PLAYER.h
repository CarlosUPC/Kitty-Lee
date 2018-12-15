#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
class Label;


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

class Player : public j1Entity
{	

public:

	Player(int x, int y);

	~Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool GetGodMode() { return god_mode; }

	void SetCoins(const int&);
	void UpdateUI();

	bool Save(pugi::xml_node&) const;
	//bool Load(pugi::xml_node&);

	void OnCollision(Collider*, Collider*, float dt);

private:

	void PushBack();
	void AddColliders();
	void Move(float dt);
	void CheckState();
	void ChangeState();
	void SetCollidersPos();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);
	void LoadCollider(pugi::xml_node&);
	void Draw(float dt);

private:

	PlayerState state = IDLE;

	float		maxSpeedX;
	float		incrementSpeedX;

	float		jumpSpeed;

	bool		air = false;
	bool		platformOverstep = false;
	bool		death = false;

	const char* walkingSound = nullptr;
	const char* jumpingSound = nullptr;
	const char* crashingSound = nullptr;

	bool ghost = false;
	bool god_mode = false;

	int coin_count = 0;

	Label* coin_label = nullptr;
	
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
