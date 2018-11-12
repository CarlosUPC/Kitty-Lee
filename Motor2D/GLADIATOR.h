#ifndef __GLADIATOR_H__
#define __GLADIATOR_H__

#include "Enemy.h"
#include "Path.h"

class Gladiator : public Enemy
{
public:

	Gladiator(int x, int y, int type);
	~Gladiator();

	void Move();
	void DeadAnim();
	void Drop();

public:

	Path path;
	Animation idle;
	Animation dead;
	Animation walking;

};

#endif // !__GLADIATOR_H__
