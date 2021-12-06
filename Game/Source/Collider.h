#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

#define MAX_LISTENERS 100

class Module;

struct Collider
{
	enum Type
	{
		STATIC,
		DYNAMIC,
		ENDLEVEL,
		ITEMSCORE,
		ITEMHEALTH,
		CHECKPOINT,
		DEATH
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	void AddListener(Module* listener);

	//Variables
	SDL_Rect rect;
	Type type;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif
