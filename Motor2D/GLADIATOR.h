#ifndef __GLADIATOR_H__
#define __GLADIATOR_H__

#include "j1Entity.h"
#include "j1Player.h"

struct SDL_Texture;


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

	Gladiator(int x, int y);
	~Gladiator();

	bool Start();
	bool Update(float dt);
	void Move(float dt);
	void OnCollision(Collider*, Collider*, float dt);
	void IdAnimToEnum();
	void DeadAnim();
	void LoadCollider(pugi::xml_node&);
	bool CleanUp();	

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

	GladiatorState gState;
	PathState pState;	


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
	COLLIDER_INFO collider_ground;
	bool grounded = true;

	bool create_back_path = true;
	bool do_back_path = false;

	int dest = 0;
	int index = 0;

	fPoint lastPosition = {128, 256};
	fPoint speed = { 30.0f, 30.0f };
	Collider* playerPathfinding;
	Collider* enemyPathfinding;

	iPoint playerPos;
	
	float cooldown = 0.25f;

public:
	bool stop = false;
	
};

#endif // !__GLADIATOR_H__
