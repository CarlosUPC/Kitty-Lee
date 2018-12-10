#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
struct _TTF_Font;

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

	enum class Mouse {
		IDLE,
		ONHOVER,
		PUSH
	};

	virtual bool Draw() { return false; };
	fPoint position;
	int width = 0;
	int height = 0;
	UI::Type type;
	Mouse mouse = Mouse::IDLE;
};

class Image :public UI {
public:
	Image(const fPoint &pos, const SDL_Rect &r) :UI(pos), dimension(r) { width = r.w; height = r.h; }

	bool Draw();

	SDL_Rect dimension;
};

class Button :public UI {
public:
	Button(const fPoint &position, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push) :UI(position), idle(idle), hover(hover), push(push) { width = idle.w; height = idle.h; }
	~Button() {}

	bool Draw();
	
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
		App->fonts->CalcSize(txt, width, height, font);
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

	void CheckMouse(UI*);
	UI* Select()const;
	const SDL_Texture* GetAtlas() const;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<UI*> objects;
	UI* selected = nullptr; 
};

#endif // __j1GUI_H__