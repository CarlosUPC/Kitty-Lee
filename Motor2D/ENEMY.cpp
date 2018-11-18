#include "ENEMY.h"
#include "PLAYER.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

Enemy::Enemy(j1Entity::Types type, int PositionX, int PositionY) : j1Entity(type, PositionX, PositionY)
{
}

bool Enemy::Update(float dt)
{
	return true;
}

Enemy::~Enemy()
{

}

void Enemy::Move(float dt)
{
	SetAnimationsSpeed(animationSpeed);

	if (!stop) {
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

void Enemy::StatesMachine() {

	switch (state) {

	case State::IDLE:
		current_animation = &anim_idle;

		if (position.x > lastPosition.x) {
			state = State::WALKING;
			flip = SDL_FLIP_HORIZONTAL;
			break;
		}
		if (position.x < lastPosition.x) {
			state = State::WALKING;
			flip = SDL_FLIP_NONE;
			break;
		}

		break;

	case State::WALKING:
		current_animation = &anim_idle;

		if (position.x > lastPosition.x) {
			state = State::WALKING;
			flip = SDL_FLIP_HORIZONTAL;
			break;
		}
		if (position.x < lastPosition.x) {
			state = State::WALKING;
			flip = SDL_FLIP_NONE;
			break;
		}

		if (!(position.x < lastPosition.x) && !(position.x > lastPosition.x))
			state = State::IDLE;

		break;

	case State::HIT:
		current_animation = &anim_hit;

		if (anim_hit.Finished()) {
			anim_hit.reset();

			cooldown = 3.0F;
			state = State::IDLE;
		}

		break;

	case State::DEAD:
		current_animation = &anim_dead;
		break;

	default:
		state = State::IDLE;
		break;
	}
	lastPosition = position;
}

void Enemy::DefaultPath(float dt) {

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

void Enemy::CreatePathfinding(iPoint destination) {


	fPoint relativePos = position;

	dest = App->pathfinding->CreatePath(App->map->WorldToMap((int)relativePos.x, (int)relativePos.y), App->map->WorldToMap(destination.x, destination.y), typepath);

	if (dest > 0) {
		entityPath.Clear();
		entityPathSize = App->pathfinding->GetLastPath()->Count();
		for (uint i = 0; i < entityPathSize; ++i) {
			entityPath.PushBack(*App->pathfinding->GetLastPath()->At(i));
		}
		index = 0;



		if (path_state == PathState::DEFAULT_PATH)
			do_dpath = true;
		else if (path_state == PathState::CHASE_PATH)
			do_chase_path = true;
		else if (path_state == PathState::BACK_TO_DEFAULT_PATH)
			do_back_path = true;
	}
	else {

		if (path_state == PathState::DEFAULT_PATH)
			do_dpath = false;
		else if (path_state == PathState::CHASE_PATH)
			do_chase_path = false;
		else if (path_state == PathState::BACK_TO_DEFAULT_PATH)
			do_back_path = false;
	}

}

void Enemy::TrackingPathfinding(float dt) {

	iPoint forwardPos = App->map->MapToWorld(entityPath.At(index)->x, entityPath.At(index)->y);


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

			if (path_state == PathState::DEFAULT_PATH) {
				index = 0;

			}

			if (path_state == PathState::BACK_TO_DEFAULT_PATH) {
				index = 0;
				path_state = PathState::DEFAULT_PATH;
				pathfinding = true;
				create_dpath = true;
				back = false;

			}

			else if (path_state == PathState::CHASE_PATH) {

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



bool Enemy::DetectPlayer() {

	bool detected = false;
	SDL_Rect enemy_pos = { (int)position.x, (int)position.y, 100, 100 };
	SDL_Rect player_pos = { (int)player->position.x, (int)player->position.y, 100, 100 };


	if (SDL_HasIntersection(&enemy_pos, &player_pos) && player->GetGodMode() == false) {
		pathfinding = true;
		detected = true;
		path_state = PathState::CHASE_PATH;
	}

	else {

		if (path_state == PathState::CHASE_PATH) {
			back = true;
			create_back_path = true;
			detected = false;
			create_chase_path = true;
			path_state = PathState::BACK_TO_DEFAULT_PATH;
		}
		else if (path_state == PathState::DEFAULT_PATH) {
			pathfinding = false;
			detected = false;

		}
	}

	return detected;

}

void Enemy::ChasePlayer(float dt) {


	if (create_chase_path) {


		playerPos.x = (int)player->position.x;
		playerPos.y = (int)player->position.y;
		
		if (type == j1Entity::Types::GLADIATOR) {
			if (App->map->WorldToMap(playerPos.x, playerPos.y).y == App->map->WorldToMap(position.x, position.y).y) {
				CreatePathfinding(playerPos);
				create_chase_path = false;
			}
		}
		else {
			CreatePathfinding(playerPos);
			create_chase_path = false;
			//do_chase_path = true;
		}

	}

	if (do_chase_path)
		TrackingPathfinding(dt);

}

void Enemy::PushBack()
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

void Enemy::SetAnimationsSpeed(float speed) {

	anim_idle.speed = speed;
	anim_walking.speed = speed;
	anim_hit.speed = speed;
	anim_detecting.speed = speed;
	anim_dead.speed = speed;

}

void Enemy::BackToDefaultPath(float dt) {


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

void Enemy::EnemyHit(float dt) {

	state = State::IDLE;
	cooldown -= dt;

	if (cooldown <= 0) {
		state = State::HIT;
	}

}