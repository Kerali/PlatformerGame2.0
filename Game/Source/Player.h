#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"

struct Animation;
struct Collider;
struct SDL_Texture;
struct Position
{
	float x; float y;
};

enum PLAYER_STATE {

	IDLE,
	RUNNING,
	JUMPING,  //while jumping aply down force
	DOUBLE_JUMPING,
	FALLING,
	DYING
};

class Player : public Module
{
private:
	unsigned int speed = 10;
public:
	Position position;

	bool isGoingRight = true;
	bool jumpDone = false;
	bool doubleJumpDone = false;
	bool isDead = false;

	PLAYER_STATE playerState = PLAYER_STATE::IDLE;

	//Load the spritesheet
	SDL_Texture* texture = nullptr;

	Animation* currentAnim = nullptr;

	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation runnningRightAnim;
	Animation runningLeftAnim;
	Animation dyingAnim;
	Animation shootRightAnim;
	Animation shootLeftAnim;
	Animation dead;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;

	bool Awake();
	void UpdateState();
	void UpdateLogic();
	void ChangeState(PLAYER_STATE previous, PLAYER_STATE next);

};


#endif
