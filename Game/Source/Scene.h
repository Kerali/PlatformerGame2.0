#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
struct Animation;

class GuiControl;

class Scene : public Module
{
public:

	enum GameplayState
	{
		TITLE_SCREEN,
		PLAYING,
		PAUSE,
		GAME_OVER_SCREEN,
		LOGO_SCREEN
	};

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	void LoadLevel(SString name, bool loadEntities = true);

	void ChangeGameplayState(GameplayState newState);
	void FadeToNewState(GameplayState newState);

	virtual bool OnGuiMouseClickEvent(GuiControl* control);

public:
	SString currentLevel;
	bool gameStarted = false;
	bool gameOver = false;
	GameplayState gameplayState = LOGO_SCREEN;

private:
	SDL_Texture* screenTexture = nullptr;

	Animation* screenDisplayAnim;
	Animation titleScreenAnim;
	Animation gameOverAnim;
	Animation logoScreenAnim;
	Animation turnOffAnim;

	float currentFade = 0.0f;
	bool fading = false;
	GameplayState targetState = gameplayState;
	SDL_Rect fullScreenRect;

	const char* musicPath;
	const char* screenTexturePath;

	bool exit = false;
};

#endif // __SCENE_H__