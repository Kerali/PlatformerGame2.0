#ifndef __BAT_H__
#define __BAT_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class Bat : Entity
{

public:

	enum class State
	{
		IDLE,
		FLYING
	};

	Bat(fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

private:
	Animation idleAnimation;
	Animation flyingAnimation;
	Animation* currentAnimation;
};

#endif // !__ENTITY_H__
