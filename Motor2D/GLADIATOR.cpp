#include "GLADIATOR.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

Gladiator::Gladiator() : j1Entity(Types::GLADIATOR)
{

	//Load Enemy data
	/*LoadEntityData(tsx.GetString());

	//Load Enemy Anims
	PushBack();

	//Load Enemy sprite
	sprite = App->tex->Load(data.tileset.imagePath.GetString());;

	//Set Gladiator Anims
	gAnim.g_idle = e_anim_idle;
	gAnim.g_walking = e_anim_walking;
	gAnim.g_hit = e_anim_hit;
	gAnim.g_detecting = e_anim_detecting;
	gAnim.g_dead = e_anim_death;

	//Load Collider Size
	gSize.x = data.tileset.tilewidth;
	gSize.y = data.tileset.tileheight;

	//Set Gladiator State
	gState = GladiatorState::G_IDLE;

	//Set Default Anim & Anim Speed
	e_animation = &gAnim.g_idle;
	speedAnim = e_animationSpeed;

	//Set Collider
	collider = App->collider->AddCollider({ 0, 0, gSize.x, gSize.y }, COLLIDER_ENEMY, (j1Module*)App->entities);
	
	//create_dpath = true;
	entityPath = nullptr;

	//original_pos = { (float)x,(float)y };*/

	
	
}

Gladiator::~Gladiator()
{
	
}

void Gladiator::Move(float dt)
{
	/*SetAnimationsSpeed(dt, speedAnim);

	if (!pathfinding)
		DefaultPath(dt);


	StatesMachine();*/
	
}

void Gladiator::Draw(float dt)
{

	/*if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (e_animation != nullptr)
	{
		App->render->Blit(sprite, (int)position.x, (int)position.y, &(e_animation->GetCurrentFrame(1)), 1.0F, flip);
	}*/
	App->render->Blit(data.tileset.texture, position.x, position.y);

}

void Gladiator::DeadAnim()
{
	/*animation = &dead;
	position.y += 0.2f; */
}

void Gladiator::Drop()
{

}

void Gladiator::SetAnimationsSpeed(float dt, float speed) {

	gAnim.g_idle.speed = speed;
	gAnim.g_walking.speed = speed;
	gAnim.g_hit.speed = speed;
	gAnim.g_detecting.speed = speed;
	gAnim.g_dead.speed = speed;

}

void Gladiator::StatesMachine() {

	/*
	switch (gState) {

	case GladiatorState::G_IDLE:
		e_animation = &gAnim.g_idle;

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
		e_animation = &gAnim.g_walking;

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
		
		if (!position.x > lastPosition.x && !position.x < lastPosition.x) 
			gState = GladiatorState::G_IDLE;
			
		break;

	case GladiatorState::G_DETECTING:
		break;
	case GladiatorState::G_HIT:
		break;
	case GladiatorState::G_DEAD:
		break;

	}

	lastPosition = position;*/
}

void Gladiator::DefaultPath(float dt) {
	/*
	if (create_dpath) {
		iPoint to_go;
		to_go.x = (int)position.x - 50;
		to_go.y = (int)position.y;
		
		CreatePathfinding(to_go);
		do_dpath = true;
		create_dpath = false;
	}
	
	if (do_dpath) {
		TrackingPathfinding(dt);
	}*/
}

void Gladiator::CreatePathfinding(iPoint destination) {

	dest = App->pathfinding->CreatePath(App->map->WorldToMap((int)position.x, (int)position.y), App->map->WorldToMap(destination.x, destination.y), TypePathDistance::MANHATTAN);
	entityPath = App->pathfinding->GetLastPath();
	index = 0;

	entityPathSize = entityPath->Count();

	
}

void Gladiator::TrackingPathfinding(float dt) {

	iPoint forwardPos = App->map->MapToWorld(entityPath->At(index)->x, entityPath->At(index)->y);
	fPoint speed = { 2.0f, 1.0f };

	if ((int)position.x < forwardPos.x)
		position.x += speed.x;
	else if ((int)position.x > forwardPos.x)
		position.x -= speed.x;
	if ((int)position.y < forwardPos.y)
		position.y += speed.y;
	else if ((int)position.y > forwardPos.y)
		position.y -= speed.y;

	if ((int)position.x == forwardPos.x) {
		
		if (index < entityPathSize -1 )
			index++;
		
		else {
			index = 0;
			lastPosition = position;
		}
	}
			
}