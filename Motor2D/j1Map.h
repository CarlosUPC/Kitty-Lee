#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"


// ----------------------------------------------------

struct ColliderObject {

	p2SString name;
	fPoint initialPosition;

	COLLIDER_TYPE type;
	uint tile_id;
	int	coll_x;
	int	coll_y;
	uint coll_width = 0;
	uint coll_height = 0;	

};


struct MapLayer {
	p2SString name;
	uint width = 0;
	uint height = 0;
	uint* tiles = nullptr;
	float speed = 1.0f;
	bool visible = true;

	~MapLayer() { delete[] tiles; tiles = nullptr; }

	inline uint Get(int x, int y) const;
};
	
// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	//p2List<ColliderObject*>	colliders;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<ColliderObject*>	colliders;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	
	fPoint GetInitialPosition() const;
	
	const char* getTypeCollider(enum COLLIDER_TYPE);

	void ColliderPrint();
	

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObject(pugi::xml_node& node, ColliderObject* obj);

public:

	MapData data;
	
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	
};

#endif // __j1MAP_H__