#include "GLADIATOR.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "PLAYER.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

Gladiator::Gladiator(int PositionX, int PositionY) : j1Entity(Types::GLADIATOR, PositionX, PositionY)
{
	//Load Enemy data
	LoadEntityData("gladiator2.tsx");

	
	gState = G_IDLE;
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;
	
	
	//Set Collider & Bouncers
	AddColliders();
	player = (j1Player*)GetEntityPosition(Types::PLAYER);
	enemyPathfinding = App->collider->AddCollider({ (int)position.x,(int)position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, this);
	playerPathfinding = App->collider->AddCollider({ (int)player->position.x, (int)player->position.y , 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE);

	
	//Enemy Path
	entityPath = nullptr;
	pState = PathState::G_DEFAULT_PATH;
	stop = false;
	
	
}

bool Gladiator::Start() {
	
	return true;
}
bool Gladiator::Update(float dt)
{
	return true;
}
Gladiator::~Gladiator()
{
	
}

void Gladiator::Move(float dt)
{
	SetAnimationsSpeed(animationSpeed);

	if (!stop) {
		
		if (!grounded) {

			position.y += 5 * dt;
		}

		if (!pathfinding)
			DefaultPath(dt);

		if (DetectPlayer())
			ChasePlayer(dt);

		if (back)
			BackToDefaultPath(dt);
		
		grounded = false;
	}
	StatesMachine();

	current_animation->GetCurrentFrame(dt);

	if (collider.collider != nullptr)
		collider.collider->SetPos((int)position.x + collider.offset.x, (int)position.y + collider.offset.y);
	if (collider_ground.collider != nullptr)
		collider_ground.collider->SetPos((int)position.x + collider_ground.offset.x, (int)position.y + collider_ground.offset.y);

	if (enemyPathfinding != nullptr)
		enemyPathfinding->SetPos((int)position.x - 34, (int)position.y - 34);

	if (playerPathfinding != nullptr)
		playerPathfinding->SetPos((int)player->position.x - 34, (int)player->position.y - 34);
	
	
}

void Gladiator::OnCollision(Collider* c1, Collider* c2, float dt) {

	if(c1->type == COLLIDER_TYPE::COLLIDER_ENEMY && c2->type == COLLIDER_TYPE::COLLIDER_PLAYER){
		EnemyHit(dt);
		stop = true;
	}
	
	if (c1->type == COLLIDER_TYPE::COLLIDER_ENTITY_DOWN && c2->type == COLLIDER_TYPE::COLLIDER_FLOOR) {
		grounded = true;
	}

}		

void Gladiator::IdAnimToEnum()
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

void Gladiator::LoadCollider(pugi::xml_node& node)
{
	p2SString nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "Collider") {
			collider.offset.x = node.attribute("x").as_int();
			collider.offset.y = node.attribute("y").as_int();
			collider.width = node.attribute("width").as_uint();
			collider.height = node.attribute("height").as_uint();
			collider.type = COLLIDER_TYPE::COLLIDER_ENEMY;
		}

		else if (nameIdentificator == "ColliderGround") {
			collider_ground.offset.x = node.attribute("x").as_int();
			collider_ground.offset.y = node.attribute("y").as_int();
			collider_ground.width = node.attribute("width").as_uint();
			collider_ground.height = node.attribute("height").as_uint();
			collider_ground.type = COLLIDER_TYPE::COLLIDER_ENTITY_DOWN;
		}

		node = node.next_sibling();
	}
}

bool Gladiator::CleanUp()
{
	bool ret = false;

	ret = App->tex->UnLoad(data.tileset.texture);
	collider.collider->to_delete = true;
	playerPathfinding->to_delete = true;
	enemyPathfinding->to_delete = true;
	collider_ground.collider->to_delete = true;
	current_animation = nullptr;
	player = nullptr;
	
	return ret;
}

void Gladiator::PushBack()
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

void Gladiator::SetAnimationsSpeed(float speed) {

	anim_idle.speed = speed;
	anim_walking.speed = speed;
	anim_hit.speed = speed;
	anim_detecting.speed = speed;
	anim_dead.speed = speed;
	
}

void Gladiator::AddColliders() {
	SDL_Rect r;
	COLLIDER_INFO* actual_collider; //create a pointer to reduce volum of code in that function

	actual_collider = &collider;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);
	
	actual_collider = &collider_ground;
	r = { (int)position.x + actual_collider->offset.x,	(int)position.y + actual_collider->offset.y, actual_collider->width, actual_collider->height };
	actual_collider->collider = App->collider->AddCollider(r, actual_collider->type, this);
}

void Gladiator::StatesMachine() {

	switch (gState) {

	case GladiatorState::G_IDLE:
		current_animation = &anim_idle;

		if (position.x > lastPosition.x) {
			gState = GladiatorState::G_WALKING;
			flip = SDL_FLIP_HORIZONTAL;
			break;
		}
		if (position.x < lastPosition.x) {
			gState = GladiatorState::G_WALKING;
			flip = SDL_FLIP_NONE;
			break;
		}

		break;

	case GladiatorState::G_WALKING:
		current_animation = &anim_walking;

		if (position.x > lastPosition.x) {
			gState = GladiatorState::G_WALKING;
			flip = SDL_FLIP_HORIZONTAL;
			break;
		}
		if (position.x < lastPosition.x) {
			gState = GladiatorState::G_WALKING;
			flip = SDL_FLIP_NONE;
			break;
		}
		
		if (!(position.x < lastPosition.x) && !(position.x > lastPosition.x)) 
			gState = GladiatorState::G_IDLE;
			
		break;

	case GladiatorState::G_DETECTING:
		current_animation = &anim_detecting;
		break;
	case GladiatorState::G_HIT:
		current_animation = &anim_hit;

		if (anim_hit.Finished()) {
			anim_hit.reset();
		
			cooldown = 3.0F;
			gState = GladiatorState::G_IDLE;
		}

		break;
	case GladiatorState::G_DEAD:
		current_animation = &anim_dead;
		break;
	default:
		gState = GladiatorState::G_IDLE;
		break;
	}
	lastPosition = position;
}

void Gladiator::DefaultPath(float dt) {
	
	if (create_dpath) {

			iPoint to_go;
			to_go.x = (int)spawn_position.x - 50;
			to_go.y = (int)spawn_position.y;
		
			CreatePathfinding(to_go);
			//do_dpath = true;
			create_dpath = false;
	}
	
	if (do_dpath) {
		TrackingPathfinding(dt);
	}
}

void Gladiator::CreatePathfinding(iPoint destination) {
	
	fPoint relativePos = position;

	
	dest = App->pathfinding->CreatePath(App->map->WorldToMap((int)relativePos.x, (int)relativePos.y), App->map->WorldToMap(destination.x, destination.y), TypePathDistance::MANHATTAN);
	
	if (dest > 0) {
		entityPath = App->pathfinding->GetLastPath();
		index = 0;

		entityPathSize = entityPath->Count();

		if (pState == PathState::G_DEFAULT_PATH)
			do_dpath = true;
		else if (pState == PathState::G_CHASE_PATH)
			do_chase_path = true;
		else if (pState == PathState::G_BACK_TO_DEFAULT_PATH)
			do_back_path = true;
	}
	else {

		if (pState == PathState::G_DEFAULT_PATH)
			do_dpath = false;
		else if (pState == PathState::G_CHASE_PATH)
			do_chase_path = false;
		else if (pState == PathState::G_BACK_TO_DEFAULT_PATH)
			do_back_path = false;
	}

}

void Gladiator::TrackingPathfinding(float dt) {

	iPoint forwardPos = App->map->MapToWorld(entityPath->At(index)->x, entityPath->At(index)->y);

	speed = { 30.0F, 0.0F };

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

			if (pState == PathState::G_DEFAULT_PATH) {
				index = 0;

			}

			else if (pState == PathState::G_BACK_TO_DEFAULT_PATH) {
				index = 0;
				pState = PathState::G_DEFAULT_PATH;
				pathfinding = true;
				create_dpath = true;
				back = false;

			}
			
			else if (pState == PathState::G_CHASE_PATH) {

				fPoint fplayerPos;
				fplayerPos.x = (float)playerPos.x;
				fplayerPos.y = (float)playerPos.y;



				if (position.DistanceTo(player->position) <= 50) {
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
		


bool Gladiator::DetectPlayer() {

	bool detected = false;
	SDL_Rect enemy_pos = { (int)position.x, (int)position.y, 100, 100 };
	SDL_Rect player_pos = { (int)player->position.x, (int)player->position.y, 100, 100 };
	
	
	if (SDL_HasIntersection(&enemy_pos, &player_pos) && player->GetGodMode() == false) {

			pathfinding = true;
			detected = true;
			pState = PathState::G_CHASE_PATH;
	}

	else {

		if (pState == PathState::G_CHASE_PATH) {
			back = true;
			create_back_path = true;
			detected = false;
			create_chase_path = true;
			pState = PathState::G_BACK_TO_DEFAULT_PATH;
		}

		 if(pState == PathState::G_DEFAULT_PATH) {
			pathfinding = false;
			detected = false;
		
		}
	}

	return detected;
	
}

void Gladiator::ChasePlayer(float dt) {
	
	
	if (create_chase_path) {
		
		
		playerPos.x = (int)player->position.x;
		playerPos.y = (int)player->position.y;
		if (App->map->WorldToMap(playerPos.x, playerPos.y).y == App->map->WorldToMap(position.x, position.y).y) {
			CreatePathfinding(playerPos);
			create_chase_path = false;
		}
		//do_chase_path = true;

	}

	if (do_chase_path)
		TrackingPathfinding(dt);

}

void Gladiator::BackToDefaultPath(float dt) {


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

void Gladiator::EnemyHit(float dt) {

	gState = GladiatorState::G_IDLE;
	cooldown -= dt;

	if (cooldown <= 0) {
		gState = GladiatorState::G_HIT;
	}
			
}