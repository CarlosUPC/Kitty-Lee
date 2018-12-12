#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1App.h"

class Image : public UIElement {
public:

	//------------------------------Constructor & Destructor Function--------------------------------//
	Image() : UIElement(IMAGE, 0, 0, nullptr) {}
	Image(int pos_x, int pos_y, const SDL_Rect &image, UIElement* parent) : UIElement(IMAGE, pos_x, pos_y, parent, true, image.w, image.h) {}
	~Image() {}
	//------------------------------Constructor & Destructor Function--------------------------------//


	//--------------------Draw Function--------------------//
	void InnerDraw() {
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &image_rect, 0.0F, false, SDL_FLIP_NONE, true);
	}
	//--------------------Draw Function--------------------//


	//--------------------Scroll Function--------------------//
	void Scroll(char dir, float percentage) {
		if (dir == 'h') {
			draw_offset.x = -(image_rect.w - position.w)*percentage;
		}
		if (dir == 'v') {
			draw_offset.y = -(image_rect.h - position.h)*percentage;
		}
	}
	//--------------------Scroll Function--------------------//


	//-------------Factory Functions--------------//
	void SetRect(SDL_Rect img_rect) {
		image_rect = img_rect;
		if (position.w == 0 && position.h == 0) {
			position.w = img_rect.w;
			position.h = img_rect.h;
		}
	}
	//-------------Factory Functions--------------//


protected:
	SDL_Rect image_rect = { 0,0,0,0 };
};
#endif
