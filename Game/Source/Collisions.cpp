#include "Collisions.h"

#include "App.h"
#include "Render.h"
#include "Log.h"
#include "Player.h"
#include "Entities.h"

Collisions::Collisions() : Module()
{
	name.create("collisions");
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

	for (int i = 0; i < dynamicColliders.count(); i++)
	{
		if (dynamicColliders[i]->pendingToDelete)
		{
			ListItem<Collider*>* c = app->collisions->dynamicColliders.At(i);
			app->collisions->dynamicColliders.del(c);
			delete c->data;
		}
	}

	for (int i = 0; i < staticColliders.count(); i++)
	{
		if (staticColliders[i]->pendingToDelete)
		{
			ListItem<Collider*>* c = app->collisions->staticColliders.At(i);
			app->collisions->staticColliders.del(c);
			delete c->data;
		}
	}

	return ret;
}

bool Collisions::Update(float dt)
{
	bool ret = true;

	for (int i = 0; i < dynamicColliders.count(); i++) {
		for (int j = 0; j < staticColliders.count(); j++) {
			if (dynamicColliders[i]->Intersects(staticColliders[j]->rect)) {
				for each (Module*m in dynamicColliders[i]->listeners)
				{
					if (m == nullptr) break;
					m->OnCollision(dynamicColliders[i], staticColliders[j], dt);
				}
				for each (Module*m in staticColliders[j]->listeners)
				{
					if (m == nullptr) break;
					m->OnCollision(staticColliders[j], dynamicColliders[i], dt);
				}
			}
		}
		for (int j = i; j < dynamicColliders.count(); j++)
		{
			if (i == j)
				continue;
			if (dynamicColliders[i]->Intersects(dynamicColliders[j]->rect))
			{
				for each (Module * m in dynamicColliders[i]->listeners)
				{
					if (m == nullptr) break;
					m->OnCollision(dynamicColliders[i], dynamicColliders[j], dt);
				}
				for each (Module * m in dynamicColliders[j]->listeners)
				{
					if (m == nullptr) break;
					m->OnCollision(dynamicColliders[j], dynamicColliders[i], dt);
				}
			}
		}
	}
	return ret;
}

bool Collisions::PostUpdate()
{
	bool ret = true;

	if (showColliders == true)
	{
		for (uint i = 0; i < staticColliders.count(); ++i)
		{
			if (staticColliders[i]->type == Collider::Type::DEATH)
			{
				DrawCollider(&staticColliders[i]->rect, 208, 48, 75, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ENDLEVEL)
			{
				DrawCollider(&staticColliders[i]->rect, 80, 72, 79, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ITEMHEALTH)
			{
				DrawCollider(&staticColliders[i]->rect, 255, 255, 255, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::ITEMSCORE)
			{
				DrawCollider(&staticColliders[i]->rect, 0, 0, 0, 80);
			}
			else if (staticColliders[i]->type == Collider::Type::CHECKPOINT)
			{
				DrawCollider(&staticColliders[i]->rect, 48, 171, 54, 80);
			}
			else
			{
				DrawCollider(&staticColliders[i]->rect, 59, 198, 191, 80);
			}
		}

		for (uint i = 0; i < dynamicColliders.count(); ++i)
		{
			DrawCollider(&dynamicColliders[i]->rect, 255, 0, 0, 80);
		}
	}

	return ret;
}

// Called before quitting
bool Collisions::CleanUp()
{
	for (int i = 0; i < staticColliders.count(); i++)
	{
		delete staticColliders[i];
	}

	for (int i = 0; i < dynamicColliders.count(); i++)
	{
		if (dynamicColliders[i] != app->entities->GetPlayer()->collider)
			delete dynamicColliders[i];
	}
	staticColliders.clear();
	dynamicColliders.clear();

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	ret = new Collider(rect, type, listener);

	if (type == Collider::Type::DYNAMIC || type == Collider::Type::PIG || type == Collider::Type::BAT)
	{
		dynamicColliders.add(ret);
	}
	else
	{
		staticColliders.add(ret);
	}
	return ret;
}

void Collisions::DrawCollider(const SDL_Rect* section, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	app->render->DrawRectangle(*section, r, g, b, a, true, true);
}

void Collisions::RemoveCollider(Collider* collider)
{
	for (int i = 0; i < staticColliders.count(); i++)
	{
		Collider* c = staticColliders[i];
		if (c == collider)
		{
			ListItem<Collider*>* item = staticColliders.At(i);
			staticColliders.del(item);
		}
	}

	for (int i = 0; i < dynamicColliders.count(); i++)
	{
		Collider* c = dynamicColliders[i];
		if (c == collider)
		{
			ListItem<Collider*>* item = dynamicColliders.At(i);
			dynamicColliders.del(item);
		}
	}
	delete collider;
}