#include "GuiManager.h"

#include "App.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

GuiManager::GuiManager() : Module()
{
	name.create("guimanager");
	// Created entities are added to the list	
}

GuiManager::~GuiManager()
{}

bool GuiManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool GuiManager::Start()
{
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

GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, { 0, 0, 20, 20 }, "EXIT");
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, { 1280 / 2 - 300 / 2, 200, 20, 20 }, "EXIT");
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, { 1280 / 2 - 300 / 2, 200, 20, 20 }, "EXIT");
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
		controls[i]->Draw(app->render);
	}
}
