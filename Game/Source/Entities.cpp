#include "Entities.h"

#include "Log.h"
#include "App.h"
#include "Textures.h"

#include "Bat.h"

Entities::Entities() : Module()
{
	name.create("entities");
}

bool Entities::Awake(pugi::xml_node& config)
{
	return true;
}

bool Entities::Start()
{
	batTexture = app->tex->Load("Assets/enemies/bat/bat.png");

	fPoint pos;
	pos.x = 176;
	pos.y = 928;

	AddEntity(pos, Entity::Type::BAT);

	return true;
}

bool Entities::PreUpdate()
{
	return true;
}

bool Entities::Update(float dt)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		entityList[i]->Update(dt);
	}

	return true;
}

bool Entities::PostUpdate()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		entityList[i]->Draw();
	}

	return true;
}

bool Entities::CleanUp()
{
	return true;
}

void Entities::AddEntity(fPoint position, Entity::Type type)
{
	switch (type)
	{
	case Entity::Type::BAT:
		Entity* e = (Entity*)(new Bat(position, batTexture, type));
		entityList.PushBack(e);
		break;
	}
}