#include "j1App.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "p2List.h"


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
	bool ret = true;
	LOG("Loading Player Module");
	
	//idle.PushBack({16,15,32,32});
	
	/*pugi::xml_document anim_document;
	pugi::xml_parse_result result = anim_document.load_file("Animations.xml");
	

	pugi::xml_node frames;
	pugi::xml_node anim;

	for (anim = anim_document.child("animations").child("animation"); anim; anim = anim.next_sibling("animation"))
	{
		Animation playerAnim;
		playerAnim.name = anim.attribute("name").as_string();

		for (frames = anim.first_child(); frames; frames = frames.next_sibling("frame")) {
			
			SDL_Rect animCoords{ 
				frames.attribute("x").as_int(), 
				frames.attribute("y").as_int(), 
				frames.attribute("h").as_int(), 
				frames.attribute("h").as_int() 
			};
			
			playerAnim.PushBack({ animCoords.x, animCoords.y, animCoords.w, animCoords.h });
			player_anims.add(&playerAnim);
		
		}

		if (result == NULL) {
			LOG("Could not load player animations xml file %s. pugi error: %s", "animations.xml", result.description());
			ret = false;
		}
	}
*/
	return true;
}

// Called before the first frame
bool j1Player::Start()
{
	bool ret = true;

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

	
	
	//App->render->Blit(texture, position.x, position.y, &frame.rect);
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