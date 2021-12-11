#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"
#include "App.h"
#include "Input.h"
//#include "Map.h"
#include "SDL/include/SDL_scancode.h"

class Debug : public Module
{
public:
	Debug();

	// Destructor
	virtual ~Debug();

	//Called at the beginning of the application execution
	bool Awake();

	//Called when the module is activated
	//By now we will consider all modules to be permanently active
	bool Start();

	//Called at the beginning of each application loop
	bool PreUpdate();

	//Called at the middle of each application loop
	bool Update(float dt);

	//Called at the end of each application loop
	bool PostUpdate();

	//Called at the end of the application
	bool CleanUp();

	void ToggleColliders();
	void GodMode();

	bool showPaths = false;

private:
	bool isEnabled = true;
	int storeGravity;

	int storeFrameRateCap;
	bool capFrameRate = true;
};

#endif
