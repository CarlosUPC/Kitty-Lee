#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "j1Fonts.h"
#include "UIElement.h"
#include "Button.h"

class Slider : public UIElement
{
public:
	SDL_Rect slider_rect;

private:
	uint slider_value = 0u;
	Button* slider_btn = nullptr;

public:
	Slider(int x, int y, UI_type type, SDL_Rect slider_rect, Button* slider_button, uint slider_button_pos, j1Module* callback, UIElement* parent = nullptr);

	
	void SetSliderValueStart(int slider_value);

	uint GetSliderValue() const;
	Button* GetSliderButton() const;
};

#endif //__UISLIDER_H__
