#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"

#include "App.h"
#include "Input.h"
#include "Map.h"
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

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
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
	if (app->map->showColliders == false)
	{
		app->map->showColliders = true;
	}
	else if (app->map->showColliders == true)
	{
		app->map->showColliders = false;
	}
}