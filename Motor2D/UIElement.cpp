#include "j1App.h"
#include "UIElement.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Textures.h"



void UIElement::Update() {

	/*int mouse_x = 0, mouse_y = 0;
	App->input->GetWorldMousePosition(mouse_x, mouse_y);*/

	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	UIElement* elem_triggered = nullptr;
	elem_triggered = App->gui->GetElemOnMouse(mouse.x*3, mouse.y*3);

	if (elem_triggered != nullptr && this == elem_triggered) {

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


	/*iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);*/

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && (mouse.x != last_mouse.x || mouse.y != last_mouse.y)) {
		if (elem_triggered != nullptr && this->draggable) {

			int x_motion = mouse.x - last_mouse.x, y_motion = mouse.y - last_mouse.y;
			elem_triggered->SetPos(elem_triggered->GetLocalPosition().x + x_motion, elem_triggered->GetLocalPosition().y + y_motion);
		}
	}
	last_mouse = mouse;

}