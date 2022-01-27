#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture* tex) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->texture = tex;
    sliderPosx = bounds.x + bounds.w - 9;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
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

            // TODO.
            unit = bounds.w / 100;
            value = (mouseX - bounds.x) / unit;
            value = round(value);

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                for (int i = 1; i <= 100; i++)
                {
                    if (i == value)
                    {
                        sliderPosx = ((i * unit) + bounds.x) - unit;
                    }
                }
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

bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
        break;

    case GuiControlState::NORMAL:
        render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 0, 116, 23 }), 0, 0, 0, 0, false);
        render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 0, 16, 23 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::FOCUSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 23, 116, 23 }), 0, 0, 0, 0, false);
        render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 0, 16, 23 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::PRESSED:
        render->DrawTexture(texture, bounds.x, bounds.y, &SDL_Rect({ 0, 23, 116, 23 }), 0, 0, 0, 0, false);
        render->DrawTexture(texture, sliderPosx, bounds.y, &SDL_Rect({ 116, 23 , 16, 23 }), 0, 0, 0, 0, false);
        break;

    case GuiControlState::SELECTED:
        break;

    default:
        break;
    }

    return false;
}