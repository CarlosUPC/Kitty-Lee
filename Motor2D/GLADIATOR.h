#ifndef __GLADIATOR_H__
#define __GLADIATOR_H__

#include "j1Entity.h"
#include "Path.h"

class SDL_Texture;

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

	Gladiator(int x, int y, p2SString tsx, int type);
	~Gladiator();

	void Move(float dt);
	void Draw(float dt);
	void DeadAnim();
	void Drop();

private:

	void SetAnimationsSpeed(float dt, float speed);
	void StatesMachine();
	void DefaultPath(float dt);
	void CreatePathfinding(iPoint destination);
	void TrackingPathfinding(float dt);
	void DetectPlayer();
	
	
private:
	GladiatorAnims gAnim;
	GladiatorState gState;
	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;
	float speedAnim;

	iPoint gSize;


	bool pathfinding = false;
	bool create_dpath = true;
	bool do_dpath = false;

	int dest = 0;
	int index = 0;

	fPoint lastPosition = {128, 256};

	Collider* playerPathfinding;
	Collider* enemyPathfinding;
	
};

#endif // !__GLADIATOR_H__
