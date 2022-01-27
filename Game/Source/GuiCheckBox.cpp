#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, SDL_Texture* tex) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->texture = tex;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        if (checked) render->DrawRectangle(bounds, 100, 100, 100, 255);
        else render->DrawRectangle(bounds, 100, 100, 100, 255);
        break;

    case GuiControlState::NORMAL:
        if (!checked) render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0,0,20,20 }), 0, 0, 0, 0, false);
        else render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 60,0,20,20 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::FOCUSED:
        if (!checked) render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 20,0,20,20 }), 0, 0, 0, 0, false);
        else render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 80,0,20,20 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::PRESSED:
        if (!checked) render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 40,0,20,20 }), 0, 0, 0, 0, false);
        else render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 100,0,20,20 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::SELECTED:
        break;

    default:
        break;
    }

    return false;
}