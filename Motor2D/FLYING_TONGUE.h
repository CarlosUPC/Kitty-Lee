#ifndef __FLYING_TONGUE_H__
#define __FLYING_TONGUE_H__

#include "ENEMY.h"
#include "PLAYER.h"
class SDL_Texture;

class FlyingTongue : public Enemy
{
public:

	FlyingTongue(int x, int y);
	~FlyingTongue();
	void OnCollision(Collider*, Collider*, float dt);
	void IdAnimToEnum();
	bool CleanUp();
	

private:

	void AddColliders();

private:

	iPoint fSize;

};

#endif // !__GLADIATOR_H__
