#ifndef _UIELEMENT_
#define _UIELEMENT_

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

class UIElement {

public:

	//------------------------------Constructor Function--------------------------------//
	UIElement() : type(UNKNOW) {}
	UIElement(UI_type type, const int &pos_x, const int &pos_y, UIElement* parent, bool interactable = true, bool draggable = false, const int &width = 0, const int &height = 0, bool drawable = true) : type(type), parent(parent), interactable(interactable), draggable(draggable), drawable(drawable), position({ pos_x, pos_y, width, height }) {
		current_state = NONE;
		if (parent != nullptr) {
			parent->childs.add(this);
		}
		//App->render->SetViewPort(viewport);
	}
	~UIElement() {}
	//------------------------------Constructor Functions--------------------------------//


	//------------------------------Draw Function--------------------------------//
	void Draw()
	{

		draw_offset.x = position.x;
		draw_offset.y = position.y;

		if(parent!=nullptr){
			for (UIElement* p = parent; p; p = p->parent) {
				draw_offset.x += p->position.x;
				draw_offset.y += p->position.y;
			}
		}
		/*
		//check element is inside parent boundaries
		if (position.x < 0) position.x = 0;
		if (position.y < 0)position.y = 0;
		if (GetPosition().x + position.w > viewport.x + viewport.w) position.x = viewport.w - position.w;
		if (GetPosition().y + position.h > viewport.y + viewport.h) position.y = viewport.h - position.h;
		*/
		if (App->gui->ui_debug)
			DebugDraw();

		//App->render->SetViewPort({ GetPosition().x,GetPosition().y,position.w,position.h });
		InnerDraw();
		//App->render->ResetViewPort();
	}
	//------------------------------Draw Function--------------------------------//



	//-------------Virtual Functions--------------//

	virtual void InnerDraw() {}

	virtual void Update();
		
	virtual void CleanUp() {}

	virtual void Scroll(char dir, float percentage) {}
	//-------------Virtual Functions--------------//



	//-------------Factory Functions--------------//
	UI_type GetType()const { return type; }

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

	p2List_item<j1Module*>* GetFirstListener() {
		return listeners.start;
	}

	p2List_item<j1Module*>* GetLastListener() {
		return listeners.end;
	}
	//-------------Application Functions--------------//



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
