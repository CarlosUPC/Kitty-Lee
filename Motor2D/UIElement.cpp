#include "j1App.h"
#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Window.h"



void UIElement::Update() {

	/*iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	UIElement* elem_triggered = App->gui->GetElemOnMouse(mouse.x, mouse.y);

	if (elem_triggered != nullptr && this == elem_triggered) {*/
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

		if (current_state != MOUSE_ENTER && current_state != LEFT_CLICK_DOWN && current_state != RIGHT_CLICK_DOWN) {
			current_state = MOUSE_ENTER;
			for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
				module->data->UI_Events(this, current_state);
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			current_state = LEFT_CLICK_DOWN;
			for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
				module->data->UI_Events(this, current_state);
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
			current_state = RIGHT_CLICK_DOWN;
			for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
				module->data->UI_Events(this, current_state);
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			current_state = LEFT_CLICK_UP;
			for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
				module->data->UI_Events(this, current_state);
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) {
			current_state = RIGHT_CLICK_UP;
			for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
				module->data->UI_Events(this, current_state);
			}
		}

	
	

	else {

		if (current_state == MOUSE_ENTER || current_state == LEFT_CLICK_DOWN)
		{
			current_state = MOUSE_LEAVE;
			for (p2List_item<j1Module*>* module = this->GetFirstListener(); module; module = module->next) {
				module->data->UI_Events(this, current_state);
			}

		}
		else if (current_state == MOUSE_LEAVE)
		{
			current_state = MOUSE_IDLE;

		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && (mouse.x != last_mouse.x || mouse.y != last_mouse.y)) {
		if (draggable) {

			int x_motion = mouse.x - last_mouse.x, y_motion = mouse.y - last_mouse.y;
			SetPos(GetLocalPosition().x + x_motion * App->win->GetScale(), GetLocalPosition().y + y_motion * App->win->GetScale());
		}
	}
	last_mouse = mouse;

}