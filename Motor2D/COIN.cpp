#include "COIN.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"

Coin::Coin(int x, int y):j1Entity(Types::COIN,x,y){

	LoadEntityData("coin.tsx");
}

Coin::~Coin() {}

bool Coin::Start()
{
	AddColliders(this);
	return true;
}

bool Coin::Update(float dt)
{
	current_animation->GetCurrentFrame(dt);
	return true;
}

bool Coin::CleanUp()
{
	bool ret = false;

	ret = App->tex->UnLoad(data.tileset.texture);
	collider.collider->to_delete = true;

	return ret;
}

void Coin::PushBack()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			anim_idle.PushBack(data.animations[i].frames[j]);
		}
	}
	current_animation = &anim_idle;
	current_animation->speed = 5.0F;
}