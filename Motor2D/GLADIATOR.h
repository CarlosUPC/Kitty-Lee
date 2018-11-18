#ifndef __GLADIATOR_H__
#define __GLADIATOR_H__

#include "ENEMY.h"
#include "PLAYER.h"

struct SDL_Texture;

class Gladiator : public Enemy
{
public:

	Gladiator(int x, int y);
	~Gladiator();

	void OnCollision(Collider*, Collider*, float dt);
	void IdAnimToEnum();
	void LoadCollider(pugi::xml_node&);
	bool CleanUp();	

private:

	void AddColliders();
	
private:	

	iPoint gSize;

	COLLIDER_INFO collider_ground;
	bool grounded = true;

};

#endif // !__GLADIATOR_H__
