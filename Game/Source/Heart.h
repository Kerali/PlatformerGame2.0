#ifndef __HEART_H__
#define __HEART_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class Heart : Entity
{

public:

	enum class State
	{
		IDLE,
		DISAPPEAR
	};

	Heart(Module* parent, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* other);

private:
	Animation idleAnimation;
	Animation disappearAnimation;
	Animation* currentAnimation;

	State state;
};

#endif // !__FRUIT_H__
