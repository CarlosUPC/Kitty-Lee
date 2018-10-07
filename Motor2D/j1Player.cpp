#include "j1App.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"


j1Player::j1Player() {
	name.create("player");
}



j1Player::~j1Player()
{
}

void j1Player::Init()
{
	active = true;
}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& node)
{
	LOG("Loading Player Module");
	
	//idle.PushBack({16,15,32,32});

	return true;
}

// Called before the first frame
bool j1Player::Start()
{

	texture = App->tex->Load("textures/Player.png");

	//This method returns player object's position
	position = App->map->GetInitialPosition();

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	SDL_Rect r = { 16,15,32,32 };
	App->render->Blit(texture, position.x, position.y, &r);
	return true;
}

// Called before quitting
bool j1Player::CleanUp()
{
	App->tex->UnLoad(texture);
	return true;
}

bool j1Player::Load(pugi::xml_node&)
{
	return true;
}

bool j1Player::Save(pugi::xml_node&) const
{
	return true;
}