#include "Player.h"

#include "App.h"
#include "Input.h"
#include "Animation.h"
#include "Textures.h"
#include "SDL/include/SDL_scancode.h"
#include "Collisions.h"
#include "Render.h"
#include "Scene.h"
#include "Audio.h"
#include "ModuleUI.h"
#include "Entities.h"

#include "../Defs.h"
#include "../Log.h"
#include <math.h>

Player::Player(Module* parent, fPoint position, Type type) : Entity(parent, position, texture, type)
{

}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player config");
	bool ret = true;

	texturePath = config.child("texture").child_value();

	pugi::xml_node movement = config.child("movement");

	gravity = movement.attribute("gravity").as_float();
	jumpForce = movement.attribute("jumpForce").as_float();
	speed = movement.attribute("speed").as_int();
	maxJumps = movement.attribute("maxJumps").as_int();

	pugi::xml_node audio = config.child("audio");

	jumpFxPath = audio.attribute("jump").as_string();
	doubleJumpFxPath = audio.attribute("doubleJump").as_string();
	gameOverFxPath = audio.attribute("gameOver").as_string();
	gameStartFxPath = audio.attribute("gameStart").as_string();
	pickUpFruitFxPath = audio.attribute("fruit").as_string();
	checkpointFxPath = audio.attribute("checkpoint").as_string();
	pickUpHeartFxPath = audio.attribute("heart").as_string();


	return ret;
}

bool Player::Start()
{
	bool ret = true;

	health = 3;

	saveOnce1 = true;
	saveOnce2 = true;

	LOG("Loading Player textures");

	texture = app->tex->Load(texturePath);

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 3, (int)position.y + 10, 16, 16 }), Collider::Type::DYNAMIC, parent);

	jumpFx = app->audio->LoadFx(jumpFxPath);
	doubleJumpFx = app->audio->LoadFx(doubleJumpFxPath);
	gameOverFx = app->audio->LoadFx(gameOverFxPath);
	gameStartFx = app->audio->LoadFx(gameStartFxPath);
	pickUpFruitFx = app->audio->LoadFx(pickUpFruitFxPath);
	checkpointFx = app->audio->LoadFx(checkpointFxPath);
	pickUpHeartFx = app->audio->LoadFx(pickUpHeartFxPath);

	currentAnim = &idleRightAnim;

	idleRightAnim.loop = idleLeftAnim.loop = runRightAnim.loop = runLeftAnim.loop = true;
	idleRightAnim.speed = idleLeftAnim.speed = 16.0f;
	runRightAnim.speed = runLeftAnim.speed = 25.0f;

	for (int i = 0; i < 11; i++)
	{
		idleRightAnim.PushBack({ i * 32,0,22,26 });
	}

	for (int i = 0; i < 11; i++)
	{
		idleLeftAnim.PushBack({ i * 32,26,22,26 });
	}

	for (int i = 0; i < 11; i++)
	{
		runRightAnim.PushBack({ i * 32,62,24,28 });
	}

	for (int i = 0; i < 11; i++)
	{
		runLeftAnim.PushBack({ i * 32,90,24,28 });
	}

	jumpRightAnim.PushBack({ 0,188,22,28 });

	jumpLeftAnim.PushBack({ 68,188,22,28 });

	doubleJumpLeftAnim.PushBack({ 68,188,22,28 });

	doubleJumpRightAnim.PushBack({ 0,188,22,28 });

	fallRightAnim.PushBack({ 22,188,23,28 });

	fallLeftAnim.PushBack({ 45,188,23,28 });

	prepareToSpawnAnim.PushBack({ 0,0,0,0 });

	appearAnim.loop = disappearLeftAnim.loop = disappearRightAnim.loop = false;
	appearAnim.speed = disappearLeftAnim.speed = disappearRightAnim.speed = 11.0f;

	for (int i = 0; i < 390; i += 55)
	{
		appearAnim.PushBack({ i,227,55,55 });
	}

	for (int i = 0; i < 390; i += 55)
	{
		disappearLeftAnim.PushBack({ i,282,55,55 });
		disappearRightAnim.PushBack({ i,282,55,55 });
	}

	return ret;
}

bool Player::Update(float dt)
{
	UpdateState(dt);
	UpdateLogic(dt);

	if (godMode) GodMovement(dt);

	return true;
}

bool Player::Draw()
{
	SDL_Rect rect = currentAnim->GetCurrentFrame();

	if (currentAnim == &disappearLeftAnim || currentAnim == &disappearRightAnim)
	{
		app->render->DrawTexture(texture, position.x - 16, position.y - 15, &rect);
	}
	else
		app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

void Player::Collision(Collider* b, float dt)
{
	if (godMode) return;

	iPoint center;
	iPoint batCenter;

	int xDiff;
	int yDiff;

	switch (b->type)
	{
	case(Collider::Type::ENDLEVEL):
		app->scene->FadeToNewState(app->scene->GameplayState::CREDITS_SCREEN);
		return;
		break;

	case(Collider::Type::DEATH):
		ChangeState(playerState, DYING);
		break;

	case(Collider::Type::ITEMSCORE):
		app->ui->score += 1000;
		app->audio->PlayFx(pickUpFruitFx, 0);
		b->pendingToDelete = true;
		break;

	case(Collider::Type::ITEMHEALTH):
		health++;
		app->audio->PlayFx(pickUpHeartFx, 0);
		b->pendingToDelete = true;
		break;

	case(Collider::Type::CHECKPOINT):
		app->entities->GetPlayer()->initialPosition = app->entities->GetPlayer()->position;
		app->audio->PlayFx(checkpointFx, 0);
		break;

	case(Collider::Type::BAT):
		center = iPoint(collider->rect.x + (collider->rect.w / 2), collider->rect.y + (collider->rect.h / 2));
		batCenter = iPoint(b->rect.x + (b->rect.w / 2), b->rect.y + (b->rect.h / 2));

		xDiff = batCenter.x - center.x;
		yDiff = batCenter.y - center.y;

		if (abs(yDiff) <= abs(xDiff) || yDiff < 0)
		{
			ChangeState(playerState, DYING);
		}

		break;

	case(Collider::Type::PIG):
		center = iPoint(collider->rect.x + (collider->rect.w / 2), collider->rect.y + (collider->rect.h / 2));
		batCenter = iPoint(b->rect.x + (b->rect.w / 2), b->rect.y + (b->rect.h / 2));

		xDiff = batCenter.x - center.x;
		yDiff = batCenter.y - center.y;

		if (abs(yDiff) <= abs(xDiff) || yDiff < 0)
		{
			ChangeState(playerState, DYING);
		}
		else
		{
			app->ui->score += 10000;
		}
		break;

	default:
		break;
	}

	if (b->type != Collider::Type::ENDLEVEL && b->type != Collider::Type::BAT && b->type != Collider::Type::PIG && b->type != Collider::Type::ITEMHEALTH && b->type != Collider::Type::ITEMSCORE && b->type != Collider::Type::CHECKPOINT)
	{
		int deltaX = collider->rect.x - b->rect.x;
		int deltaY = collider->rect.y - b->rect.y;
		if (std::abs(deltaX) > std::abs(deltaY))
		{
			if (deltaX > 0)
			{
				position.x += b->rect.x + b->rect.w - collider->rect.x;
			}
			else
			{
				position.x -= collider->rect.x + collider->rect.w - b->rect.x;
			}
		}
		else
		{
			if (deltaY > 0)
			{
				verticalVelocity = 0.0f;
				position.y += b->rect.y + b->rect.h - collider->rect.y;
			}
			else
			{
				if (verticalVelocity < 0.0f)
				{
					verticalVelocity = 0.0f;
					if (playerState != PlayerState::DYING)
					{
						ChangeState(playerState, IDLE);
					}
					position.y -= collider->rect.y + collider->rect.h - b->rect.y;
					availableJumps = maxJumps;
				}
			}
		}
		collider->SetPos((int)position.x, (int)position.y);
	}

}

void Player::UpdateState(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		isGoingRight = false;
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		isGoingRight = true;

	switch (playerState)
	{
		case PREPARE_TO_SPAWN:
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				ChangeState(PREPARE_TO_SPAWN, SPAWNING);
			break;
		}
		case SPAWNING:
		{
			if (currentAnim->HasFinished() == true)
				ChangeState(SPAWNING, IDLE);
			break;
		}
		case IDLE:
		{
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				ChangeState(playerState, RUNNING);

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !godMode)
			{
				app->audio->PlayFx(jumpFx, 0);
				if (availableJumps > 0)
				{
					/*if (verticalVelocity < 0.0f)
						availableJumps -= 2;
					else*/
					availableJumps--;
				}

				verticalVelocity = jumpForce;

				if (verticalVelocity > maxVerticalVelocity)
				{
					verticalVelocity = maxVerticalVelocity;
				}

				if (verticalVelocity < -maxVerticalVelocity)
				{
					verticalVelocity = -maxVerticalVelocity;
				}

				ChangeState(playerState, JUMPING);
			}

			break;
		}
		case RUNNING:
		{
			if (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
			{
				ChangeState(playerState, IDLE);
			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !godMode)
			{
				app->audio->PlayFx(jumpFx, 0);
				if (availableJumps > 0)
				{
					/*if (verticalVelocity < 0.0f)
						availableJumps -= 2;
					else*/
					availableJumps--;
				}

				verticalVelocity = jumpForce;

				if (verticalVelocity > maxVerticalVelocity)
				{
					verticalVelocity = maxVerticalVelocity;
				}

				if (verticalVelocity < -maxVerticalVelocity)
				{
					verticalVelocity = -maxVerticalVelocity;
				}

				ChangeState(playerState, JUMPING);
			}

			break;
		}
		case JUMPING:
		{
			//once animation is done change to falling
			// or simply add the falling sprite on jumping animations
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				if (availableJumps > 0)
				{
					availableJumps--;

					app->audio->PlayFx(doubleJumpFx, 0);

					verticalVelocity = jumpForce;

					if (verticalVelocity > maxVerticalVelocity)
					{
						verticalVelocity = maxVerticalVelocity;
					}

					if (verticalVelocity < -maxVerticalVelocity)
					{
						verticalVelocity = -maxVerticalVelocity;
					}
				}
			}		
		
			break;
		}
		case DYING:
		{
			break;
		}

	}
}

void Player::UpdateLogic(float dt)
{
	if (gravityOn == false)		
		gravityOn = true;

	if (!godMode && gravityOn && playerState != DYING) verticalVelocity -= gravity * dt;

	if (verticalVelocity > maxVerticalVelocity)
	{
		verticalVelocity = maxVerticalVelocity;
	}

	if (verticalVelocity < -maxVerticalVelocity)
	{
		verticalVelocity = -maxVerticalVelocity;
	}

	position.y -= verticalVelocity * dt;

	switch (playerState)
	{
		case PREPARE_TO_SPAWN:
		{
			currentAnim = &prepareToSpawnAnim;
			break;
		}
		case SPAWNING:
		{
			currentAnim = &appearAnim;
			break;
		}
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
				position.x += speed*dt;
			}
			else
			{
				currentAnim = &runLeftAnim;
				position.x -= speed*dt;
			}
	
			break;
		}
		case(JUMPING):
		{
			if (verticalVelocity > 0.0f)
			{
				if (availableJumps == 1)
				{
					if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
					{
						currentAnim = &jumpLeftAnim;
						position.x -= speed*dt;
					}
					else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
					{
						currentAnim = &jumpRightAnim;
						position.x += speed*dt;
					}
				}
				if (availableJumps == 0)
				{
					if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
					{
						currentAnim = &doubleJumpLeftAnim;
						position.x -= speed*dt;
					}
					else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
					{
						currentAnim = &doubleJumpRightAnim;
						position.x += speed*dt;
					}
				}
			}
			else
			{
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					currentAnim = &fallLeftAnim;
					position.x -= speed*dt;
				}
				else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					currentAnim = &fallRightAnim;
					position.x += speed*dt;
				}
			}
		
			break;
		}
		case(DYING):
		{
			gravityOn = false;
			verticalVelocity = 0.0f;

			if (isGoingRight == true)
				currentAnim = &disappearRightAnim;
			else
				currentAnim = &disappearLeftAnim;

			if (isDead == false)
			{
				app->audio->PlayFx(gameOverFx, 0);
				isDead = true;
			}

			if (currentAnim->HasFinished())
			{
				health--;

				if (health < 0)
					health = 0;

				if (health == 0)
				{
					app->scene->FadeToNewState(Scene::GAME_OVER_SCREEN);
				}
				else
				{
					app->audio->PlayFx(gameStartFx, 0);
					playerState = PlayerState::IDLE;
					verticalVelocity = 0.0f;
					position = initialPosition;
					app->entities->ResetEntities();
				}
			}

			break;

		}
	}

	collider->SetPos(position.x, position.y);

	currentAnim->Update(dt);
}

void Player::ChangeState(PlayerState previousState, PlayerState newState)
{
	if (playerState != PlayerState::DYING && newState == PlayerState::DYING)
	{
		disappearLeftAnim.Reset();
		disappearRightAnim.Reset();
	}

	playerState = newState;
}

void Player::Reload()
{
	app->audio->PlayFx(gameStartFx, 0);
	playerState = PlayerState::IDLE;
	verticalVelocity = 0.0f;
	if (health == 0)
	{
		health = 3;
		app->ui->score = 0;
	}
	if (collider != nullptr)
		app->collisions->RemoveCollider(collider);
	collider = app->collisions->AddCollider(SDL_Rect({ (int)(position.x + 3.0f), (int)(position.y + 10.0f), 16, 16 }), Collider::Type::DYNAMIC, parent);
	initialPosition = position;
	gravityOn = true;
	initialWaitCount = 0.0f;
}

void Player::GodMovement(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		position.y -= speed*dt;
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
		position.y += speed*dt;
}