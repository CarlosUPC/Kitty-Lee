#ifndef _LABEL_
#define _LABEL_

#include "UIElement.h"
#include "p2SString.h"
#include "j1App.h"
#include "j1Fonts.h"

class Label : public UIElement {
public:
	//------------------------------Constructor & Destructor Function--------------------------------//
	Label() : UIElement(LABEL, 0, 0, nullptr) {	}
	Label(int pos_x, int pos_y, int w, int h, UIElement* parent) : UIElement(LABEL, pos_x, pos_y, parent, true, w, h) {}
	Label(UI_type type, int pos_x, int pos_y, int w, int h, UIElement* parent) : UIElement(type, pos_x, pos_y, parent, true, w, h) {}
	~Label() {}
	//------------------------------Constructor & Destructor Function--------------------------------//



	//--------------------Draw Function--------------------//
	void InnerDraw() {
		if (label_tex != nullptr)
			SDL_DestroyTexture(label_tex);
		label_tex = App->fonts->Print(text.c_str());
		SDL_QueryTexture(label_tex, NULL, NULL, &label_rect.w, &label_rect.h);
		App->render->Blit(label_tex, draw_offset.x, draw_offset.y, &label_rect, false, false);

		/*App->render->SetViewPort({ GetPosition().x,GetPosition().y,position.w,position.h });
		App->render->Blit(text_texture, 0, 0, &texture_rect, false);*/

		//App->render->Blit(text_texture, position.x, position.y, &texture_rect, false);
		//App->render->Blit(App->font->Print(text.GetString()), position.x, position.y, NULL, false);
	}
	//--------------------Draw Function--------------------//



	//--------------------Scroll Function--------------------//
	void Scroll(char dir, float percentage) {
		if (dir == 'h') {
			draw_offset.x = -(label_rect.w - position.w)*percentage;
		}
		if (dir == 'v') {
			draw_offset.y = -(label_rect.h - position.h)*percentage;
		}
	}
	//--------------------Scroll Function--------------------//



	//-------------Factory Functions--------------//
	void SetText(char* txt) {
		text = txt;
	}
	//-------------Factory Functions--------------//

protected:
	std::string text = "";
	//p2SString text = "";

	SDL_Texture* label_tex;
	SDL_Rect label_rect = { 0, 0, 0, 0 };

};
#endif
