#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct Animation;
struct Collider;
struct SDL_Texture;

enum PlayerState
{

	IDLE,
	RUNNING,
	JUMPING,  //while jumping aply down force
	DOUBLE_JUMPING,
	FALLING,
	PREPARE_TO_SPAWN,
	SPAWNING,
	DYING
};

class Player : public Module
{
private:
	SDL_Texture* texture = nullptr;
	Animation* currentAnim = nullptr;

	bool isGoingRight = false;
	unsigned int jumpsLeft = 2;

public:
	PlayerState playerState = PlayerState::PREPARE_TO_SPAWN;
	bool isDead;

public:
	Player();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	void OnCollision(Collider* a, Collider* b, float dt);

	void UpdateState(float dt);
	void UpdateLogic(float dt);
	void ChangeState(PlayerState previous, PlayerState next);
	void GodMovement(float dt);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void Reload();

	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation runRightAnim;
	Animation runLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation fallRightAnim;
	Animation fallLeftAnim;
	Animation prepareToSpawnAnim;
	Animation appearAnim;
	Animation disappearRightAnim;
	Animation disappearLeftAnim;
	Animation doubleJumpRightAnim;
	Animation doubleJumpLeftAnim;

	Point<float> position;

	bool godMode = false;
	float gravity;
	float verticalVelocity = 0.0f;

	int health;

	Point<float> initialPosition;

	bool gravityOn = false;

	uint gameStartFx = 0;
	const char* gameStartFxPath;

	Collider* collider;

	float jumpForce;

	uint doubleJumpFx = 0;

private:

	float initialWaitCount = 0.0f;
	float initialWait = 0.3f;

	unsigned int speed;
	float maxVerticalVelocity = 350.0f;

	int maxJumps = 2;
	int availableJumps = 2;

	bool canMoveRight = true;
	bool canMoveLeft = true;
	bool canMoveUp = true;
	bool canMoveDown = true;

	const char* texturePath;

	uint jumpFx = 0;
	uint gameOverFx = 0;
	uint pickUpFruitFx = 0;
	uint checkpointFx = 0;
	uint pickUpHeartFx = 0;

	const char* jumpFxPath;
	const char* doubleJumpFxPath;
	const char* gameOverFxPath;
	const char* pickUpFruitFxPath;
	const char* checkpointFxPath;
	const char* pickUpHeartFxPath;
	
};


#endif
