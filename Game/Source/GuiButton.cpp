#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* tex) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->texture = tex;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
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
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiButton::Draw(Render* render)
{
    int yOffset = 3;

    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;

    case GuiControlState::NORMAL:
        break;

    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 0,0,9,7 }), 0, 0, 0, 0, false);
        render->DrawTexture(texture, bounds.x + bounds.w - 9, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 18,0,9,7 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::PRESSED:
        render->DrawTexture(texture, bounds.x, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 9,0,9,7 }), 0, 0, 0, 0, false);
        render->DrawTexture(texture, bounds.x + bounds.w - 9, bounds.y - yOffset + bounds.h / 2, &SDL_Rect({ 27,0,9,7 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::SELECTED:
        break;

    default:
        break;
    }

    return false;
}