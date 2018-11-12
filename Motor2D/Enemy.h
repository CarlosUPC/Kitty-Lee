#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL_timer.h"

struct SDL_Texture;
struct Collider;

class Enemy
{
protected:
	Animation* animation = nullptr;


public:

	Collider* collider = nullptr;
	fPoint position;


public:
	Enemy(int x, int y, int type);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);

	//virtual void ExtraAnim(SDL_Texture* texture) {};
	//virtual void DeadAnim();
	//virtual void Drop();
};

#endif // __ENEMY_H__
