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
	void Draw();
	void DeadAnim();
	void Drop();

public:

	void SetAnimationsSpeed(float dt, float speed);
	/*Path path;
	Animation idle;
	Animation dead;
	Animation walking;*/
	
private:
	GladiatorAnims gAnim;
	GladiatorState gState;

	float speedAnim;
};

#endif // !__GLADIATOR_H__
