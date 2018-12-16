#ifndef _CHECKBOX_
#define _CHECKBOX_

#include "UIElement.h"

class CheckBox :public UIElement {

public:

	CheckBox(const int &x, const int &y, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push, UIElement* parent);
	~CheckBox() {};
	
	void InnerDraw();

	void AddOptions(CheckBox* option);
	void Clicked();
	bool GetIsClicked()const;
		
	void SetRects(const SDL_Rect &std_rect, const SDL_Rect &hl_rect, const SDL_Rect &click_rect);
	
public:
	bool is_option = false;

private:
	bool box_clicked = false;
	p2List<CheckBox*> options;

	SDL_Rect idle_rect = { 0,0,0,0 };
	SDL_Rect hovered_rect = { 0,0,0,0 };
	SDL_Rect clicked_rect = { 0,0,0,0 };
};

#endif // !_UICHECKBOX_


