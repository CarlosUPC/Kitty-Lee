#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "p2DynArray.h"
//#include "UIElement.h"

class UIElement;
class Button;
class Label;
class Image;

//class UI {
//public:
//	UI(const fPoint &pos) :position(pos) {}
//	~UI() {}
//
//	enum class Type {
//		BUTTON,
//		IMAGE,
//		LABEL,
//
//		MAX
//	};
//
//	enum class Mouse {
//		IDLE,
//		ONHOVER,
//		PUSH
//	};
//
//	virtual bool Draw() { return false; };
//	fPoint position;
//	int width = 0;
//	int height = 0;
//	UI::Type type;
//	Mouse mouse = Mouse::IDLE;
//};
//
//class UIImage :public UI {
//public:
//	UIImage(const fPoint &pos, const SDL_Rect &r) :UI(pos), dimension(r) { width = r.w; height = r.h; }
//
//	bool Draw();
//
//	SDL_Rect dimension;
//};
//
//class UIButton :public UI {
//public:
//	UIButton(const fPoint &position, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push) :UI(position), idle(idle), hover(hover), push(push) { width = idle.w; height = idle.h; }
//	~UIButton() {}
//
//	bool Draw();
//	
//	SDL_Rect idle;
//	SDL_Rect hover;
//	SDL_Rect push;
//};
//
//class UILabel :public UI {
//public:
//	UILabel(const fPoint &pos, const char* txt, const char* path_font, const uint &size) :UI(pos) {
//		font = App->fonts->Load(path_font, size);
//		text.create(txt);
//		SDL_Color color = { 255, 255, 255, 255 };
//		texture = App->fonts->Print(text.GetString(), color, font);
//		App->fonts->CalcSize(txt, width, height, font);
//	}
//	~UILabel() {}
//
//	bool Draw();
//
//	_TTF_Font*		font;
//	uint			size;
//	SDL_Texture*	texture;
//	p2SString		text;
//};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//bool DestroyUI(UI*);

	//void CheckMouse(UI*);
	//UI* Select()const;
	const SDL_Texture* GetAtlas() const;

	//----------------------------------------------------------------------------------------

	Button* CreateButton(const int &pos_x, const int &pos_y, const SDL_Rect &idle = { 0,0,0,0 }, const SDL_Rect &hover = { 0,0,0,0 }, const SDL_Rect &push = { 0,0,0,0 }, const UIElement* parent = nullptr);
	Image * CreateImage(const int &pos_x, const int &pos_y, const SDL_Rect & rect = { 0,0,0,0 }, const UIElement* parent = nullptr);
	Label * CreateLabel(const int &pos_x, const int &pos_y, const char* text, const uint &size = DEFAULT_FONT_SIZE, const char* font = DEFAULT_FONT, UIElement* parent = nullptr);

	bool DeleteUIElement(UIElement &element);
	bool DeleteAllUIElements();
	UIElement* GetElemOnMouse(int x, int y);
	void UI_Events(UIElement* element, Mouse_Event action);

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2DynArray<UIElement*> ui_elements;

	
};

#endif // __j1GUI_H__