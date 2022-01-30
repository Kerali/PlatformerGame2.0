#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Audio.h"
#include "ModuleUI.h"
#include "GuiManager.h"
#include "Entities.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	pugi::xml_node music = config.child("music");

	musicPath = music.attribute("path").as_string();

	pugi::xml_node texture = config.child("texture");

	screenTexturePath = texture.attribute("screen").as_string();

	titleButtonsPath = texture.attribute("titleButtons").as_string();

	titleMenuPath = texture.attribute("titleMenu").as_string();

	creditsPath = texture.attribute("creditsScreen").as_string();

	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	fullScreenRect = SDL_Rect({ 0, 0, app->render->camera.w, app->render->camera.h });

	screenTexture = app->tex->Load(screenTexturePath);
	if (screenTexture == nullptr)
		LOG("Couldn't load title screen");

	titleMenuTex = app->tex->Load(titleMenuPath);
	creditsTex = app->tex->Load(creditsPath);

	continueButtonTex = app->tex->Load(titleButtonsPath);
	newGameButtonTex = app->tex->Load(titleButtonsPath);
	settingsButtonTex = app->tex->Load(titleButtonsPath);
	creditsButtonTex = app->tex->Load(titleButtonsPath);
	exitButtonTex = app->tex->Load(titleButtonsPath);

	titleScreenAnim.PushBack({ 0,360,480,360 });
	titleScreenAnim.PushBack({ 480,360,480,360 });

	titleMenuAnim.PushBack({ 0,0,480,360 });
	titleMenuAnim.PushBack({ 480,0,480,360 });

	creditsAnim.PushBack({ 0,0,480,360 });
	creditsAnim.PushBack({ 480,0,480,360 });

	gameOverAnim.PushBack({ 0,720,480,360 });
	gameOverAnim.PushBack({ 480,720,480,360 });

	logoScreenAnim.PushBack({ 0,0,480,360 });
	logoScreenAnim.PushBack({ 480,0,480,360 });

	turnOffAnim.PushBack({ 0,0,0,0, });

	continueButtonAnim.PushBack({ 0,0,101,24 });
	continueButtonAnim.PushBack({ 101,0,101,24 });

	newGameButtonAnim.PushBack({ 0,24,101,24 });
	newGameButtonAnim.PushBack({ 101,24,101,24 });

	settingsButtonAnim.PushBack({ 0,48,101,24 });
	settingsButtonAnim.PushBack({ 101,48,101,24 });

	creditsButtonAnim.PushBack({ 0,72,101,24 });
	creditsButtonAnim.PushBack({ 101,72,101,24 });

	exitButtonAnim.PushBack({ 0,96,101,24 });
	exitButtonAnim.PushBack({ 101,96,101,24 });

	app->audio->PlayMusic(musicPath);

	titleScreenAnim.loop = gameOverAnim.loop = logoScreenAnim.loop = creditsAnim.loop = true;
	continueButtonAnim.loop = newGameButtonAnim.loop = settingsButtonAnim.loop = creditsButtonAnim.loop = exitButtonAnim.loop = false;

	continueButtonAnim.speed = newGameButtonAnim.speed = settingsButtonAnim.speed = creditsButtonAnim.speed = exitButtonAnim.speed = 6.0f;
	logoScreenAnim.speed = 6.0f;
	titleScreenAnim.speed = 6.0f;
	titleMenuAnim.speed = 6.0f;
	creditsAnim.speed = 3.0f;
	gameOverAnim.speed = 1.8f;

	screenDisplayAnim = &logoScreenAnim;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && gameplayState == LOGO_SCREEN)
	{
		FadeToNewState(TITLE_SCREEN);
	}

	if (gameplayState == TITLE_SCREEN && continueButtonPressed == true)
	{
		app->scene->FadeToNewState(Scene::GameplayState::PLAYING);
		LOG("LOAD REQUESTED");
	}
	else if (gameplayState == TITLE_MENU)
	{
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			app->guimanager->DestroyAllGuiControls();
			FadeToNewState(TITLE_SCREEN);
		}
	}
	else if (gameplayState == CREDITS_SCREEN)
	{

		if (creditsPosY < 360)
		{
			creditsPosY += creditsSpeed * dt;
		}
		else
		{
			creditCooldown += dt;
			if (creditCooldown > maxcreditCooldown)
			{
				FadeToNewState(TITLE_SCREEN);
			}
		}
	}
	else if (gameplayState == GAME_OVER_SCREEN)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			FadeToNewState(TITLE_SCREEN);
	}

	if (gameplayState == PLAYING)
	{
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN || continueButtonPressed == true)
		{
			continueButtonPressed = false;
			app->scene->FadeToNewState(Scene::GameplayState::PLAYING);
			app->RequestLoad();
			LOG("LOAD REQUESTED");
		}

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->RequestSave();
			LOG("SAVE REQUESTED");
		}

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			LoadLevel("level1.tmx");
		}

		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			LoadLevel(currentLevel);
		}
	}

	// 8 to volume down and 9 to volume up
	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT)
	{
		app->audio->VolumeDown();
		LOG("Volume down");
	}

	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT)
	{
		app->audio->VolumeUp();
		LOG("Volume up");
	}

	if (gameplayState != targetState)
	{
		currentFade += 0.02f;
		if (currentFade >= 1.0f)
		{
			currentFade = 1.0f;
			ChangeGameplayState(targetState);
		}
	}
	else if (currentFade > 0.0f)
	{
		currentFade -= 0.02f;
	}
	else if (currentFade <= 0.0f)
	{
		currentFade = 0.0f;
		fading = false;
	}
	screenDisplayAnim->Update(dt);

	continueButtonAnim.Update(dt);
	newGameButtonAnim.Update(dt);
	settingsButtonAnim.Update(dt);
	creditsButtonAnim.Update(dt);
	exitButtonAnim.Update(dt);

	return true;
}

bool Scene::Load(pugi::xml_node& savedGame)
{
	FadeToNewState(PLAYING);
	LoadLevel(savedGame.attribute("currentLevel").as_string("level1.tmx"), false);

	return true;
}

bool Scene::Save(pugi::xml_node& savedGame)
{
	pugi::xml_attribute lvl = savedGame.append_attribute("currentLevel");
	lvl.set_value(currentLevel.GetString());

	return true;
}

void Scene::FadeToNewState(GameplayState newState)
{
	if (gameplayState == newState)
		return;
	if (fading)
		return;
	targetState = newState;
	currentFade = 0.0f;
	fading = true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	if (fading)
		return true;

	if (gameplayState == TITLE_SCREEN)
	{
		switch (control->type)
		{
		case GuiControlType::BUTTON:
			switch (control->id)
			{
			case 1:
				app->guimanager->DestroyAllGuiControls();
				continueButtonPressed = true;
				break;

			case 2:
				app->guimanager->DestroyAllGuiControls();
				FadeToNewState(PLAYING);
				break;

			case 3:
				app->guimanager->DestroyAllGuiControls();
				FadeToNewState(TITLE_MENU);
				app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 220, 62, 116, 23 }), 1);
				app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 220, 105, 116, 23 }), 2);
				app->guimanager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 316, 147, 20, 20 }), 1);
				app->guimanager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 316, 186, 20, 20 }), 2);
				break;

			case 4:
				app->guimanager->DestroyAllGuiControls();
				FadeToNewState(CREDITS_SCREEN);
				break;

			case 5:
				exit = true;
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	else if (gameplayState == TITLE_MENU)
	{
		switch (control->type)
		{
		case GuiControlType::CHECKBOX:
			switch (control->id)
			{
			case 1:
				//fullscreen
				break;

			case 2:
				LOG("VSYNC switched.");
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	else if (gameplayState == PLAYING)
	{
		switch (control->type)
		{
		case GuiControlType::BUTTON:
			switch (control->id)
			{
			case 1:
				app->guimanager->DestroyAllGuiControls();
				app->ui->uiToRender = 0;
				break;

			case 2:
				app->guimanager->DestroyAllGuiControls();
				app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 188, 93, 116, 23 }), 1);
				app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 0, 0, SDL_Rect({ 188, 141, 116, 23 }), 2);
				app->guimanager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 275, 166, 20, 20 }), 1);
				app->guimanager->CreateGuiControl(GuiControlType::CHECKBOX, 0, 0, SDL_Rect({ 238, 189, 20, 20 }), 2);
				app->ui->uiToRender = 2;
				break;

			case 3:
				app->guimanager->DestroyAllGuiControls();
				FadeToNewState(TITLE_SCREEN);
				break;

			case 4:
				app->guimanager->DestroyAllGuiControls();
				app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 199, 150, 41, 7 }), 5);
				app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ 240, 150, 35, 7 }), 6);
				app->ui->uiToRender = 3;
				break;

			case 5:
				app->entities->GetPlayer()->saveOnce1 = true;
				app->entities->GetPlayer()->saveOnce2 = true;
				app->RequestSave();
				LOG("SAVE REQUESTED");
				exit = true;
				break;

			case 6:
				exit = true;
				break;

			default:
				break;
			}
			break;

		case GuiControlType::CHECKBOX:
			switch (control->id)
			{
			case 1:			
				//do fullscreen
				break;

			case 2:
				LOG("VSYNC switched.");
				break;

			default:
				break;
			}
			break;

		default: break;
		}
	}
	return true;
}

void Scene::ChangeGameplayState(GameplayState newState)
{
	if (gameplayState == newState) return;

	switch (newState)
	{
		case PLAYING:
			screenDisplayAnim = &turnOffAnim;
			gameplayState = PLAYING;
			currentLevel.create("level1.tmx");
			app->map->Load("level1.tmx");
			app->entities->GetPlayer()->Reload();
			break;
		case TITLE_SCREEN:
			screenDisplayAnim = &titleScreenAnim;
			gameplayState = TITLE_SCREEN;
			app->map->CleanUp();
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->ui->uiToRender = 0;
			app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY, 101, 24 }), 1);
			app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 24, 101, 24 }), 2);
			app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 48, 101, 24 }), 3);
			app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 72, 101, 24 }), 4);
			app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 0, 0, SDL_Rect({ buttonsPosX, buttonsPosY + 96, 101, 24 }), 5);
			break;
		case TITLE_MENU:
			screenDisplayAnim = &titleMenuAnim;
			gameplayState = TITLE_MENU;
			app->map->CleanUp();
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->ui->uiToRender = 0;
			break;
		case GAME_OVER_SCREEN:
			screenDisplayAnim = &gameOverAnim;
			gameplayState = GAME_OVER_SCREEN;
			app->map->CleanUp();
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->ui->uiToRender = 0;
			break;
		case CREDITS_SCREEN:
			screenDisplayAnim = &creditsAnim;
			gameplayState = CREDITS_SCREEN;
			app->map->CleanUp();
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->ui->uiToRender = 0;
			creditsPosY = 0;
			break;
	}
}

void Scene::LoadLevel(SString name, bool loadEntities)
{
	app->entities->GetPlayer()->isDead = false;
	currentLevel = name;
	app->map->Load(name.GetString(), loadEntities);
	app->entities->GetPlayer()->Reload();
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (exit)
		ret = false;

	SDL_Rect rect = screenDisplayAnim->GetCurrentFrame();

	app->render->DrawTexture(screenTexture, 0, 0, &rect);

	if (gameplayState == TITLE_MENU)
	{
		app->render->DrawTexture(titleMenuTex, 0, 0, &rect, 0, 0, 0, false);
	}
	else if (gameplayState == TITLE_SCREEN)
	{
		SDL_Rect continueRect;

		if (continueButtonDisabled)
			continueRect = SDL_Rect({ 0,120,100,20 });
		else
			continueRect = continueButtonAnim.GetCurrentFrame();

		app->render->DrawTexture(continueButtonTex, buttonsPosX, buttonsPosY, &continueRect, 0, 0, 0, 0, false);

		SDL_Rect newGameRect = newGameButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(newGameButtonTex, buttonsPosX, buttonsPosY + 24, &newGameRect, 0, 0, 0, 0, false);

		SDL_Rect settingsRect = settingsButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(settingsButtonTex, buttonsPosX, buttonsPosY + 48, &settingsRect, 0, 0, 0, 0, false);

		SDL_Rect creditsRect = creditsButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(creditsButtonTex, buttonsPosX, buttonsPosY + 72, &creditsRect, 0, 0, 0, 0, false);

		SDL_Rect exitRect = exitButtonAnim.GetCurrentFrame();
		app->render->DrawTexture(exitButtonTex, buttonsPosX, buttonsPosY + 96, &exitRect, 0, 0, 0, 0, false);
	}
	else if (gameplayState == CREDITS_SCREEN)
	{
		app->render->DrawRectangle(fullScreenRect, 0, 0, 0, 255, true, false);
		app->render->DrawTexture(creditsTex, 0, fullScreenRect.h / 2 - creditsPosY, &rect, 0, 0, 0, false);
	}

	float adjustedFade = currentFade;
	if (adjustedFade < 0.0f) adjustedFade = 0.0f;
	if (adjustedFade > 1.0f) adjustedFade = 1.0f;

	int alpha = adjustedFade*255.0f;

	app->render->DrawRectangle(fullScreenRect, 0, 0, 0, alpha, true, false);

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
