#include "Bat.h"
#include "App.h"
#include "Render.h"

Bat::Bat(fPoint position, SDL_Texture* texture, Type type) : Entity(position, texture, type)
{
	idleAnimation.GenerateAnimation(SDL_Rect({ 0, 0, 216, 25 }), 1, 12);
	idleAnimation.speed = 6.0f;

	currentAnimation = &idleAnimation;
}

bool Bat::Start()
{
	return true;
}

bool Bat::Update(float dt)
{
	currentAnimation->Update(dt);

	return true;
}

bool Bat::Draw()
{
	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());

	return true;
}