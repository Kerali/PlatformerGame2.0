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

	enum class PathfindingState
	{
		NONE,
		MOVING
	};

	enum class KeyInput
	{
		LEFT,
		RIGHT,
		DOWN,
		JUMP,
		COUNT
	};

	Pig(Module* parent, fPoint position, SDL_Texture* texture, Type type, int s, int h);

	bool Start();

	bool Update(float dt);

	void UpdatePathfinding(float dt);

	bool Draw();

	void Collision(Collider* other);

	void CleanUp();

private:
	int GetJumpFrameCount(int deltaY);

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
	PathfindingState pathfindingState;

	DynArray<iPoint> path;
	iPoint lastPlayerPosition;
	int pathIndex = 0;
	bool hasPath = false;

	int speed;
	int health;

	bool lookingRight = false;
	bool inputs[];
};

#endif // !__PIG_H__
