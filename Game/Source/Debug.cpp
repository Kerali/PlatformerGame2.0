#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"

#include "App.h"
#include "Input.h"
#include "Collisions.h"
#include "Log.h"

Debug::Debug() : Module()
{
	name.create("Debug");
}


Debug::~Debug()
{

}

bool Debug::Awake()
{
	return true;
}

bool Debug::Start()
{
	return true;
}

bool Debug::PreUpdate()
{
	return true;
}

bool Debug::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		ToggleColliders();
	}

	return ret;
}

bool Debug::PostUpdate()
{

	return true;
}

bool Debug::CleanUp()
{
	return true;
}

void Debug::ToggleColliders()
{
	if (app->collisions->showColliders == false)
	{
		app->collisions->showColliders = true;
	}
	else if (app->collisions->showColliders == true) 
	{
		app->collisions->showColliders = false;
	}
}