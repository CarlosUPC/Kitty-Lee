#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Window.h"

#include "Brofiler\Brofiler.h"


j1Colliders::j1Colliders() : j1Module()
{
	name.create("colliders");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_FLOOR][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER_DOWN] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER_LEFT] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER_RIGHT] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER_UP] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_DEATH] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_SCENE] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_GHOST] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_WIN] = false;
	
	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SCENE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GHOST] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;

	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_SCENE] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_GHOST] = false;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_WIN] = false;

	matrix[COLLIDER_PLAYER_UP][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_DEATH] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_SCENE] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_GHOST] = false;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_WIN] = false;

	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_DEATH] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_SCENE] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_GHOST] = true;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_WIN] = false;

	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_DEATH] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_SCENE] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_GHOST] = true;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_WIN] = false;

	matrix[COLLIDER_DEATH][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_SCENE] = false;
	matrix[COLLIDER_DEATH][COLLIDER_GHOST] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WIN] = false;

	matrix[COLLIDER_SCENE][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_SCENE][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_SCENE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SCENE][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_SCENE][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_SCENE][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_SCENE][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_SCENE][COLLIDER_DEATH] = false;
	matrix[COLLIDER_SCENE][COLLIDER_SCENE] = false;
	matrix[COLLIDER_SCENE][COLLIDER_GHOST] = false;
	matrix[COLLIDER_SCENE][COLLIDER_WIN] = false;

	matrix[COLLIDER_GHOST][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_GHOST][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_GHOST][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GHOST][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_GHOST][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_GHOST][COLLIDER_PLAYER_LEFT] = true;
	matrix[COLLIDER_GHOST][COLLIDER_PLAYER_RIGHT] = true;
	matrix[COLLIDER_GHOST][COLLIDER_DEATH] = false;
	matrix[COLLIDER_GHOST][COLLIDER_GHOST] = false;
	matrix[COLLIDER_GHOST][COLLIDER_SCENE] = false;
	matrix[COLLIDER_GHOST][COLLIDER_WIN] = false;

	matrix[COLLIDER_WIN][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLATFORM] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_WIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WIN][COLLIDER_GHOST] = false;
	matrix[COLLIDER_WIN][COLLIDER_SCENE] = false;
	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLATFORM] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_UP] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_DOWN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_DEATH] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GHOST] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_SCENE] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_WIN] = false;


}
// Destructor
j1Colliders::~j1Colliders()
{
}

bool j1Colliders::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	debug = node.child("debug").attribute("value").as_bool();
	return ret;
}
bool j1Colliders::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateCollision", Profiler::Color::Yellow);
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
// Called before render is available
bool j1Colliders::Update(float dt)
{
	BROFILER_CATEGORY("UpdateCollision", Profiler::Color::Red);

	Collider* c1;
	Collider* c2;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;
		c1 = colliders[i];
		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;
			c2 = colliders[k];
			if (c1->CheckCollision(c2->rect) == true)
			{
				if (c1->to_delete == false && c2->to_delete != true) {
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);
					if (c1->to_delete == false) {
						if (matrix[c2->type][c1->type] && c2->callback)
							c2->callback->OnCollision(c2, c1);
					}
				}
			}
		}
	}
	DebugDraw();
	return true;
}
void j1Colliders::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		LOG("You can see the colliders");
		debug = !debug;
	}

	if(debug) {
		Uint8 alpha = 80;
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == nullptr)
				continue;
			switch (colliders[i]->type)
			{
			case COLLIDER_NONE: // white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_FLOOR: // red
				App->render->DrawQuad(colliders[i]->rect, 220, 20, 60, alpha);
				break;
			case COLLIDER_PLATFORM: //purple
				App->render->DrawQuad(colliders[i]->rect, 220, 20, 150, alpha);
				break;
			case COLLIDER_SCENE: //blue
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 220, alpha);
				break;
			case COLLIDER_DEATH: //purple
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha);
				break;
			case COLLIDER_GHOST: //green
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 60, alpha);
				break;
			case COLLIDER_WIN: //yellow
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 60, alpha);
				break;
			case COLLIDER_PLAYER: // green
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
				break;
			case COLLIDER_PLAYER_DOWN: // orange
				App->render->DrawQuad(colliders[i]->rect, 255, 144, 0, 150);
				break;
			case COLLIDER_PLAYER_UP: // orange
				App->render->DrawQuad(colliders[i]->rect, 255, 144, 0, 150);
				break;
			case COLLIDER_PLAYER_LEFT: // orange
				App->render->DrawQuad(colliders[i]->rect, 255, 144, 0, 150);
				break;
			case COLLIDER_PLAYER_RIGHT: // orange
				App->render->DrawQuad(colliders[i]->rect, 255, 144, 0, 150);
				break;
			default: //white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			}
		}
		App->render->DrawCircle(App->win->GetWindowWidth()*0.5F, App->win->GetWindowHeight()*0.5F, 3, 255, 0, 0);//Draw a circle on the middle of screen to know where is the center
	}
}
bool j1Colliders::checkColisionList(Collider * enemCollider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;
		if (enemCollider->CheckCollision(colliders[i]->rect))
			return true;
	}
	return false;
}
// Called before quitting
bool j1Colliders::CleanUp()
{
	LOG("Freeing all colliders");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}
bool j1Colliders::EraseAllCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}
	return false;
}

bool j1Colliders::EraseMapCollider()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && (colliders[i]->type <= COLLIDER_SCENE))
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return false;
}
// -----------------------------------------------------
bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (r.x > rect.x + rect.w || r.x + r.w<rect.x || r.y>rect.y + rect.h || r.y + r.h < rect.y)
		return false;
	return true;
}

bool j1Colliders::Check(Collider* c1, COLLIDER_TYPE type) {

	Collider* c2 = nullptr;
	// avoid checking collisions already checked
	for (uint k = 0; k < MAX_COLLIDERS; ++k)
	{
		// skip empty colliders
		if (colliders[k] == nullptr || colliders[k]->type != type)
			continue;
		c2 = colliders[k];
		if (c1->CheckCollision(c2->rect) == true)
		{
			return true;
		}
	}

	return false;
	
}

void j1Colliders::GodMode() {

	//Just a test for God mode debug key
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = god_mode;
	matrix[COLLIDER_PLAYER_DOWN][COLLIDER_DEATH] = god_mode;
	matrix[COLLIDER_PLAYER_UP][COLLIDER_DEATH] = god_mode;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_DEATH] = god_mode;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_DEATH] = god_mode;
	
	god_mode = !god_mode;

}

void j1Colliders::GhostMode(const bool active) {

	//Just a test for God mode debug key
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_GHOST] = active;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_GHOST] = active;

}