#include "j1App.h"
#include "Button.h"
#include "p2Log.h"
#include "j1Gui.h"
#include "Slider.h"


Slider::Slider(int x, int y, UI_type type, SDL_Rect slider_rect, Button* slider_button, uint slider_button_pos, j1Module* callback, UIElement* parent) : UIElement(SLIDER, x, y, parent, true, slider_rect.w, slider_rect.h)
{
	
	position = slider_rect;
	slider_btn= slider_button;
	slider_value = slider_button_pos * 100 / (position.w - slider_button->position.w);
}


void Slider::SetSliderValueStart(int slider_value) {
	slider_value = slider_value * 100 / (position.w - slider_btn->position.w);
	this->slider_btn->SetPos((this->position.w + 2) * slider_value / 100, 0);
}

uint Slider::GetSliderValue() const
{
	return slider_value;
}

Button * Slider::GetSliderButton() const
{
	return slider_btn;
}