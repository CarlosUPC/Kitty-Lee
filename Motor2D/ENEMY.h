#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "j1Entity.h"
#include "j1Pathfinding.h"
class Player;

enum class PathState {
	DEFAULT_PATH,
	CHASE_PATH,
	BACK_TO_DEFAULT_PATH
};

enum class State {
	IDLE,
	WALKING,
	HIT,
	DETECTING,
	DEAD
};

class Enemy :public j1Entity {
public:
	Enemy(j1Entity::Types type, int x, int y);
	~Enemy();

	bool Update(float dt);
	void Move(float dt);
	//virtual void OnCollision(Collider*, Collider*, float dt);
	//void IdAnimToEnum();
	//virtual bool CleanUp();


public:

	//void AddColliders();
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

public:

	PathState	path_state;
	State		state;

	const char* AttackSound = nullptr;

	Player* player;

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

	fPoint lastPosition;
	fPoint speed;

	iPoint playerPos;

	float cooldown = 0.25f;

	Collider* enemyPathfinding;
	Collider* playerPathfinding;

	p2DynArray<iPoint> entityPath;
	uint entityPathSize = 0;

	Animation anim_idle;
	Animation anim_walking;
	Animation anim_hit;
	Animation anim_detecting;
	Animation anim_dead;

	TypePathDistance typepath = TypePathDistance::MANHATTAN;

public:
	bool stop = false;


};
#endif // !__ENEMY_H__

