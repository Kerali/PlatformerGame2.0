#include "Heart.h"
#include "Render.h"
#include "Player.h"
#include "Entities.h"

Heart::Heart(Module* parent, fPoint position, SDL_Texture* texture, Type type) : Entity(parent, position, texture, type)
{

	idleAnimation.GenerateAnimation(SDL_Rect({ 0, 0, 105, 21 }), 1, 5);
	idleAnimation.speed = 6.0f;

	disappearAnimation.GenerateAnimation(SDL_Rect({ 0, 21, 192, 32 }), 1, 6);
	disappearAnimation.speed = 15.0f;
	disappearAnimation.loop = false;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 16, 24 }), Collider::Type::ITEMHEALTH, parent);

	state = State::IDLE;
}

bool Heart::Start()
{
	return true;
}

bool Heart::Update(float dt)
{

	if (state == State::DISAPPEAR)
	{
		currentAnimation = &disappearAnimation;
		if (currentAnimation->HasFinished())
			pendingToDelete = true;
	}

	currentAnimation->Update(dt);

	return true;
}

bool Heart::Draw()
{
	if (state == State::DISAPPEAR)
	{
		currentAnimation = &disappearAnimation;
		app->render->DrawTexture(texture, position.x - 10, position.y - 10, &currentAnimation->GetCurrentFrame());
	}
	else
		app->render->DrawTexture(texture, position.x - 2, position.y - 2, &currentAnimation->GetCurrentFrame());

	return true;
}

void Heart::Collision(Collider* other, float dt)
{
	if (other == app->entities->GetPlayer()->collider)
	{
		state = State::DISAPPEAR;
		collider->pendingToDelete = true;
	}
}