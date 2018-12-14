#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Image.h"
#include "p2SString.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "Label.h"


class Button : public UIElement {

public:
	//------------------------------Constructor & Destructor Function--------------------------------//
	Button(const int &x, const int &y, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push, UIElement* parent) : UIElement(BUTTON, x, y, parent, true, idle.w, idle.h) {
		position = { x,y,idle.w,idle.h };
		button_rect = idle;
		hovered_rect = hover;
		clicked_rect = push;
	}
	~Button() {}
	//------------------------------Constructor & Destructor Function--------------------------------//


	//--------------------Draw Function--------------------//
	void InnerDraw()
	{
		switch (current_state) {
		case Mouse_Event::HOVER:
			if(drawable)
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &hovered_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		case Mouse_Event::CLICKED_DOWN:
			if (drawable)
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &clicked_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		default:
			if (drawable)
				App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &button_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		}
	}
	//--------------------Draw Function--------------------//


	//-------------Factory Functions--------------//
	void SetRects(SDL_Rect std_rect, SDL_Rect hl_rect, SDL_Rect click_rect) {
		button_rect = std_rect;
		hovered_rect = hl_rect;
		clicked_rect = click_rect;

		if (position.w == 0 && position.h == 0) {
			position.w = std_rect.w;
			position.h = std_rect.h;
		}
	}

	SDL_Rect GetClickedRect() const {
		return clicked_rect;
	}

	void CenterTextInButton() {
		int width_space = (this->button_rect.w - partner->position.w) / 2;
		int height_space = (this->button_rect.h - partner->position.h) / 2;
		partner->SetPos(width_space, height_space);
	}
		
	//-------------Factory Functions--------------//


private:
	SDL_Rect button_rect = { 0,0,0,0 };
	SDL_Rect hovered_rect = { 0,0,0,0 };
	SDL_Rect clicked_rect = { 0,0,0,0 };

public:
	Label* partner = nullptr;
};
#endif // !_UIBUTTON_

