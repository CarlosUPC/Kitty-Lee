
#include "j1App.h"
#include "Enemy.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "SDL/include/SDL_timer.h"
#include "j1Audio.h"


Enemy::Enemy(int x, int y, p2SString tsx, int type) : position(x, y), enemyTSX(tsx)
{

}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
	{
		App->render->Blit(sprites, position.x, position.y , &(animation->GetCurrentFrame()));
	}

}

void Enemy::OnCollision(Collider* collider)
{


}

/*
void Enemy::DeadAnim()
{
	animation = nullptr;
}


void Enemy::Drop()
{

}
*/