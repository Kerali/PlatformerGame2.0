#include "Pig.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Debug.h"
#include "Log.h"
#include "Collisions.h"
#include "Audio.h"

#include <math.h>

Pig::Pig(Module* parent, fPoint position, SDL_Texture* texture, Type type, int s, int h) : Entity(parent, position, texture, type)
{
	idleLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 0, 324, 30 }), 1, 9);
	idleLeftAnimation.speed = 10.0f;

	idleRightAnimation.GenerateAnimation(SDL_Rect({ 0, 30, 324, 30 }), 1, 9);
	idleRightAnimation.speed = 10.0f;

	walkingLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 60, 576, 30 }), 1, 16);
	walkingLeftAnimation.speed = 20.0f;

	walkingRightAnimation.GenerateAnimation(SDL_Rect({ 0, 90, 576, 30 }), 1, 16);
	walkingRightAnimation.speed = 20.0f;

	runningLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 120, 432, 30 }), 1, 12);
	runningLeftAnimation.speed = 22.0f;

	runningRightAnimation.GenerateAnimation(SDL_Rect({ 0, 150, 432, 30 }), 1, 12);
	runningRightAnimation.speed = 22.0f;

	hitLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 180, 180, 30 }), 1, 5);
	hitLeftAnimation.speed = 10.0f;
	hitLeftAnimation.loop = false;

	hitRightAnimation.GenerateAnimation(SDL_Rect({ 0, 210, 180, 30 }), 1, 5);
	hitRightAnimation.speed = 10.0f;
	hitRightAnimation.loop = false;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y - 12, 16, 28 }), Collider::Type::PIG, parent);

	speed = s;
	health = h;

	lastPlayerPosition.x = -1;
	lastPlayerPosition.y = -1;

	currentAnimation = &idleLeftAnimation;
	state = State::IDLE;
}

bool Pig::Start()
{
	return true;
}

void Pig::CleanUp()
{
	collider->pendingToDelete = true;
}

bool Pig::Update(float dt)
{
	UpdatePathfinding(dt);

	currentAnimation->Update(dt);

	switch (state)
	{
	case State::IDLE:
		if (!lookingRight)
			currentAnimation = &idleLeftAnimation;
		else
			currentAnimation = &idleRightAnimation;
		break;
	case State::WALKING:
		if (health > 1)
		{
			if (!lookingRight)
				currentAnimation = &walkingLeftAnimation;
			else
				currentAnimation = &walkingRightAnimation;
		}
		else
		{
			if (!lookingRight)
				currentAnimation = &runningLeftAnimation;
			else
				currentAnimation = &runningRightAnimation;
		}
		break;
	case State::HIT:
		if (!lookingRight)
			currentAnimation = &hitLeftAnimation;
		else
			currentAnimation = &hitRightAnimation;
		break;
	case State::LOST:
		if (!lookingRight)
			currentAnimation = &idleLeftAnimation;
		else
			currentAnimation = &idleRightAnimation;
		break;
	case State::DYING:
		currentAnimation = &deathAnimation;
	}

	return true;
}

void Pig::UpdatePathfinding(float dt)
{
	iPoint playerPos;
	playerPos.x = app->player->position.x / app->map->data.tileWidth;
	playerPos.y = app->player->position.y / app->map->data.tileHeight;

	iPoint gridPos;
	gridPos.x = position.x / app->map->data.tileWidth;
	gridPos.y = position.y / app->map->data.tileHeight;

	if (playerPos != lastPlayerPosition && playerPos.DistanceTo(gridPos) <= 16 && state != State::DYING && !app->player->godMode)
	{
		lastPlayerPosition = playerPos;

		int n = app->pathfinding->CreatePath(gridPos, playerPos, true, 3, 20);
		if (n == -1)
		{
			hasPath = false;
			path.Clear();
			path.PushBack(iPoint(gridPos.x, gridPos.y));
		}
		else
		{
			hasPath = true;
			const DynArray<iPoint>* newPath = app->pathfinding->GetLastPath();
			int j = 0;
			for (int i = 0; i < path.Count(); i++)
			{
				if (path[i] != (*newPath)[j])
					continue;
				j++;
				break;
			}
			path.Clear();
			for (int i = 0; i < newPath->Count(); i++)
			{
				path.PushBack((*newPath)[i]);
			}
			if (j < path.Count())
				pathIndex = j;
			else
				pathIndex = 0;
		}
	}

	if (hasPath)
	{
		state = State::WALKING;
	}
	else
	{
		state = State::IDLE;
	}
}

bool Pig::Draw()
{
	if (hasPath)
		app->pathfinding->DrawPath(&path, 255, 0, 0);

	app->render->DrawTexture(texture, position.x - 10, position.y - 14, &currentAnimation->GetCurrentFrame());

	return true;
}

void Pig::Collision(Collider* other)
{
	if (other == app->player->collider)
	{

	}
}

int Pig::GetJumpFrameCount(int deltaY)
{
	if (deltaY <= 0)
		return 0;
	else
	{
		switch (deltaY)
		{
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 5;
		case 4:
			return 8;
		default:
			return 14;
		}
	}
}