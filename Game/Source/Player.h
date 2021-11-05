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
private:
	PlayerState playerState = PlayerState::IDLE;
	SDL_Texture* texture = nullptr;
	Animation* currentAnim = nullptr;

	bool isGoingRight = false;
	unsigned int jumpsLeft = 2;
	bool isDead;
public:
	Player();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	void OnCollision(Collider* a, Collider* b);

	void UpdateState(float dt);
	void UpdateLogic(float dt);
	void ChangeState(PlayerState previous, PlayerState next);

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
	Animation appearAnim;
	Animation disappearRightAnim;
	Animation disappearLeftAnim;
	Animation doubleJumpRightAnim;
	Animation doubleJumpLeftAnim;

	Point<int> position;

private:
	unsigned int speed;
	float gravity;
	float verticalVelocity = 0.0f;
	float maxVerticalVelocity = 30.0f;
	float jumpForce;

	int maxJumps = 2;
	int availableJumps = 2;

	Collider* collider;

	bool canMoveRight = true;
	bool canMoveLeft = true;
	bool canMoveUp = true;
	bool canMoveDown = true;

	const char* texturePath;
};


#endif
