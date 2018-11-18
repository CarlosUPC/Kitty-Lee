#ifndef __FLYING_TONGUE_H__
#define __FLYING_TONGUE_H__

#include "j1Entity.h"
#include "j1Player.h"
class SDL_Texture;

enum PathStateTongue {
	F_DEFAULT_PATH,
	F_CHASE_PATH,
	F_BACK_TO_DEFAULT_PATH
};

enum FlyingTongueState {
	F_IDLE,
	F_WALKING,
	F_HIT,
	F_DEAD
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
	bool Update(float dt);
	void Move(float dt);
	//void Draw(float dt);
	void IdAnimToEnum();
	void DeadAnim();
	void Drop();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2, float dt) {}

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
	void EnemyHit(float dt);

private:
	FlyingTongueState fState;
	PathStateTongue pState;

	iPoint fSize;

	j1Player* player;

	Animation anim_idle;
	Animation anim_hit;
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

	fPoint lastPosition = { 800, 272 };
	fPoint speed = { 30.0f, 30.0f };

	iPoint playerPos;

	float cooldown = 2.0f;

	Collider* enemyPathfinding;
};

#endif // !__GLADIATOR_H__
