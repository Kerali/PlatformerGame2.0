#include "GuiManager.h"

#include "App.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Textures.h"
#include "Audio.h"

GuiManager::GuiManager() : Module()
{
	name.create("guimanager");
	// Created entities are added to the list	
}

GuiManager::~GuiManager()
{}

bool GuiManager::Awake(pugi::xml_node& config)
{
	pugi::xml_node guimanagerPathN = config.child("guimanager");

	checkBoxPath = guimanagerPathN.attribute("checkbox").as_string();
	arrowMenuPath = guimanagerPathN.attribute("menuArrow").as_string();
	sliderPath = guimanagerPathN.attribute("slider").as_string();

	pugi::xml_node audio = config.child("audio");

	pressButtonFxPath = audio.attribute("pressButton").as_string();
	hoverButtonFxPath = audio.attribute("hoverButton").as_string();
	checkboxFxPath = audio.attribute("checkboxButton").as_string();

	return true;
}

bool GuiManager::Start()
{
	checkBoxTex = app->tex->Load(checkBoxPath);
	arrowMenuTex = app->tex->Load(arrowMenuPath);
	sliderTex = app->tex->Load(sliderPath);

	hoverButtonFx = app->audio->LoadFx(hoverButtonFxPath);
	pressButtonFx = app->audio->LoadFx(pressButtonFxPath);
	checkboxFx = app->audio->LoadFx(checkboxFxPath);
	

	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::PostUpdate()
{

	DrawAll();

	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int x, int y, SDL_Rect bounds, int id)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, arrowMenuTex);
		control->SetObserver(app->scene);
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, bounds, checkBoxTex);
		control->SetObserver(app->scene);
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds, sliderTex);
		control->SetObserver(app->scene);
		break;

	default: break;
	}
	id++;

	// Created entities are added to the list
	if (control != nullptr) controls.add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int i = controls.find(entity);
	ListItem<GuiControl*>* c = controls.At(i);
	controls.del(c);
}

void GuiManager::DestroyAllGuiControls()
{
	int u = controls.count();

	for (int i = 0; i < u; i++)
	{
		delete controls.At(0)->data;
		controls.del(controls.At(0));
	}
}

void GuiManager::AddGuiControl(GuiControl* entity)
{
	if (entity != nullptr) controls.add(entity);
}

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		for (int i = 0; i < controls.count(); i++)
		{
			controls[i]->Update(app->input, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.count(); i++)
	{
		if (showDebug)
			controls[i]->DrawDebug(app->render);
		controls[i]->Draw(app->render);
	}
}
