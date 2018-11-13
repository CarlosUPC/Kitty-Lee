#include "Gladiator.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"

Gladiator::Gladiator(int x, int y, p2SString tsx ,int type) : j1Entity(x, y, tsx, type)
{
	
	
	//Load Enemy data
	LoadEnemy(tsx.GetString());

	//Load Enemy Anims
	PushBack();

	//Load Enemy sprite
	sprite = App->tex->Load(data.tileset.imagePath.GetString());;


	//Set path
	path.PushBack({ 0.0f, 0.0f }, 60, &e_anim_idle); 
	path.PushBack({ -0.5f, 0.0f }, 140, &e_anim_walking);
	path.PushBack({ 0.0f, 0.0f }, 60, &e_anim_idle);
	path.PushBack({ 0.5f, 0.0f }, 140, &e_anim_walking);
	
	e_animation = &e_anim_idle;
	e_animation->speed = e_animationSpeed;

	collider = App->collider->AddCollider({ 0, 0, 18, 24 }, COLLIDER_ENEMY, (j1Module*)App->entities);
	original_pos = { (float)x,(float)y };

	
}

Gladiator::~Gladiator()
{
	
}

void Gladiator::Move(float dt)
{
	//position = original_pos + path.GetCurrentPosition();
	e_animation->speed = e_animationSpeed;

	fPoint path_pos = path.GetCurrentPosition();
	position.x = float(original_pos.x + path_pos.x);
	position.y = float(original_pos.y + path_pos.y);
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