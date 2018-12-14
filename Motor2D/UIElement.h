#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__

#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"

class j1Module;

enum UI_type {
	CHECKBOX,
	INPUT_TEXT,
	SCROLLBAR,
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

class UIElement {

public:

	UIElement() : type(UNKNOW) {}
	UIElement(UI_type type, const int &pos_x, const int &pos_y, UIElement* parent, bool interactable = true, bool draggable = false, const int &width = 0, const int &height = 0, bool drawable = true);
	~UIElement() {}

	void Draw();

	virtual void InnerDraw() {}
	virtual void Update();
	virtual void CleanUp() {}

	virtual void Scroll(char dir, float percentage) {}

	void SetPos(const int &x, const int &y);

	UI_type GetType()const { return type; }
	int GetPriority() const;
	iPoint GetGlobalPosition() const;
	iPoint GetLocalPosition() const;
	void SetPosRespectParent(Position_Type, const int& margin = 0);


	void SetPos(int x, int y) {
		position.x = x;
		position.y = y;

	}

	int GetPriority()const {
		return priority;
	}

	iPoint GetGlobalPosition()const {
		return draw_offset;
	}

	iPoint GetLocalPosition()const {
		return{ position.x,position.y };
	}

	//-------------Factory Functions--------------//



	//-------------Debug Functions--------------//
	void DebugDraw() {
		App->render->DrawQuad({ draw_offset.x,draw_offset.y,position.w,position.h }, 255U, 0U, 0U, 255U, false, false);
	}
	//-------------Debug Functions--------------//



	//-------------Application Functions--------------//
	void AddListener(j1Module* module) {
		if (listeners.find(module) == -1) {
			listeners.add(module);
		}
	}

	void DeleteListener(j1Module* module) {

		if (listeners.find(module) != -1) {
			listeners.clear();
		}
		/*for (p2List_item<j1Module*>* item = listeners.start; item; item = item->next) {

			RELEASE(item->data);
		}
		listeners.clear();*/
	}

	p2List_item<j1Module*>* GetFirstListener() {
		return listeners.start;
	}

	p2List_item<j1Module*>* GetLastListener() {
		return listeners.end;
	}
	//-------------Application Functions--------------//


	void DebugDraw();


	void AddListener(j1Module* module);
	p2List_item<j1Module*>* GetFirstListener();
	p2List_item<j1Module*>* GetLastListener();

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

	//SDL_Rect viewport = { 0,0,0,0 };

	int priority = 0;

	p2List<j1Module*> listeners;
};

#endif
