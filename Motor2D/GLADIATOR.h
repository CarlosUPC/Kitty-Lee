#ifndef __GLADIATOR_H__
#define __GLADIATOR_H__

#include "j1Entity.h"
#include "Path.h"
#include "j1Player.h"

class SDL_Texture;

enum PathState {
	G_DEFAULT_PATH,
	G_CHASE_PATH,
	G_BACK_TO_DEFAULT_PATH
};

enum GladiatorState {
	G_IDLE,
	G_WALKING,
	G_HIT,
	G_DETECTING,
	G_DEAD
};

struct  GladiatorAnims {
	Animation g_idle;
	Animation g_walking;
	Animation g_hit;
	Animation g_detecting;
	Animation g_dead;

};

class Gladiator : public j1Entity
{
public:

	Gladiator();
	~Gladiator();

	bool Start();
	void Move(float dt);
	void Draw(float dt);
	void IdAnimToEnum();
	void DeadAnim();
	void Drop();

private:

	void AddColliders();
	void PushBack();
	void SetAnimationsSpeed(float speed);
	void StatesMachine();
	void DefaultPath(float dt);
	void CreatePathfinding(iPoint destination);
	void TrackingPathfinding(float dt);
	bool DetectPlayer();
	void ChasePlayer(float dt);
	void BackToDefaultPath(float dt);
	
	
private:
	GladiatorState gState;
	PathState pState;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	

	iPoint gSize;

	j1Player* player;

	Animation anim_idle;
	Animation anim_walking;
	Animation anim_hit;
	Animation anim_detecting;
	Animation anim_dead;

	bool pathfinding = false;

	bool create_dpath = true;
	bool do_dpath = false;

	bool create_chase_path = true;
	bool do_chase_path = false;

	bool back = false;

	bool create_back_path = true;
	bool do_back_path = false;

	int dest = 0;
	int index = 0;

	fPoint lastPosition = {128, 256};

	Collider* playerPathfinding;
	Collider* enemyPathfinding;

	iPoint playerPos;
	fPoint initialPos;
	
};

#endif // !__GLADIATOR_H__
