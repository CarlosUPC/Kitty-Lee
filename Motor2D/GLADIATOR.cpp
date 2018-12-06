#include "GLADIATOR.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "p2Point.h"
#include "j1Render.h"
#include "PLAYER.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"

Gladiator::Gladiator(int PositionX, int PositionY) : Enemy(Types::GLADIATOR, PositionX, PositionY)
{
	//Load Enemy data
	LoadEntityData("gladiator.tsx");

	
	state = State::IDLE;
	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;
	speed = { 30.0f,0.0f };

	typepath = TypePathDistance::MANHATTAN;
	
	//Set Collider & Bouncers
	AddColliders();
	player = (Player*)GetEntityPosition(Types::PLAYER);
	enemyPathfinding = App->collider->AddCollider({ (int)position.x,(int)position.y, 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE, this);
	playerPathfinding = App->collider->AddCollider({ (int)player->position.x, (int)player->position.y , 100, 100 }, COLLIDER_TYPE::COLLIDER_NONE);

	//Load Sound Effects
	App->audio->LoadFx(AttackSound);
	
	//Enemy Path
	entityPath.Clear();
	path_state = PathState::DEFAULT_PATH;
	stop = false;
	App->audio->LoadFx(AttackSound);
	
}

Gladiator::~Gladiator()
{
	
}

void Gladiator::OnCollision(Collider* c1, Collider* c2, float dt) {

	if(c1->type == COLLIDER_TYPE::COLLIDER_ENEMY && c2->type == COLLIDER_TYPE::COLLIDER_PLAYER){
		EnemyHit(dt);
		App->audio->PlayFx(4);
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