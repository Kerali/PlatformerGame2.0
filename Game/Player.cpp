#include "Player.h"

#include "../App.h"
#include "../Input.h"
#include "Animation.h"
#include "../Textures.h"
#include "SDL/include/SDL_scancode.h"

#include "../Defs.h"
#include "../Log.h"

bool Player::Awake()
{
	bool ret = true;
	position.x = 30;
	position.y = 30;

	LOG("Loading Player textures");

	texture = app->tex->Load("Assets/SpriteSheet.png");
	currentAnim = &idleRightAnim;

	if (texture == nullptr)
	{
		LOG("Could't load player textures");
		ret = false;
	}

	return ret;
}

void Player::UpdateState()
{
	switch (playerState)
	{
	case IDLE:
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
				)
				ChangeState(playerState, RUNNING);

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
	switch (playerState)
	{
	case(IDLE):
	{

		break;
	}
	case(RUNNING):
	{



		break;
	}
	case(JUMPING):
	{


		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			--position.y;
			//currentAnimation->Update();
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			++position.y;
			//currentAnimation->Update();
		}

		break;
	}

	case(DOUBLE_JUMPING):
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			--position.y;

		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			++position.y;

		}

		//currentAnimation->Update();
		break;
	}
	case(DYING):
	{

		// my man is gone

		break;

	}

	}


}



void Player::ChangeState(PLAYER_STATE previousState, PLAYER_STATE newState)
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