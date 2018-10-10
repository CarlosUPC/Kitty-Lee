#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	p2List_item<TileSet*>* item = nullptr;
	p2List_item<MapLayer*>* item_layer = nullptr;
	uint id = 0;
	float velocity = 1.0f;
	for (item_layer = data.layers.start; item_layer; item_layer = item_layer->next) {
		if (item_layer->data->name.GetString()[0] == 'b')
			velocity = 0.7f;
		else velocity = 1.0f;
		if (item_layer->data->visible)
			for (item = data.tilesets.start; item; item = item->next) {
				for (uint i = 0; i < item_layer->data->height; i++) {

					for (uint j = 0; j < item_layer->data->width; j++) {
						id = item_layer->data->tiles[item_layer->data->Get(j, i)];
						if (id != 0)
							App->render->Blit(item->data->texture, MapToWorld(j, i).x, MapToWorld(j, i).y, &item->data->GetTileRect(id), velocity);
					}
				}
			}
	}
}

void j1Map::ColliderPrint()
{
	

	
	uint counterWidth = 1;
	uint counterHeight = 1;
	uint aux_width = 16;
	uint aux_height = 16;

	p2List_item<TileSet*>* item = nullptr;
	p2List_item<MapLayer*>* item_layer = nullptr;
	p2List_item<ColliderObject*>* item_coll = nullptr;
	
	for (item_layer = data.layers.start; item_layer; item_layer = item_layer->next)
	{

		for (item = data.tilesets.start; item; item = item->next) {
			
			for (uint i = 0; i < item_layer->data->height; i++)
			{
				for (uint j = 0; j < item_layer->data->width; j++)
				{
					uint id = item_layer->data->tiles[item_layer->data->Get(j, i)];

					if (id != 0)
					{
						
							for (item_coll = data.colliders.start; item_coll; item_coll = item_coll->next)
							{
								
								int WorldX = item_coll->data->coll_x;
								int WorldY = item_coll->data->coll_y;

								int WidthColl= item_coll->data->coll_width;
								int HeightColl = item_coll->data->coll_height;

								while(!(WidthColl <= aux_width)) {
									aux_width += 16;
									counterWidth++;
								}

								while (!(HeightColl <= aux_height)) {
									aux_height += 16;
									counterHeight++;
								}

								int x = MapToWorld(j, i).x;
								int y = MapToWorld(j, i).y;
								
								if(x == WorldX && y == WorldY){
							
									SDL_Rect collider_rec = { x,y,data.tile_width*(counterWidth),data.tile_height*(counterHeight) };
									App->collider->AddCollider(collider_rec, item_coll->data->type);
								}
								counterWidth = 1;
								aux_width = 16;

								counterHeight = 1;
								aux_height = 16;
							
						}
					}
				}

				
			}
		}
	}
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);


	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = int (data.tile_width * 0.5f) * (x - y);
		ret.y = int (data.tile_height * 0.5f) * (x + y);
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const {
	iPoint ret(0, 0);

	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = x / data.tile_width + y / data.tile_height;
		ret.y = 2 * y / data.tile_height - ret.x;
		ret.x--; ret.y--;
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}


inline uint MapLayer::Get(int x, int y) const {
	return y * this->width + x;
}

inline uint ColliderObject::Get(int x, int y) const {
	return y * this->coll_width + x;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;
	
	while (item2 != NULL) {
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Remove all colliders
	p2List_item<ColliderObject*>* item3;
	item3 = data.colliders.start;

	while (item3!= NULL) {
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.colliders.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer")) {

		MapLayer* lay = new MapLayer();

		if (ret == true)
			ret = LoadLayer(layer, lay);

		data.layers.add(lay);
	}

	

	//Load Object data ------------------------------------------------
	pugi::xml_node objectGroup;
	pugi::xml_node object;
	
	for (objectGroup = map_file.child("map").child("objectgroup"); objectGroup && ret; objectGroup = objectGroup.next_sibling("objectgroup"))
	{
		for (object = objectGroup.first_child(); object;object = object.next_sibling("object")) {

			ColliderObject* obj = new ColliderObject();
			

			if (ret == true && object != NULL) 
				ret = LoadObject(object, obj);
				
			

			data.colliders.add(obj);
		}
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ColliderObject*>* item_object = data.colliders.start;
		while (item_object != NULL) {
			ColliderObject* o = item_object->data;
			LOG("Object ------");
			LOG("name: %s", o->name.GetString());
			LOG("Player Position: (%i , %i)", o->initialPosition.x, o->initialPosition.y);
			LOG("Collider Position: (%i , %i)", o->coll_x, o->coll_y);
			LOG("width: %i  height: %i", o->coll_width, o->coll_height);
			item_object = item_object->next;
		}

	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red;
			p2SString green;
			p2SString blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();


	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	if (node.empty())	ret = false;

	layer->name.create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->visible = node.attribute("visible").as_bool(true);
	
	layer->tiles = new uint[layer->width*layer->height];

	memset(layer->tiles, 0, layer->width*layer->height * sizeof(uint));

	pugi::xml_node tile = node.child("data").child("tile");

	for (uint i = 0; i < layer->width*layer->height; i++) {
		layer->tiles[i] = tile.attribute("gid").as_uint();
		tile = tile.next_sibling("tile");
	}

	return ret;
}

bool j1Map::LoadObject(pugi::xml_node& node_object, ColliderObject* obj) {

	bool ret = true;
	if (node_object.empty())	ret = false;

	obj->name = node_object.attribute("name").as_string();
	obj->tile_id = node_object.attribute("id").as_uint();
	obj->coll_x = node_object.attribute("x").as_int();
	obj->coll_y = node_object.attribute("y").as_int();
	obj->coll_height = node_object.attribute("height").as_uint();
	obj->coll_width = node_object.attribute("width").as_uint();

	pugi::xml_node objGroup = node_object.parent();
	p2SString type(objGroup.child("properties").child("property").attribute("value").as_string());

	if (type == "COLLIDER_NONE")
	{
		obj->type = COLLIDER_NONE;
	}
	else if (type == "COLLIDER_WALL")
	{
		obj->type = COLLIDER_WALL;
	}
	else if (type == "COLLIDER_FLOOR")
	{
		obj->type = COLLIDER_FLOOR;
	}

	//Take initial position of player
	// if-condition doesnt work, i dont know why but xml doesnt detect object child name "Player" 
	// i commented the condition to run the code in right way at the moment because we have only one object xD

	//Solved if-condition problem in this way, but i would like to improve the code
	if (node_object.attribute("name").as_string()[0] == 'P') {
			obj->name = node_object.attribute("name").as_string();
			obj->initialPosition.x = node_object.attribute("x").as_int();
			obj->initialPosition.y = node_object.attribute("y").as_int();	
			obj->coll_width = node_object.attribute("width").as_uint();
			obj->coll_height = node_object.attribute("height").as_uint();
		}


	return ret;
}

fPoint j1Map::GetInitialPosition() const {

	fPoint initialPos;
	p2List_item<ColliderObject*>* ente = data.colliders.start;

	while (ente != NULL)
	{

		if (ente->data->name == "Player")
			initialPos = ente->data->initialPosition;
		
	
		ente = ente->next;
	}

	return initialPos;

}

