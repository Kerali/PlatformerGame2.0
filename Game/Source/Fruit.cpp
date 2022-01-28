#include "Fruit.h"
#include "Render.h"
#include "Player.h"
#include "Entities.h"

Fruit::Fruit(Module* parent, fPoint position, SDL_Texture* texture, Type type, int r) : Entity(parent, position, texture, type)
{

	idleAnimation.GenerateAnimation(SDL_Rect({ 0, r * 16, 272, 16 }), 1, 17);
	idleAnimation.speed = 30.0f;

	disappearAnimation.GenerateAnimation(SDL_Rect({ 0, 48, 192, 32 }), 1, 6);
	disappearAnimation.speed = 15.0f;
	disappearAnimation.loop = false;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 16, 24 }), Collider::Type::ITEMSCORE, parent);

	state = State::IDLE;
}

bool Fruit::Start()
{
	return true;
}

bool Fruit::Update(float dt)
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

bool Fruit::Draw()
{
	if (state == State::DISAPPEAR)
	{
		currentAnimation = &disappearAnimation;
		app->render->DrawTexture(texture, position.x - 8, position.y - 8, &currentAnimation->GetCurrentFrame());
	}
	else
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());

	return true;
}

void Fruit::Collision(Collider* other, float dt)
{
	if (other == app->entities->GetPlayer()->collider)
	{
		state = State::DISAPPEAR;
		collider->pendingToDelete = true;
	}
}