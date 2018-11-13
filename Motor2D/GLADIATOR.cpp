#include "Gladiator.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Player.h"


Gladiator::Gladiator(int x, int y, p2SString tsx ,int type) : Enemy(x, y, tsx, type)
{
	
	
	//Load Enemy data
	LoadEnemy(tsx.GetString());

	//Load Enemy Anims
	PushBack();


	////Idle
	//idle.PushBack({ 8, 7, 17, 24 });
	//idle.PushBack({ 40, 6, 17, 25 });
	//idle.PushBack({ 72, 5, 17, 26 });
	//idle.PushBack({ 104, 5, 17, 26 });
	//idle.PushBack({ 136, 6, 17, 25 });
	//idle.speed = 0.2f;
	//idle.loop = true;

	////walking
	//walking.PushBack({ 6, 38, 21, 23 });
	//walking.PushBack({ 38, 38, 21, 22 });
	//walking.PushBack({ 70, 38, 21, 23 });
	//walking.PushBack({ 102, 41, 21, 22 });
	//walking.PushBack({ 134, 39, 21, 24 });
	//walking.PushBack({ 166, 39, 21, 23 });
	//walking.PushBack({ 198, 39, 21, 24 });
	//walking.PushBack({ 230, 41, 21, 22 });
	//walking.speed = 0.2f;
	//walking.loop = true;

	//Set path
	path.PushBack({ 0.0f, 0.0f }, 60, &e_anim_idle); 
	path.PushBack({ -0.5f, 0.0f }, 140, &e_anim_walking);
	path.PushBack({ 0.0f, 0.0f }, 60, &e_anim_idle);
	path.PushBack({ 0.5f, 0.0f }, 140, &e_anim_walking);
	
	e_animation = &e_anim_idle;
	e_animation->speed = e_animationSpeed;
	collider = App->collider->AddCollider({ 0, 0, 18, 24 }, COLLIDER_ENEMY, (j1Module*)App->enemies);
	original_pos = { (float)x,(float)y };

	
}

Gladiator::~Gladiator()
{
	
}

void Gladiator::Move()
{
	//position = original_pos + path.GetCurrentPosition();
	e_animation->speed = e_animationSpeed;

	fPoint path_pos = path.GetCurrentPosition();
	position.x = float(original_pos.x + path_pos.x);
	position.y = float(original_pos.y + path_pos.y);
}

void Gladiator::DeadAnim()
{
	/*animation = &dead;
	position.y += 0.2f; */
}

void Gladiator::Drop()
{

}