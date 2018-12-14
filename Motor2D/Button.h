#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "UIElement.h"

class Button : public UIElement {

public:
	
	Button(const int &x, const int &y, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push, UIElement* parent);
	~Button() {}

	void InnerDraw();

	void SetRects(const SDL_Rect &std_rect, const SDL_Rect &hover_rect, const SDL_Rect &click_rect);

private:
	SDL_Rect idle_rect = { 0,0,0,0 };
	SDL_Rect hovered_rect = { 0,0,0,0 };
	SDL_Rect clicked_rect = { 0,0,0,0 };

};
#endif // !_UIBUTTON_

