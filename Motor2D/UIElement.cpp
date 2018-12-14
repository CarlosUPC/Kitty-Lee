#include "j1App.h"
#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"



UIElement::UIElement(UI_type type, const int & pos_x, const int & pos_y, UIElement * parent, bool interactable, bool draggable, const int & width, const int & height, bool drawable) 
					: type(type), parent(parent), interactable(interactable), draggable(draggable), drawable(drawable), position({ pos_x, pos_y, width, height })
{
	current_state = NONE;
	if (parent != nullptr) {
		parent->childs.add(this);
	}
	//App->render->SetViewPort(viewport);
}

void UIElement::Draw()
{

	draw_offset.x = position.x;
	draw_offset.y = position.y;

	if (parent != nullptr) {
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

void UIElement::Update() {

	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	for (p2List_item<j1Module*>* module = listeners.start; module; module = module->next) {
		module->data->UI_Events(this);
	}
	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && (mouse.x != last_mouse.x || mouse.y != last_mouse.y)) {
		if (draggable) {

			int x_motion = mouse.x - last_mouse.x, y_motion = mouse.y - last_mouse.y;
			SetPos(GetLocalPosition().x + x_motion * App->win->GetScale(), GetLocalPosition().y + y_motion * App->win->GetScale());
		}
	}
	last_mouse = mouse;

}

void UIElement::SetPos(const int & x, const int & y)
{
	position.x = x;
	position.y = y;
}

int UIElement::GetPriority() const
{
	return priority;
}

iPoint UIElement::GetGlobalPosition() const
{
	return draw_offset;
}

iPoint UIElement::GetLocalPosition() const
{
	return{ position.x,position.y };
}

void UIElement::SetPosRespectParent(Position_Type pos, const int& margin)
{
	int x = 0, y = 0;
	switch (pos)
	{
	case CENTERED:
		x = (parent->position.w - position.w) / 2;
		y = (parent->position.h - position.h) / 2;
		break;
	case CENTERED_UP:
		x = (parent->position.w - position.w) / 2;
		y = margin;
		break;
	case CENTERED_DOWN:
		x = (parent->position.w - position.w) / 2;
		y = parent->position.h - position.h - margin;
		break;
	case LEFT_CENTERED:
		x = margin;
		y = (parent->position.h - position.h) / 2;
		break;
	case LEFT_UP:
		x = margin;
		y = margin;
		break;
	case LEFT_DOWN:
		x = margin;
		y = parent->position.h - position.h - margin;
		break;
	case RIGHT_CENTERED:
		x = parent->position.w - position.w - margin;
		y = (parent->position.h - position.h) / 2;
		break;
	case RIGHT_UP:
		x = parent->position.w - position.w - margin;
		y = margin;
		break;
	case RIGHT_DOWN:
		x = parent->position.w - position.w - margin;
		y = parent->position.h - position.h - margin;
		break;
	default:
		x = (parent->position.w - position.w) / 2;
		y = (parent->position.h - position.h) / 2;
		break;
	}
	SetPos(x, y);
}

void UIElement::DebugDraw()
{
	App->render->DrawQuad({ draw_offset.x,draw_offset.y,position.w,position.h }, 255U, 0U, 0U, 255U, false, false);
}

void UIElement::AddListener(j1Module * module)
{
	if (listeners.find(module) == -1) {
		listeners.add(module);
	}
}