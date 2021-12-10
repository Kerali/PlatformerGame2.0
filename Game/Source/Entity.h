#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SDL_image/include/SDL_image.h"

class Entity
{

public:

	enum class Type
	{
		FRUIT,
		HEART,
		BAT
	};

	Entity(fPoint position, SDL_Texture* texture, Type type) : position(position), texture(texture), type(type)
	{}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	Type type;
	fPoint position;
	SDL_Texture* texture;

	bool pendingToDelete = false;
};

#endif // !__ENTITY_H__
