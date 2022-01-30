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
		TITLE_MENU,
		PLAYING,
		PAUSE,
		GAME_OVER_SCREEN,
		LOGO_SCREEN,
		CREDITS_SCREEN
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

	Animation continueButtonAnim;
	Animation newGameButtonAnim;
	Animation settingsButtonAnim;
	Animation creditsButtonAnim;
	Animation exitButtonAnim;

	bool continueButtonPressed = false;
	bool continueButtonDisabled = true;
	bool exit = false;
	bool fading = false;

private:
	SDL_Texture* screenTexture = nullptr;
	SDL_Texture* titleMenuTex = nullptr;
	SDL_Texture* continueButtonTex = nullptr;
	SDL_Texture* newGameButtonTex = nullptr;
	SDL_Texture* settingsButtonTex = nullptr;
	SDL_Texture* creditsButtonTex = nullptr;
	SDL_Texture* exitButtonTex = nullptr;
	SDL_Texture* creditsTex = nullptr;

	int buttonsPosX = 183;
	int buttonsPosY = 125;
	int creditsPosY = 0;
	int creditsSpeed = 70;
	float creditCooldown = 0.0f;
	float maxcreditCooldown = 2.5f;

	Animation* screenDisplayAnim;
	Animation titleScreenAnim;
	Animation titleMenuAnim;
	Animation gameOverAnim;
	Animation logoScreenAnim;
	Animation turnOffAnim;
	Animation creditsAnim;

	float currentFade = 0.0f;
	GameplayState targetState = gameplayState;
	SDL_Rect fullScreenRect;

	const char* musicPath;
	const char* screenTexturePath;
	const char* titleMenuPath;
	const char* titleButtonsPath;
	const char* creditsPath;

};

#endif // __SCENE_H__