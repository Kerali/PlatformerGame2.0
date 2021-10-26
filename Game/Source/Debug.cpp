#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"

#include "App.h"
#include "Input.h"
#include "Map.h"

Debug::Debug()
{

}


Debug::~Debug()
{

}

bool Debug::Init()
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

bool Debug::Update()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		ShowColliders();
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

void Debug::Enable()
{
	if (!isEnabled)
	{
		isEnabled = true;
		Start();
	}
}

void Debug::Disable()
{
	if (isEnabled)
	{
		isEnabled = false;
		CleanUp();
	}
}

void Debug::ShowColliders()
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