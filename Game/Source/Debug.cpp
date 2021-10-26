#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"

Debug::Debug(bool startEnabled)
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

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		//app->map->showColliders == true;
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