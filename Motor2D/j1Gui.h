#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
struct _TTF_Font;

enum Mouse_Event {
	MOUSE_ENTER,
	MOUSE_LEAVE,
	RIGHT_CLICK,
	LEFT_CLICK,
	RIGHT_CLICK_UP,
	LEFT_CLICK_UP,
	TAB,
	NONE
};


class UI {
public:
	UI(const fPoint &pos) :position(pos) {}
	~UI() {}

	enum class Type {
		BUTTON,
		IMAGE,
		LABEL,

		MAX
	};

	virtual bool Draw() { return false; };
	fPoint position;
	UI::Type type;
};

class Image :public UI {
public:
	Image(const fPoint &pos, const SDL_Rect &r) :UI(pos), dimension(r) {}

	bool Draw();

	SDL_Rect dimension;
};
enum class Mouse {
	IDLE,
	ONHOVER,
	PUSH
};
class Button :public UI {
public:
	Button(const fPoint &position, const SDL_Rect &idle,const SDL_Rect &hover, const SDL_Rect &push) :UI(position), idle(idle), hover(hover), push(push) {}
	~Button() {}

	bool Draw();
	

	Mouse	 mouse = Mouse::IDLE;
	SDL_Rect idle;
	SDL_Rect hover;
	SDL_Rect push;
};

class Label :public UI {
public:
	Label(const fPoint &pos, const char* txt, const char* path_font, const uint &size) :UI(pos) {
		font = App->fonts->Load(path_font, size);
		text.create(txt);
		SDL_Color color = { 255, 255, 255, 255 };
		texture = App->fonts->Print(text.GetString(), color, font);
	}
	~Label() {}

	bool Draw();

	_TTF_Font*		font;
	uint			size;
	SDL_Texture*	texture;
	p2SString		text;
};

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

	// Gui creation functions
	Button* CreateButton(const fPoint &pos, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push);
	Image * CreateImage(const fPoint & pos, const SDL_Rect & rect);
	Label * CreateLabel(const fPoint & pos, const char * text, const uint &size = DEFAULT_FONT_SIZE, const char * font = DEFAULT_FONT);

	bool DestroyUI(UI*);

	void CheckMouse(Button*);
	const SDL_Texture* GetAtlas() const;

	void UI_Events(UIElement* element, Mouse_Event action);

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<UI*> objects;
};

#endif // __j1GUI_H__