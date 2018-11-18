#include "j1App.h"
#include "j1Textures.h"
//#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "FLYING_TONGUE.h"

FlyingTongue::FlyingTongue(int PositionX, int PositionY) : j1Entity(Types::FLYING_TONGUE, PositionX, PositionY)
{
	//Load Enemy data
	LoadEntityData("Flying Tongue.tsx");


	fState = F_IDLE;
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;


	//Set Collider & Bouncers
	AddColliders();

	player = (j1Player*)GetEntityPosition(Types::PLAYER);
	enemyPathfinding = App->collider->AddCollider({ (int)position.x,(int)position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, this);
	playerPathfinding = App->collider->AddCollider({ (int)player->position.x, (int)player->position.y , 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE);


	//Enemy Path
	entityPath = nullptr;
	pState = PathStateTongue::F_DEFAULT_PATH;
	stop = false;
}

bool FlyingTongue::Update(float dt)
{
	return true;
}

FlyingTongue::~FlyingTongue()
{

}

void FlyingTongue::Move(float dt)
{
	SetAnimationsSpeed(animationSpeed);

	if(!stop) {
		if (!pathfinding)
			DefaultPath(dt);

		if (DetectPlayer())
			ChasePlayer(dt);

		if (back)
			BackToDefaultPath(dt);

	}

	StatesMachine();

	current_animation->GetCurrentFrame(dt);

	if (collider.collider != nullptr)
		collider.collider->SetPos((int)position.x + collider.offset.x, (int)position.y + collider.offset.y);

	if (enemyPathfinding != nullptr)
		enemyPathfinding->SetPos((int)position.x - 24, (int)position.y - 24);

	if (playerPathfinding != nullptr)
		playerPathfinding->SetPos((int)player->position.x - 34, (int)player->position.y - 34);


}

void FlyingTongue::OnCollision(Collider* c1, Collider* c2, float dt) {

	switch (c2->type) {

	case COLLIDER_PLAYER:
		EnemyHit(dt);
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

void FlyingTongue::PushBack()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::HIT:
				anim_hit.PushBack(data.animations[i].frames[j]);
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
	anim_hit.speed = speed;;
	anim_dead.speed = speed;

}

void FlyingTongue::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function

	actual_collider = &collider;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);

}

void FlyingTongue::StatesMachine() {

	switch (fState) {

	case FlyingTongueState::F_IDLE:
		current_animation = &anim_idle;

		if (position.x > lastPosition.x) {
			fState = FlyingTongueState::F_WALKING;
			flip = SDL_FLIP_NONE;
			break;
		}
		if (position.x < lastPosition.x) {
			fState = FlyingTongueState::F_WALKING;
			flip = SDL_FLIP_HORIZONTAL;
			break;
		}

		break;

	case FlyingTongueState::F_WALKING:
		current_animation = &anim_idle;

		if (position.x > lastPosition.x) {
			fState = FlyingTongueState::F_WALKING;
			flip = SDL_FLIP_NONE;
			break;
		}
		if (position.x < lastPosition.x) {
			fState = FlyingTongueState::F_WALKING;
			flip = SDL_FLIP_HORIZONTAL;
			break;
		}

		if (!(position.x < lastPosition.x) && !(position.x > lastPosition.x))
			fState = FlyingTongueState::F_IDLE;

		break;

	case FlyingTongueState::F_HIT:
		current_animation = &anim_hit;

		if (anim_hit.Finished()) {
			anim_hit.reset();

			cooldown = 3.0F;
			fState = FlyingTongueState::F_IDLE;
		}

		break;

	case FlyingTongueState::F_DEAD:
		current_animation = &anim_dead;
		break;

	default:
		fState = FlyingTongueState::F_IDLE;
		break;
	}
	lastPosition = position;
}

void FlyingTongue::DefaultPath(float dt) {

	if (create_dpath) {

		iPoint to_go;
		to_go.x = (int)position.x - 50;
		to_go.y = (int)position.y;

		CreatePathfinding(to_go);
		//do_dpath = true;
		create_dpath = false;
	}

	if (do_dpath) {
		TrackingPathfinding(dt);
	}
}

void FlyingTongue::CreatePathfinding(iPoint destination) {


	fPoint relativePos = position;

	dest = App->pathfinding->CreatePath(App->map->WorldToMap((int)relativePos.x, (int)relativePos.y), App->map->WorldToMap(destination.x, destination.y), TypePathDistance::MANHATTAN);
	
	if (dest > 0) {
		entityPath = App->pathfinding->GetLastPath();
		index = 0;

		entityPathSize = entityPath->Count();

		if (pState == PathStateTongue::F_DEFAULT_PATH)
			do_dpath = true;
		else if (pState == PathStateTongue::F_CHASE_PATH)
			do_chase_path = true;
		else if (pState == PathStateTongue::F_BACK_TO_DEFAULT_PATH)
			do_back_path = true;
	}
	else {

		if (pState == PathStateTongue::F_DEFAULT_PATH)
			do_dpath = false;
		else if (pState == PathStateTongue::F_CHASE_PATH)
			do_chase_path = false;
		else if (pState == PathStateTongue::F_BACK_TO_DEFAULT_PATH)
			do_back_path = false;
	}

}

void FlyingTongue::TrackingPathfinding(float dt) {



	iPoint forwardPos = App->map->MapToWorld(entityPath->At(index)->x, entityPath->At(index)->y);

	speed = { 30.0f, 30.0f };

	
	if ((int)position.x < forwardPos.x)
		position.x += speed.x * dt;
	else if ((int)position.x > forwardPos.x)
		position.x -= speed.x * dt;
	if ((int)position.y < forwardPos.y)
		position.y += speed.y * dt;
	else if ((int)position.y > forwardPos.y)
		position.y -= speed.y * dt;

	if ((int)position.x == forwardPos.x) {

		if ((uint)index < entityPathSize - 1)
			index++;

		else {

			if (pState == PathStateTongue::F_DEFAULT_PATH) {
				index = 0;

			}

			if (pState == PathStateTongue::F_BACK_TO_DEFAULT_PATH) {
				index = 0;
				pState = PathStateTongue::F_DEFAULT_PATH;
				pathfinding = true;
				create_dpath = true;
				back = false;

			}

			else if (pState == PathStateTongue::F_CHASE_PATH) {

				fPoint fplayerPos;
				fplayerPos.x = (float)playerPos.x;
				fplayerPos.y = (float)playerPos.y;



				if (position.DistanceTo(GetEntityPosition(Types::PLAYER)->position) <= 50) {
					EnemyHit(dt);
				}

				else {
					create_chase_path = true;
					do_chase_path = false;
				}

			}
		}
	}
}



bool FlyingTongue::DetectPlayer() {

	bool detected = false;
	SDL_Rect enemy_pos = { (int)position.x, (int)position.y, 100, 100 };
	SDL_Rect player_pos = { (int)player->position.x, (int)player->position.y, 100, 100 };


	if (SDL_HasIntersection(&enemy_pos, &player_pos) && player->GetGodMode() == false) {
		pathfinding = true;
		detected = true;
		pState = PathStateTongue::F_CHASE_PATH;
	}

	else {

		if (pState == PathStateTongue::F_CHASE_PATH) {
			back = true;
			create_back_path = true;
			detected = false;
			create_chase_path = true;
			pState = PathStateTongue::F_BACK_TO_DEFAULT_PATH;
		}
		else if (pState == PathStateTongue::F_DEFAULT_PATH) {
			pathfinding = false;
			detected = false;

		}
	}

	return detected;

}

void FlyingTongue::ChasePlayer(float dt) {


	if (create_chase_path) {


		playerPos.x = (int)player->position.x;
		playerPos.y = (int)player->position.y;



		CreatePathfinding(playerPos);
		create_chase_path = false;
		//do_chase_path = true;

	}

	if (do_chase_path)
		TrackingPathfinding(dt);

}

void FlyingTongue::BackToDefaultPath(float dt) {


	if (create_back_path) {


		iPoint to_go;
		to_go.x = (int)spawn_position.x;
		to_go.y = (int)spawn_position.y;


		CreatePathfinding(to_go);
		create_back_path = false;
		//do_back_path = true;

	}

	if (do_back_path)
		TrackingPathfinding(dt);

}

void FlyingTongue::EnemyHit(float dt) {

	fState = FlyingTongueState::F_IDLE;
	cooldown -= dt;

	if (cooldown <= 0) {
		fState = FlyingTongueState::F_HIT;
	}

}