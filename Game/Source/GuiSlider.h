#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(uint32 id, SDL_Rect bounds, SDL_Texture* tex);
    virtual ~GuiSlider();

    bool Update(Input* input, float dt);
    bool Draw(Render* render);

private:

    // GuiSlider specific properties
    // Maybe some animation properties for state change?
    int sliderPosx;
    int value;
    int unit;

    int minValue;
    int maxValue;
};

#endif // __GUISLIDER_H__
