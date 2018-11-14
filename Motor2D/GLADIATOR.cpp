#include "Gladiator.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

Gladiator::Gladiator(int x, int y, p2SString tsx ,int type) : j1Entity(x, y, tsx, type)
{
	
	
	//Load Enemy data
	LoadEnemy(tsx.GetString());

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
	

	//original_pos = { (float)x,(float)y };

	//Set path
	/*path.PushBack({ 0.0f, 0.0f }, 60, &e_anim_idle);
	path.PushBack({ -0.5f, 0.0f }, 140, &e_anim_walking);
	path.PushBack({ 0.0f, 0.0f }, 60, &e_anim_idle);
	path.PushBack({ 0.5f, 0.0f }, 140, &e_anim_walking);*/
	
}

Gladiator::~Gladiator()
{
	
}

void Gladiator::Move(float dt)
{
	SetAnimationsSpeed(dt, speedAnim);

	
	StatesMachine();
	//position = original_pos + path.GetCurrentPosition();
	
	/*fPoint path_pos = path.GetCurrentPosition();
	position.x = float(original_pos.x + path_pos.x);
	position.y = float(original_pos.y + path_pos.y);*/
}

void Gladiator::Draw()
{

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (e_animation != nullptr)
	{
		App->render->Blit(sprite, position.x, position.y, &(e_animation->GetCurrentFrame(1)));
	}

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

	gAnim.g_idle.speed = speed * dt;
	gAnim.g_walking.speed = speed * dt;
	gAnim.g_hit.speed = speed * dt;
	gAnim.g_detecting.speed = speed * dt;
	gAnim.g_dead.speed = speed * dt;

}

void Gladiator::StatesMachine() {

	fPoint lastPosition;

	switch (gState) {

	case GladiatorState::G_IDLE:
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
		e_animation = &gAnim.g_idle;
		break;

	case GladiatorState::G_WALKING:

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
			
		
		e_animation = &gAnim.g_walking;
		break;

	case GladiatorState::G_DETECTING:
		break;
	case GladiatorState::G_HIT:
		break;
	case GladiatorState::G_DEAD:
		break;

	}

	lastPosition = position;
}

void Gladiator::DefaultPath() {

	iPoint to_go;

	if (CreatePathfinding(to_go)) {}
	
}

bool Gladiator::CreatePathfinding(iPoint destination) {

	bool ret = false;

	if (App->pathfinding->CreatePath(App->map->WorldToMap((int)position.x, (int)position.y), App->map->WorldToMap(destination.x, destination.y), TypePathDistance::MANHATTAN)) {}

	return ret;
}