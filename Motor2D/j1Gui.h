#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "p2DynArray.h"

class UIElement;
class Button;
class Label;
class Slider;
class Image;
class CheckBox;

enum class Slider_TYPE {
	X,
	Y
};

enum Color
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	GREY,
	BLACK,
	WHITE,

	COLOR_NOT_DEF
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

	void CreateScreen();

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	const SDL_Texture* GetAtlas() const;

	//----------------------------------------------------------------------------------------
	CheckBox* CreateCheckBox(const int &pos_x, const int &pos_y, const SDL_Rect &idle = { 0,0,0,0 }, UIElement* parent = nullptr, const SDL_Rect &hover = { 0,0,0,0 }, const SDL_Rect &push = { 0,0,0,0 });
	Button* CreateButton(const int &pos_x, const int &pos_y, const SDL_Rect &idle = { 0,0,0,0 }, UIElement* parent = nullptr, const SDL_Rect &hover = { 0,0,0,0 }, const SDL_Rect &push = { 0,0,0,0 });
	Image * CreateImage(const int &pos_x, const int &pos_y, const SDL_Rect & rect = { 0,0,0,0 }, UIElement* parent = nullptr, bool interactable = false, bool draggable = false, bool drawable = true);
	Label * CreateLabel(const int &pos_x, const int &pos_y, const char* text, bool interactable = false, bool draggable = false, UIElement* parent = nullptr, Color color = WHITE, const uint &size = DEFAULT_FONT_SIZE, const char* font = DEFAULT_FONT, uint32 wrap_length = 0U);
	Slider * CreateSlider(const int &x, const int &y, const SDL_Rect &slider_rect, Slider_TYPE type, UIElement* parent = nullptr);

	bool DeleteUIElement(UIElement * element);
	void BFS(p2List<UIElement *> &visited, UIElement * elem);
	bool DeleteAllUIElements();
	bool GetElemOnMouse(int x, int y, UIElement* & element);
	bool CheckCollision(int x, int y, p2List_item<UIElement *> * item);
	void UI_Events(UIElement* element);

public:
	UIElement* screen = nullptr;
	bool ui_debug = false;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<UIElement*> ui_elements;

	
};

#endif // __j1GUI_H__