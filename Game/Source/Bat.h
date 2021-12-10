#ifndef __BAT_H__
#define __BAT_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "SDL_image/include/SDL_image.h"

class Bat : Entity
{

public:

	enum class State
	{
		IDLE,
		FLYING,
		DYING
	};

	Bat(Module* parent, fPoint position, SDL_Texture* texture, Type type, int s);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* other);

private:
	Animation idleAnimation;
	Animation flyingLeftAnimation;
	Animation flyingRightAnimation;
	Animation questionMarkAnimation;
	Animation deathAnimation;
	Animation* currentAnimation;

	State state;

	iPoint lastPlayerPosition;
	DynArray<iPoint> path;
	int pathIndex = 0;
	int speed;
	bool hasPath = false;
};

#endif // !__ENTITY_H__