#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "UIElement.h"

class Image : public UIElement {
public:

	Image() : UIElement(IMAGE, 0, 0, nullptr) {}
	Image(int pos_x, int pos_y, const SDL_Rect &image, UIElement* parent, bool inter = true, bool drag = false, bool draw = true);
	~Image() {}

	void InnerDraw();

	void Scroll(char dir, float percentage);


	void SetRect(SDL_Rect img_rect);

protected:
	SDL_Rect image_rect = { 0,0,0,0 };
};
#endif
