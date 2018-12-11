#include "COIN.h"

Coin::Coin(int x, int y):j1Entity(Types::COIN,x,y){

	LoadEntityData("coin.tsx");

	AddColliders();
}

Coin::~Coin() {}

bool Coin::Update(float dt)
{
	current_animation->GetCurrentFrame(dt);
	return true;
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