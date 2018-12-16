#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "j1Fonts.h"
#include "UIElement.h"
#include "Button.h"

enum class Slider_TYPE;

class Slider : public UIElement
{
public:

	Slider(const int &x, const int &y, const SDL_Rect &slider_rect, UIElement* parent = nullptr, Slider_TYPE type = Slider_TYPE::Y);

	void AddTargets(UIElement*);
	void AddThumb(Button*);
	void SetSliderValueStart(float slider_value);

	void InnerDraw();
	void PostUpdate();

	float GetSliderValue() const;
	Button* GetSliderButton() const;

private:

	float value = 0.0F;
	Slider_TYPE type = Slider_TYPE::Y;
	Button* thumb = nullptr;
	SDL_Rect image = { 0,0,0,0 };
	p2List<UIElement*> control;
};

#endif //__UISLIDER_H__
