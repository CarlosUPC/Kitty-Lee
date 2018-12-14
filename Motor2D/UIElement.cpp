#include "j1App.h"
#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"



void UIElement::Update() {

	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
		module->data->UI_Events(this,this->current_state);
	}
	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && (mouse.x != last_mouse.x || mouse.y != last_mouse.y)) {
		if (draggable) {

			int x_motion = mouse.x - last_mouse.x, y_motion = mouse.y - last_mouse.y;
			SetPos(GetLocalPosition().x + x_motion * App->win->GetScale(), GetLocalPosition().y + y_motion * App->win->GetScale());
		}
	}
	last_mouse = mouse;

}