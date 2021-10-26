#include "Collisions.h"

#include "App.h"
#include "Render.h"
#include "Log.h"

Collisions::Collisions() : Module()
{

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

	for (int i = 0; i < dynamicColliders.count(); i++) {
		for (int j = 0; j < staticColliders.count(); j++) {
			if (dynamicColliders[i]->Intersects(staticColliders[j]->rect)) {
				for each (Module * m in dynamicColliders[i]->listeners) {
					if (m == nullptr) break;
					m->OnCollision(dynamicColliders[i], staticColliders[j]);
				}

				for each (Module * m in staticColliders[j]->listeners) {
					if (m == nullptr) break;
					m->OnCollision(staticColliders[j], dynamicColliders[i]);
				}
			}
		}
	}
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
	staticColliders.clear();
	dynamicColliders.clear();

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	ret = new Collider(rect, type, listener);

	if (type == Collider::Type::STATIC)
	{
		staticColliders.add(ret);
	}
	else if (type == Collider::Type::DYNAMIC)
	{
		dynamicColliders.add(ret);
	}
	return ret;
}


void Collisions::RemoveCollider(Collider* collider)
{
	
}