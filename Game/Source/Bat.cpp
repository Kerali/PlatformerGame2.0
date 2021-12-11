#include "Bat.h"
#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Debug.h"
#include "Log.h"
#include "Collisions.h"

#include <math.h>

Bat::Bat(Module* parent, fPoint position, SDL_Texture* texture, Type type, int s) : Entity(parent, position, texture, type)
{
	idleAnimation.GenerateAnimation(SDL_Rect({ 0, 0, 216, 25 }), 1, 12);
	idleAnimation.speed = 6.0f;

	flyingLeftAnimation.GenerateAnimation(SDL_Rect({ 0, 25, 322, 26 }), 1, 7);
	flyingLeftAnimation.speed = 10.0f;

	flyingRightAnimation.GenerateAnimation(SDL_Rect({ 0, 51, 322, 26 }), 1, 7);
	flyingRightAnimation.speed = 10.0f;

	questionMarkAnimation.GenerateAnimation(SDL_Rect({ 322, 0, 96, 96 }), 3, 3);
	questionMarkAnimation.speed = 10.0f;

	deathAnimation.GenerateAnimation(SDL_Rect({ 0, 96, 390, 55 }), 1, 7);
	deathAnimation.speed = 10.0f;
	deathAnimation.loop = false;

	currentAnimation = &idleAnimation;

	lastPlayerPosition.x = -1;
	lastPlayerPosition.y = -1;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 18, 24 }), Collider::Type::BAT, parent);


	speed = s;

	state = State::IDLE;
}

bool Bat::Start()
{
	return true;
}

bool Bat::Update(float dt)
{
	currentAnimation->Update(dt);

	iPoint playerPos;
	playerPos.x = app->player->position.x / app->map->data.tileWidth;
	playerPos.y = app->player->position.y / app->map->data.tileHeight;

	iPoint gridPos;
	gridPos.x = position.x / app->map->data.tileWidth;
	gridPos.y = position.y / app->map->data.tileHeight;

	if (playerPos != lastPlayerPosition && playerPos.DistanceTo(gridPos) <= 12 && state != State::DYING)
	{
		lastPlayerPosition = playerPos;

		int n = app->pathfinding->CreatePath(gridPos, playerPos, false, 0, 14);
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
			path = *newPath;
			if (j < path.Count())
				pathIndex = j;
			else
				pathIndex = 0;
			state = State::FLYING;
		}
	}

	fPoint pixelPosition;
	float distance;

	switch (state)
	{
	case State::IDLE:
		currentAnimation = &idleAnimation;
		break;
	case State::FLYING:

		//currentAnimation = &flyingLeftAnimation;

		if (pathIndex >= path.Count())
			break;

		pixelPosition.x = path[pathIndex].x * app->map->data.tileWidth;
		pixelPosition.y = path[pathIndex].y * app->map->data.tileHeight;

		distance = pixelPosition.DistanceTo(position);

		if (distance == 0)
		{
			pathIndex++;
		}
		else
		{
			float xDiff = pixelPosition.x - position.x;
			float yDiff = pixelPosition.y - position.y;

			if (xDiff < 0)
			{
				currentAnimation = &flyingLeftAnimation;
			}
			else if (xDiff >= 0)
			{
				currentAnimation = &flyingRightAnimation;
			}

			if (abs(xDiff) > abs(yDiff))
			{
				int xDir = (xDiff > 0) ? 1 : -1;
				if (abs(xDiff) < abs(xDir * speed * dt))
				{
					position.x += xDiff;
				}
				else
					position.x += xDir * speed * dt;
			}
			else
			{
				int yDir = (yDiff > 0) ? 1 : -1;
				if (abs(yDiff) < abs(yDir * speed * dt))
				{
					position.y += yDiff;
				}
				else
					position.y += yDir * speed * dt;
			}
		}
		break;

	case State::DYING:
		currentAnimation = &deathAnimation;
		if (currentAnimation->HasFinished())
		{
			pendingToDelete = true;
		}
		break;
	}

	collider->SetPos(position.x, position.y);

	questionMarkAnimation.Update(dt);

	return true;
}

bool Bat::Draw()
{
	if (state == State::FLYING)
	{
		app->render->DrawTexture(texture, position.x - 14, position.y, &currentAnimation->GetCurrentFrame());
	}
	else if (state == State::DYING)
	{
		currentAnimation = &deathAnimation;
		app->render->DrawTexture(texture, position.x - 20, position.y - 15, &currentAnimation->GetCurrentFrame());
	}
	else
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());

	if (hasPath)
	{
		if (app->debug->showPaths)
			app->pathfinding->DrawPath(&path, 255, 0, 0);
	}
	else
	{
		if (state == State::FLYING)
		{
			app->render->DrawTexture(texture, position.x - 6, position.y - 32, &questionMarkAnimation.GetCurrentFrame());
		}
	}

	return true;
}

void Bat::Collision(Collider* other)
{
	if (other == app->player->collider)
	{
		iPoint center = iPoint(collider->rect.x + (collider->rect.w / 2), collider->rect.y + (collider->rect.h / 2));
		iPoint playerCenter = iPoint(other->rect.x + (other->rect.w / 2), other->rect.y + (other->rect.h / 2));

		int xDiff = center.x - playerCenter.x;
		int yDiff = center.y - playerCenter.y;

		if (abs(yDiff) > abs(xDiff) && yDiff > 0 && app->player->verticalVelocity < 0.0f)
		{
			state = State::DYING;
			collider->pendingToDelete = true;
			app->player->verticalVelocity = app->player->jumpForce;
		}
	}
}