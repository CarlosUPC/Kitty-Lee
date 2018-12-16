#ifndef __LABEL_H__
#define __LABEL_H__

#include "UIElement.h"

class Label : public UIElement {
public:

	Label() : UIElement(LABEL, 0, 0, nullptr) {	}
	Label(int pos_x, int pos_y, const char* txt, const Color &c, const char* path_font, const uint &size = 12, UIElement* parent = nullptr, bool interactable = false, bool draggable = false,uint32 wrap_length = 0);
	~Label() {}

	void InnerDraw();

	void SetText(const char* txt);

	void SetColor(Color c);

	void SetColor(const SDL_Color &c);

	/*void SetAlpha(const Uint8 &alpha) {
		color = { color.r,color.g,color.b,alpha };
		texture = App->fonts->Print(text.GetString(), color, font);
	}*/

	void ChangeFont(const char* f, const int &size);

protected:

	_TTF_Font*		font = nullptr;
	uint			size = 0;
	SDL_Texture*	texture = nullptr;
	SDL_Color		color = { 255,255,255,255 };
	p2SString		text;

};
#endif
