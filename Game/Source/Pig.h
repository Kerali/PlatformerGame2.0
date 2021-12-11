#ifndef __PIG_H__
#define __PIG_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "SDL_image/include/SDL_image.h"

class Pig : Entity
{

public:

	enum class State
	{
		IDLE,
		WALKING,
		HIT,
		LOST,
		DYING
	};

	Pig(Module* parent, fPoint position, SDL_Texture* texture, Type type, int s, int h, int g, int jForce);

	bool Start();

	bool Update(float dt);

	void UpdatePathfinding(float dt);

	bool Draw();

	void Collision(Collider* other);

	void CleanUp();

private:
	int GetJumpFrameCount(int deltaY);

	void SafeMovementX(float deltaX);
	void SafeMovementY(float deltaY);

	float verticalVelocity = 0.0f;

	Animation idleLeftAnimation;
	Animation idleRightAnimation;
	Animation walkingLeftAnimation;
	Animation walkingRightAnimation;
	Animation runningLeftAnimation;
	Animation runningRightAnimation;
	Animation hitLeftAnimation;
	Animation hitRightAnimation;
	Animation questionMarkAnimation;
	Animation deathAnimation;
	Animation* currentAnimation;

	State state;

	DynArray<iPoint> path;
	iPoint lastPlayerPosition;
	iPoint lastPosition;
	int pathIndex = 0;
	bool hasPath = false;

	int speed;
	int health;
	int gravity;
	int jumpForce;

	bool jumping = false;

	bool lookingRight = false;
};

#endif // !__PIG_H__
