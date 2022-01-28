#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "Module.h"
#include "SDL_image/include/SDL_image.h"
#include "Collisions.h"
#include "App.h"

#include "Log.h"

class Entity
{

public:

	enum class Type
	{
		FRUIT,
		HEART,
		BAT,
		PIG,
		PLAYER
	};

	Entity(Module* parent, fPoint position, SDL_Texture* texture, Type type) : parent(parent), position(position), texture(texture), type(type)
	{}

	~Entity()
	{
		CleanUp();
	}

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

	virtual void Collision(Collider* other, float dt)
	{

	}

	virtual void CleanUp()
	{
		/*collider->pendingToDelete = true;*/
	}

	virtual void Reset()
	{

	}

	Module* parent;

	Type type;
	fPoint position;
	SDL_Texture* texture;

	Collider* collider = nullptr;

	bool pendingToDelete = false;
};

#endif // !__ENTITY_H__
