#ifndef __FRUIT_H__
#define __FRUIT_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class Fruit : Entity
{

public:

	enum class State
	{
		IDLE,
		DISAPPEAR
	};

	Fruit(Module* parent, fPoint position, SDL_Texture* texture, Type type, int r);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* other, float dt) override;

private:
	Animation idleAnimation;
	Animation disappearAnimation;
	Animation* currentAnimation;

	State state;
};

#endif // !__FRUIT_H__
