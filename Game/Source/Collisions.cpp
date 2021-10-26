#include "Collisions.h"

#include "App.h"
#include "Render.h"

Collisions::Collisions() : Module()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;
}

Collisions::~Collisions()
{

}

// Called before render is available
bool Collisions::Awake(pugi::xml_node&)
{
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Collisions::Start()
{
	bool ret = true;
	return ret;
}

// Called each loop iteration
bool Collisions::PreUpdate()
{
	bool ret = true;
	return ret;
}

bool Collisions::Update(float dt)
{
	bool ret = true;
	return ret;
}

bool Collisions::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool Collisions::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}


void Collisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}