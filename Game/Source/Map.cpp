#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include <algorithm>

Map::Map() : Module(), mapLoaded(false)
{
	name.create("map");
}

// Destructor
Map::~Map()
{
	for (int i = 0; i < data.tilesets.count(); i++)
	{
		delete data.tilesets[i]->texture;
	}
}

int Properties::GetProperty(const char* name, int defaultValue) const
{
	for (int i = 0; i < list.count(); i++)
	{
		if (list[i]->name == name)
		{
			return list[i]->value;
		}
	}

	return defaultValue;
}

void Properties::SetProperty(const char* name, int value)
{
	for (int i = 0; i < list.count(); i++)
	{
		if (list[i]->name == name)
		{
			list[i]->value = value;
		}
	}
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

bool Map::PostUpdate()
{
	Draw();
	return true;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;
	// L04: TODO 5: Prepare the loop to draw all tilesets + DrawTexture()
	
	// L04: TODO 9: Complete the draw function
	for (int i = 0; i < data.maplayers.count(); i++)
	{
		if (data.maplayers[i]->properties.GetProperty("draw", 1) == 0) continue;
		int layerSize = data.maplayers[i]->width*data.maplayers[i]->height;
		for (int j = 0; j < layerSize; j++)
		{
			uint tileGid = data.maplayers[i]->data[j];
			int layerWidth = data.maplayers[i]->width;

			for (int k = 0; k < data.tilesets.count(); k++)
			{
				TileSet* tileset = data.tilesets[k];

				if (data.tilesets.count() > k + 1 && data.tilesets[k + 1]->firstgid <= tileGid)
				{
					continue;
				}

				int tilesetPosition = tileGid - tileset->firstgid;
				SDL_Rect section = { tilesetPosition % tileset->numTilesWidth*tileset->tileWidth, tilesetPosition/tileset->numTilesWidth*tileset->tileHeight, tileset->tileWidth, tileset->tileHeight };
				app->render->DrawTexture(tileset->texture, j % layerWidth*data.tileWidth, j/layerWidth*data.tileHeight, &section);

				break;
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x*data.tileWidth;
	ret.y = y*data.tileHeight;

	return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
	
	// L04: TODO 7: Get relative Tile rectangle

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets

	data.tilesets.clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers

	data.maplayers.clear();
	data.properties.list.clear();

	app->collisions->CleanUp();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	CleanUp();

	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if(ret == true)
	{
		// L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}
	// L04: TODO 4: Iterate all layers and load each of them
	
	pugi::xml_node layerNode;

	for (layerNode = mapFile.child("map").child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
	{
		MapLayer* layerSet = new MapLayer();

		if (ret == true) ret = LoadLayer(layerNode, layerSet);

		data.maplayers.add(layerSet);
	}

	if(ret == true)
	{
	// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything

		LOG("Successfully parsed map XML file: %s", filename);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);

		for (int i = 0; i < data.tilesets.count(); i++)
		{
			TileSet* t = data.tilesets[i];
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", t->name.GetString(), t->firstgid);
			LOG("tile width: %d tile height: %d", t->tileWidth, t->tileHeight);
			LOG("spacing: %d margin: %d", t->spacing, t->margin);
		}
		// L04: TODO 4: LOG the info for each loaded layer
	}

	CreateColliders();
	app->player->position.x = data.properties.GetProperty("playerX", 0)*data.tileWidth;
	app->player->position.y = data.properties.GetProperty("playerY", 0)*data.tileHeight;

	CreateWalkabilityMap();

	app->pathfinding->SetMap(data.width, data.height, walkabilityMap);
	int p = app->pathfinding->CreatePath(iPoint(20, 61), iPoint(24, 61));
	LOG("%d\n", p);

	mapLoaded = ret;

	return ret;
}

// L03: DONE: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load map general properties
		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);
		data.nextObjectId = map.attribute("nextobjectid").as_int(0);
		data.nextLayerId = map.attribute("nextlayerid").as_int(0);
		SString hexColor = map.attribute("backgroundcolor").as_string("#000000");

		// Cut the string into the respective hex values for each color
		SString red = SString(hexColor).Cut(1, 2);
		SString green = SString(hexColor).Cut(3, 4);
		SString blue = SString(hexColor).Cut(5, 6);

		// Convert base-16 values to bae-10 to get the final color
		data.backgroundColor.r = strtol(red.GetString(), nullptr, 16);
		data.backgroundColor.g = strtol(green.GetString(), nullptr, 16);
		data.backgroundColor.b = strtol(blue.GetString(), nullptr, 16);
		SString orientation = map.attribute("orientation").as_string();

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
		LoadProperties(map.child("properties"), &data.properties);
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	
	// L03: TODO: Load Tileset attributes
	set->name.create(tileset_node.attribute("name").as_string("tileset"));
	set->firstgid = tileset_node.attribute("firstgid").as_int(0);
	set->margin = tileset_node.attribute("margin").as_int(0);
	set->spacing = tileset_node.attribute("spacing").as_int(0);
	set->tileWidth = tileset_node.attribute("tilewidth").as_int(0);
	set->tileHeight = tileset_node.attribute("tileheight").as_int(0);

	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string(""));
		set->texture = app->tex->Load(tmp.GetString());
		set->texWidth = image.attribute("width").as_int(0);
		set->texHeight = image.attribute("height").as_int(0);
		set->numTilesWidth = set->texWidth/set->tileWidth;
		set->numTilesHeight = set->texHeight/set->tileHeight;
		set->offsetX = image.attribute("offsetx").as_int(0);
		set->offsetY = image.attribute("offsety").as_int(0);
	}

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	// L04: TODO 3: Load a single layer

	layer->name.create(node.attribute("name").as_string("Not Found"));
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);
	layer->data = new uint[(data.width*data.height*sizeof(uint))];
	memset(layer->data, 0, size_t(data.width*data.height*sizeof(uint)));

	pugi::xml_node nodeID;

	int counter = 0;
	for (nodeID = node.child("data").child("tile"); nodeID && ret; nodeID = nodeID.next_sibling("tile"))
	{
		if (ret == true) ret = StoreID(nodeID, layer, counter);
		counter++;
	}

	LoadProperties(node.child("properties"), &layer->properties);

	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties* properties)
{
	bool ret = true;

	pugi::xml_node propertyNode;

	for (propertyNode = node.child("property"); propertyNode && ret; propertyNode = propertyNode.next_sibling("property"))
	{
		Properties::Property* prop = new Properties::Property();
		prop->name.create(propertyNode.attribute("name").as_string("Not Found"));
		prop->type.create(propertyNode.attribute("type").as_string("Not Found"));
		prop->value = propertyNode.attribute("value").as_int(0);
		properties->list.add(prop);
	}



	return ret;
}

bool Map::StoreID(pugi::xml_node & node, MapLayer*layer, int ID)
{
	bool ret = true;

	layer->data[ID] = node.attribute("gid").as_uint(0);

	return ret;
}

bool Map::CreateColliders() {
	bool ret = true;

	for (int i = 0; i < data.maplayers.count(); i++) {
		if (data.maplayers[i]->properties.GetProperty("navigation", 0) == 0) continue;
		int layerSize = data.maplayers[i]->width*data.maplayers[i]->height;
		for (int j = 0; j < layerSize; j++) {
			if (data.maplayers[i]->data[j] == 0) continue;
			int layerWidth = data.maplayers[i]->width;
			SDL_Rect section = { j % layerWidth*data.tileWidth, j/layerWidth*data.tileHeight, data.tileWidth, data.tileHeight };
			if (data.maplayers[i]->properties.GetProperty("nextLevel", 0) == 1)
			{
				app->collisions->AddCollider(section, Collider::Type::ENDLEVEL, this);
			}
			else if (data.maplayers[i]->properties.GetProperty("itemType", 0) == 1)
			{
				app->collisions->AddCollider(section, Collider::Type::ITEMHEALTH, this);
			}
			else if (data.maplayers[i]->properties.GetProperty("itemType", 0) == 2)
			{
				app->collisions->AddCollider(section, Collider::Type::ITEMSCORE, this);
			}
			else if (data.maplayers[i]->properties.GetProperty("death", 0) == 1)
			{
				app->collisions->AddCollider(section, Collider::Type::DEATH, this);
			}
			else if (data.maplayers[i]->properties.GetProperty("checkpoint", 0) == 1)
			{
				app->collisions->AddCollider(section, Collider::Type::CHECKPOINT, this);
			}
			else
			{
				app->collisions->AddCollider(section, Collider::Type::STATIC, this);
			}
		}
	}

	return ret;
}

void Map::CreateWalkabilityMap()
{
	walkabilityMap = new uchar[data.width * data.height];
	std::fill_n(walkabilityMap, data.width * data.height, 1);
	List<Collider*>* colliders = &app->collisions->staticColliders;
	for (int i = 0; i < colliders->count(); i++)
	{
		if ((*colliders)[i]->type == Collider::Type::STATIC || (*colliders)[i]->type == Collider::Type::DEATH)
		{
			Point<int> p;
			p.x = (*colliders)[i]->rect.x / data.tileWidth;
			p.y = (*colliders)[i]->rect.y / data.tileHeight;
			walkabilityMap[p.x + data.width * p.y] = 0;
		}
	}
}
