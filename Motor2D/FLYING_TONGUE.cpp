#include "j1App.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "j1Render.h"
#include "PLAYER.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"
#include "FLYING_TONGUE.h"

FlyingTongue::FlyingTongue(int PositionX, int PositionY) : Enemy(Types::FLYING_TONGUE, PositionX, PositionY)
{
	//Load Enemy data
	LoadEntityData("Flying_Tongue.tsx");
	//LoadProperties();

	state = State::IDLE;
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;
	speed = { 30.0f,30.0f };

	//Set Collider & Bouncers
	AddColliders();

	typepath = TypePathDistance::DISTANCE_TO;

	player = (Player*)GetEntityPosition(Types::PLAYER);
	enemyPathfinding = App->collider->AddCollider({ (int)position.x,(int)position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, this);
	playerPathfinding = App->collider->AddCollider({ (int)player->position.x, (int)player->position.y , 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE);

	//Load Sound Effects
	App->audio->LoadFx(AttackSound);

	//Enemy Path
	entityPath.Clear();
	path_state = PathState::DEFAULT_PATH;
	stop = false;
}


FlyingTongue::~FlyingTongue()
{

}

void FlyingTongue::OnCollision(Collider* c1, Collider* c2, float dt) {

	switch (c2->type) {

	case COLLIDER_PLAYER:
		EnemyHit(dt);
		App->audio->PlayFx(8);
		stop = true;
		break;
	}
}

void FlyingTongue::IdAnimToEnum()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		case 8:
			data.animations[i].animType = EntityState::HIT;
			break;
		case 16:
			data.animations[i].animType = EntityState::DEAD;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}


bool FlyingTongue::CleanUp()
{
	bool ret = false;

	ret = App->tex->UnLoad(data.tileset.texture);
	collider.collider->to_delete = true;
	enemyPathfinding->to_delete = true;
	playerPathfinding->to_delete = true;
	current_animation = nullptr;
	player = nullptr;

	return ret;
}

void FlyingTongue::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function

	actual_collider = &collider;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

}