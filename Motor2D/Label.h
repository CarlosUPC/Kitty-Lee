#ifndef __LABEL_H__
#define __LABEL_H__

#include "UIElement.h"
#include "p2SString.h"
#include "j1App.h"
#include "j1Fonts.h"

class Label : public UIElement {
public:
	//------------------------------Constructor & Destructor Function--------------------------------//
	Label() : UIElement(LABEL, 0, 0, nullptr) {	}
	Label(int pos_x, int pos_y, const char* txt, const Color &c, const char* path_font, const uint &size = 12, UIElement* parent = nullptr, bool interactable = false, bool draggable = false) : UIElement(LABEL, pos_x, pos_y, parent, interactable, draggable, true) {
		font = App->fonts->Load(path_font, size);
		text.create(txt);
		
		SetColor(c);
		
		texture = App->fonts->Print(text.GetString(), color, font);
		App->fonts->CalcSize(txt, position.w, position.h, font);
	}
	~Label() {}
	//------------------------------Constructor & Destructor Function--------------------------------//



	//--------------------Draw Function--------------------//
	void InnerDraw() {

		if(drawable)
			App->render->Blit(texture, draw_offset.x, draw_offset.y, NULL, 0.0F, false, SDL_FLIP_NONE, true);
		/*if (label_tex != nullptr)
			SDL_DestroyTexture(label_tex);
		label_tex = App->fonts->Print(text.c_str());
		SDL_QueryTexture(label_tex, NULL, NULL, &label_rect.w, &label_rect.h);
		App->render->Blit(label_tex, draw_offset.x, draw_offset.y, &label_rect, false, false);*/

		/*App->render->SetViewPort({ GetPosition().x,GetPosition().y,position.w,position.h });
		App->render->Blit(text_texture, 0, 0, &texture_rect, false);*/

		//App->render->Blit(text_texture, position.x, position.y, &texture_rect, false);
		//App->render->Blit(App->font->Print(text.GetString()), position.x, position.y, NULL, false);
	}
	//--------------------Draw Function--------------------//


	//--------------------Scroll Function--------------------//
	//void Scroll(char dir, float percentage) {
	//	if (dir == 'h') {
	//		draw_offset.x = -(label_rect.w - position.w)*percentage;
	//	}
	//	if (dir == 'v') {
	//		draw_offset.y = -(label_rect.h - position.h)*percentage;
	//	}
	//}
	//--------------------Scroll Function--------------------//



	//-------------Factory Functions--------------//
	void SetText(const char* txt) {
		text.create(txt);
		texture = App->fonts->Print(text.GetString(), color, font);
		App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
	}

	void SetColor(Color c) {
		switch (c) {
		case RED:
			color = { 255,0,0,color.a };
			break;
		case GREEN:
			color = { 0,255,0,color.a };
			break;
		case BLUE:
			color = { 0,0,255,color.a };
			break;
		case YELLOW:
			color = { 255,255,0,color.a };
			break;
		case GREY:
			color = { 150,150,150,color.a };
			break;
		case BLACK:
			color = { 0,0,0,color.a };
			break;
		case WHITE:
			color = { 255,255,255,color.a };
			break;
		default:
			color = { 255,255,255,color.a };
			break;
		}
		texture = App->fonts->Print(text.GetString(), color, font);
	}

	void SetColor(const SDL_Color &c) {
		color = c;
		texture = App->fonts->Print(text.GetString(), color, font);
	}

	/*void SetAlpha(const Uint8 &alpha) {
		color = { color.r,color.g,color.b,alpha };
		texture = App->fonts->Print(text.GetString(), color, font);
	}*/

	void ChangeFont(const char* f, const int &size) {
		font = App->fonts->Load(f, size);
		texture = App->fonts->Print(text.GetString(), color, font);
		App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
	}

	/*void ChangeFont(const int &index) {
		font = App->fonts->Load(App->fonts->fonts.At(index), size);
		texture = App->fonts->Print(text.GetString(), color, font);
		App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
	}*/
	//-------------Factory Functions--------------//

protected:

	_TTF_Font*		font = nullptr;
	uint			size = 0;
	SDL_Texture*	texture = nullptr;
	SDL_Color		color = { 255,255,255,255 };
	p2SString		text;

};
#endif
