#include "Player.h"

#include "App.h"
#include "Input.h"
#include "Animation.h"
#include "Textures.h"
#include "SDL/include/SDL_scancode.h"
#include "Collisions.h"
#include "Render.h"

#include "../Defs.h"
#include "../Log.h"

bool Player::Start()
{
	bool ret = true;

	position.x = 30;
	position.y = 840;

	LOG("Loading Player textures");

	//texture = app->tex->Load("SpriteSheet.png");

	//texture = app->tex->Load("Assets/textures/Main Characters/Virtual Guy/Idle (32x32).png");
	texture = app->tex->Load("Assets/textures/Main Characters/Virtual Guy/SpriteSheet.png");

	if (texture == nullptr)
		LOG("Couldn't load player texture");

	currentAnim = &idleRightAnim;

	app->collisions->AddCollider(SDL_Rect({ 15, 303, 16, 16 }), Collider::Type::DYNAMIC, this);

	idleRightAnim.loop = idleLeftAnim.loop = runRightAnim.loop = runLeftAnim.loop = true;
	idleRightAnim.speed = idleLeftAnim.speed = 0.8f;
	runRightAnim.speed = runLeftAnim.speed = 2.0f;

	idleRightAnim.PushBack({ 0,0,22,26 });
	idleRightAnim.PushBack({ 32,0,22,26 });
	idleRightAnim.PushBack({ 64,0,22,26 });
	idleRightAnim.PushBack({ 96,0,22,26 });
	idleRightAnim.PushBack({ 128,0,22,26 });
	idleRightAnim.PushBack({ 160,0,22,26 });
	idleRightAnim.PushBack({ 192,0,22,26 });
	idleRightAnim.PushBack({ 224,0,22,26 });
	idleRightAnim.PushBack({ 256,0,22,26 });
	idleRightAnim.PushBack({ 288,0,22,26 });
	idleRightAnim.PushBack({ 320,0,22,26 });

	idleLeftAnim.PushBack({ 0,26,22,26 });
	idleLeftAnim.PushBack({ 32,26,22,26 });
	idleLeftAnim.PushBack({ 64,26,22,26 });
	idleLeftAnim.PushBack({ 96,26,22,26 });
	idleLeftAnim.PushBack({ 128,26,22,26 });
	idleLeftAnim.PushBack({ 160,26,26,26 });
	idleLeftAnim.PushBack({ 192,26,27,26 });
	idleLeftAnim.PushBack({ 224,26,27,26 });
	idleLeftAnim.PushBack({ 256,26,28,26 });
	idleLeftAnim.PushBack({ 288,26,28,26 });
	idleLeftAnim.PushBack({ 320,26,28,26 });

	runRightAnim.PushBack({ 0,62,24,28 });
	runRightAnim.PushBack({ 32,62,24,28 });
	runRightAnim.PushBack({ 64,62,24,28 });
	runRightAnim.PushBack({ 96,62,24,28 });
	runRightAnim.PushBack({ 128,62,24,28 });
	runRightAnim.PushBack({ 160,62,24,28 });
	runRightAnim.PushBack({ 192,62,24,28 });
	runRightAnim.PushBack({ 224,62,24,28 });
	runRightAnim.PushBack({ 256,62,24,28 });
	runRightAnim.PushBack({ 288,62,24,28 });
	runRightAnim.PushBack({ 320,62,24,28 });
	runRightAnim.PushBack({ 352,62,24,28 });

	runLeftAnim.PushBack({ 0,90,24,28 });
	runLeftAnim.PushBack({ 32,90,24,28 });
	runLeftAnim.PushBack({ 64,90,24,28 });
	runLeftAnim.PushBack({ 96,90,24,28 });
	runLeftAnim.PushBack({ 128,90,24,28 });
	runLeftAnim.PushBack({ 160,90,24,28 });
	runLeftAnim.PushBack({ 192,90,24,28 });
	runLeftAnim.PushBack({ 224,90,24,28 });
	runLeftAnim.PushBack({ 256,90,24,28 });
	runLeftAnim.PushBack({ 288,90,24,28 });
	runLeftAnim.PushBack({ 320,90,24,28 });
	runLeftAnim.PushBack({ 352,90,24,28 });

	jumpRightAnim.PushBack({ 0,188,22,28 });

	jumpLeftAnim.PushBack({ 68,188,22,28 });

	fallRightAnim.PushBack({ 22,188,23,28 });

	fallLeftAnim.PushBack({ 45,188,23,28 });

	appearAnim.loop = disappearLeftAnim.loop = disappearRightAnim.loop = false;

	for (int i = 0; i < 390; i += 55)
	{
		appearAnim.PushBack({ i,227,55,55 });

	}

	for (int i = 0; i < 390; i += 55)
	{
		disappearLeftAnim.PushBack({ i,282,55,55 });

	}

	return ret;
}

bool Player::Update(float dt)
{
	currentAnim->Update();

	return true;
}

bool Player::PostUpdate()
{

	SDL_Rect rect = currentAnim->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect);

	app->player->UpdateState();
	app->player->UpdateLogic();


	return true;
}

void Player::OnCollision(Collider* a, Collider* b) {
	LOG("Detected collision");
}

//bool Player::Awake()
//{
//	bool ret = true;
//	position.x = 30;
//	position.y = 30;
//
//	LOG("Loading Player textures");
//
//	texture = app->tex->Load("Assets/SpriteSheet.png");
//	currentAnim = &idleRightAnim;
//
//	if (texture == nullptr)
//	{
//		LOG("Could't load player textures");
//		ret = false;
//	}
//
//	return ret;
//}
//
void Player::UpdateState()
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		isGoingRight = false;
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		isGoingRight = true;

	switch (playerState)
	{
	case IDLE:
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				ChangeState(playerState, RUNNING);
			}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(playerState, JUMPING);
		}

		if (isDead == true) ChangeState(playerState, DYING);

		break;
	}
	case RUNNING:
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)

		{
			ChangeState(playerState, IDLE);

		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ChangeState(playerState, JUMPING);
		}

		break;
	}
	case JUMPING:
	{
		//once animation is done change to falling
		// or simply add the falling sprite on jumping animations
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
			)
		{
			ChangeState(playerState, DOUBLE_JUMPING);
		}
		break;
	}
	case DOUBLE_JUMPING:
	{
		break;
	}

	case FALLING:
	{
		break;
	}
	case DYING:
	{
		break;
	}

	}

}

void Player::UpdateLogic()
{
	if (position.y < 130)
	{
		position.y += gravityForce;
		//jumpForce = 0;
	}

	position.y -= jumpForce;

	switch (playerState)
	{
	case(IDLE):
	{
		if (isGoingRight == true)
			currentAnim = &idleRightAnim;
		else
			currentAnim = &idleLeftAnim;

		break;
	}
	case(RUNNING):
	{
		if (isGoingRight == true)
		{
			currentAnim = &runRightAnim;
			position.x += speed;
		}
		else
		{
			currentAnim = &runLeftAnim;
			position.x -= speed;
		}

		break;
	}
	case(JUMPING):
	{
		jumpForce = jumpForceValue;
		jumpsLeft--;

		if (isGoingRight == true)
			currentAnim = &jumpRightAnim;
		else
			currentAnim = &jumpLeftAnim;

		ChangeState(JUMPING, FALLING);

		break;
	}
	case(FALLING):
	{
		jumpForce = 0;

		if (isGoingRight == true)
			currentAnim = &fallRightAnim;
		else
			currentAnim = &fallLeftAnim;

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += speed;

		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= speed;

		}

		ChangeState(FALLING, IDLE);
	}
	case(DOUBLE_JUMPING):
	{
		jumpForce = jumpForceValue;

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += speed;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= speed;
		}

		break;
	}
	case(DYING):
	{

		if (isGoingRight == true)
			currentAnim = &disappearRightAnim;
		else
			currentAnim = &disappearLeftAnim;

		break;

	}

	}
}



void Player::ChangeState(PlayerState previousState, PlayerState newState)
{
	switch (newState)
	{
	case(IDLE):
	{
		//currentAnimation = &(goingRight == false ? idleLeftAnim : idleRightAnim);
		break;
	}
	case(RUNNING):
	{

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT
			)
			isGoingRight = false;
		else
			isGoingRight = true;
		//currentAnimation = &(goingRight == false ? leftAnim : rightAnim);
		break;
	}
	case(JUMPING):
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			isGoingRight = false;
		else
			isGoingRight = true;
		//currentAnimation = &climbAnim;
		break;
	}
	case(DOUBLE_JUMPING):
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)

			isGoingRight = false;
		else
			isGoingRight = true;
		//currentAnimation = &(goingRight == false ? shootLeftAnim : shootRightAnim);
		break;
	}
	case(DYING):
	{
		//currentAnimation = &(goingRight == false ? dieLeftAnim : dieRightAnim);
		break;
	}

	}

	playerState = newState;
}