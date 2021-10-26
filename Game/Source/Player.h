#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct Animation;
struct Collider;
struct SDL_Texture;

enum PlayerState {

	IDLE,
	RUNNING,
	JUMPING,  //while jumping aply down force
	DOUBLE_JUMPING,
	FALLING,
	DYING
};

class Player : public Module
{
public:
	Point<int> position;
	PlayerState playerState = PlayerState::IDLE;
	SDL_Texture* texture = nullptr;
	Animation* currentAnim = nullptr;

	bool Start();
	void OnCollision(Collider* a, Collider* b);
private:
	unsigned int speed = 10;
};


#endif
