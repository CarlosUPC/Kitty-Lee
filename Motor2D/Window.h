#ifndef _WINDOW_
#define _WINDOW_

#include "Image.h"
#include "p2SString.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Gui.h"

class Window : public Image {

public:

	//------------------------------Constructor & Destructor Function--------------------------------//
	Window(int x, int y, int w, int h, UI* parent) : Image(WINDOW, x, y, parent, true, w, h) {}
	~Window() {}
	//------------------------------Constructor & Destructor Function--------------------------------//


	//--------------------Draw Function--------------------//
	void InnerDraw()
	{
		if (visible) {
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &image_rect, false);
		}
	}
	//--------------------Draw Function--------------------//


};
#endif // !_UIBUTTON_

