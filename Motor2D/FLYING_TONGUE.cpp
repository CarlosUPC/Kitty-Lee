#include "FLYING_TONGUE.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

FlyingTongue::FlyingTongue() : j1Entity(Types::GLADIATOR)
{
	//Load Enemy data
	LoadEntityData("Flying Tongue.tsx");

	
	gState = G_IDLE;
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;


	//Set Collider & Bouncers
	AddColliders();

	enemyPathfinding = App->collider->AddCollider({ (int)position.x,(int)position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, this);
	playerPathfinding = App->collider->AddCollider({ (int)GetEntityPosition(Types::PLAYER)->position.x, (int)GetEntityPosition(Types::PLAYER)->position.y , 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE);


	//Enemy Path
	entityPath = nullptr;

}

FlyingTongue::~FlyingTongue()
{

}

void FlyingTongue::Move(float dt)
{
	SetAnimationsSpeed(animationSpeed);


}

void FlyingTongue::Draw(float dt)
{

	if (collider.collider != nullptr)
		collider.collider->SetPos(position.x, position.y);

	if (enemyPathfinding != nullptr)
		enemyPathfinding->SetPos((int)position.x - 34, (int)position.y - 34);

	if (playerPathfinding != nullptr)
		playerPathfinding->SetPos(GetEntityPosition(Types::PLAYER)->position.x - 34, GetEntityPosition(Types::PLAYER)->position.y - 34);

	App->render->Blit(data.tileset.texture, position.x, position.y, &current_animation->GetCurrentFrame(dt), 1.0F, flip);



}

void FlyingTongue::IdAnimToEnum()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		case 15:
			data.animations[i].animType = EntityState::WALKING;
			break;
		case 16:
			data.animations[i].animType = EntityState::HIT;
			break;
		case 24:
			data.animations[i].animType = EntityState::DETECTING;
			break;
		case 32:
			data.animations[i].animType = EntityState::DEAD;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}

void FlyingTongue::DeadAnim()
{
	/*animation = &dead;
	position.y += 0.2f; */
}

void FlyingTongue::Drop()
{

}

void FlyingTongue::PushBack()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING:
				anim_walking.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::HIT:
				anim_hit.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::DETECTING:
				anim_detecting.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::DEAD:
				anim_dead.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}
}

void FlyingTongue::SetAnimationsSpeed(float speed) {

	anim_idle.speed = speed;
	anim_walking.speed = speed;
	anim_hit.speed = speed;;
	anim_detecting.speed = speed;
	anim_dead.speed = speed;

}

void FlyingTongue::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function

	actual_collider = &collider;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);
}