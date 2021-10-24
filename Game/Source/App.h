#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: TODO 1: Create methods to request Load / Save

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate(); 


	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// L02: TODO 5: Declare methods to load/save game

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Map* map;

	bool Load();
	bool Save();

	void RequestSave() {
		requestSave = true;
	}

	void RequestLoad() {
		requestLoad = true;
		
	}

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	uint frames;
	float dt;

	// L02: TODO 1: Create required variables to request load / save and 
	// the filename for save / load
	pugi::xml_document saveGame;
	pugi::xml_node save_state;
	pugi::xml_node rend;//variable que carga las cosas en renderer
	pugi::xml_node inp;//carga en input
	pugi::xml_node sce;//carga en scene
	pugi::xml_node wi;//carga en window
	pugi::xml_node au;//carga en audio


	//PREGUNTASELO A RAMÓN
	bool requestLoad = false;
	bool requestSave = false;

};

extern App* app;

#endif	// __APP_H__