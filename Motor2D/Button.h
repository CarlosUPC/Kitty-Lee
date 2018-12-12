#ifndef _BUTTON_
#define _BUTTON_

#include "Image.h"
#include "p2SString.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Gui.h"

class Button : public UIElement {

public:
	//------------------------------Constructor & Destructor Function--------------------------------//
	Button(const int &x, const int &y, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push, const UIElement* parent) : UIElement(BUTTON, x, y, parent, true, idle.w, idle.h) {}
	~Button() {}
	//------------------------------Constructor & Destructor Function--------------------------------//


	//--------------------Draw Function--------------------//
	void InnerDraw()
	{
		switch (current_state) {
		case Mouse_Event::HOVER:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &hovered_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		case Mouse_Event::CLICKED:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &clicked_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		default:
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
	//-------------Factory Functions--------------//


private:
	SDL_Rect button_rect = { 0,0,0,0 };
	SDL_Rect hovered_rect = { 0,0,0,0 };
	SDL_Rect clicked_rect = { 0,0,0,0 };
};
#endif // !_UIBUTTON_

