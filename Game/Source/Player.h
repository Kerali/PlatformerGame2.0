#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Entity.h"

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

class Player : public Entity
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
	Player(Module* parent, fPoint position, Type type);
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool Draw();
	void Collision(Collider* other, float dt) override;

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

	float jumpForce;

	uint doubleJumpFx = 0;

	bool saveOnce1;
	bool saveOnce2;

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
