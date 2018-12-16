#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__

#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"

class j1Module;

enum Position_Type {
	CENTERED,
	CENTERED_UP,
	CENTERED_DOWN,
	LEFT_CENTERED,
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_CENTERED,
	RIGHT_UP,
	RIGHT_DOWN,
};

enum UI_type {
	CHECKBOX,
	INPUT_TEXT,
	SLIDER,
	BUTTON,
	LABEL,
	IMAGE,
	WINDOW,
	UNKNOW
};

enum Mouse_Event {
	HOVER,
	CLICKED_DOWN,
	CLICKED_REPEAT,
	CLICKED_UP,

	NONE
};

class UIElement {

public:

	UIElement() : type(UNKNOW) {}
	UIElement(UI_type type, const int &pos_x, const int &pos_y, UIElement* parent, bool interactable = true, bool draggable = false, const int &width = 0, const int &height = 0, bool drawable = true);
	~UIElement() {}

	void Draw();

	virtual void InnerDraw() {}
	void Update();
	virtual void PostUpdate() {}
	virtual void CleanUp() {}

	virtual void Scroll(char dir, float percentage) {}

	void SetPos(const int &x, const int &y);

	UI_type GetType()const { return type; }
	int GetPriority() const;
	iPoint GetGlobalPosition() const;
	iPoint GetLocalPosition() const;
	void SetPosRespectParent(Position_Type, const int& margin = 0);

	void DebugDraw();

	void AddListener(j1Module* module);
	void DeleteListener(j1Module* module);

public:
	SDL_Rect position;

	bool interactable = true;
	bool draggable = true;
	bool drawable = true;
	
	bool to_delete = false;
	iPoint draw_offset = { 0,0 };

	UIElement* parent = nullptr;

	p2List<UIElement*> childs;

	Mouse_Event current_state = NONE;
	iPoint last_mouse;
	
private:
	UI_type type = UNKNOW;
	
	int priority = 0;

	p2List<j1Module*> listeners;
};

#endif
