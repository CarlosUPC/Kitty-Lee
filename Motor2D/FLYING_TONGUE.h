#ifndef __FLYING_TONGUE_H__
#define __FLYING_TONGUE_H__

#include "j1Entity.h"
#include "Path.h"

class SDL_Texture;

enum PathState {
	G_DEFAULT_PATH,
	G_CHASE_PATH
};

enum FlyingTongueState {
	G_IDLE,
	G_WALKING,
	G_HIT,
	G_DETECTING,
	G_DEAD
};

struct  FlyingTongueAnims {
	Animation g_idle;
	Animation g_walking;
	Animation g_hit;
	Animation g_detecting;
	Animation g_dead;

};

class FlyingTongue : public j1Entity
{
public:

	FlyingTongue(int x, int y);
	~FlyingTongue();

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


private:
	FlyingTongueState gState;
	PathState pState;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;


	iPoint gSize;

	Animation anim_idle;
	Animation anim_walking;
	Animation anim_hit;
	Animation anim_detecting;
	Animation anim_dead;

	bool pathfinding = false;

	fPoint lastPosition = { 128, 256 };

	Collider* playerPathfinding;
	Collider* enemyPathfinding;

	iPoint playerPos;

};

#endif // !__GLADIATOR_H__

