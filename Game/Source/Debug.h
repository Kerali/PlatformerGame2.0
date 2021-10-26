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
	Debug(bool startEnabled);

	// Destructor
	~Debug();

	//Called at the beginning of the application execution
	bool Init();

	//Called when the module is activated
	//By now we will consider all modules to be permanently active
	bool Start();

	//Called at the beginning of each application loop
	bool PreUpdate();

	//Called at the middle of each application loop
	bool Update();

	//Called at the end of each application loop
	bool PostUpdate();

	//Called at the end of the application
	bool CleanUp();

	// Switches isEnabled and calls Start() method
	void Enable();

	// Switches isEnabled and calls CleanUp() method
	void Disable();

	inline bool IsEnabled() const { return isEnabled; }


private:
	bool isEnabled = true;
};

#endif
