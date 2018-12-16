#include "CheckBox.h"



CheckBox::CheckBox(const int &x, const int &y, const SDL_Rect &idle, const SDL_Rect &hover, const SDL_Rect &push, UIElement* parent) :UIElement(CHECKBOX, x, y, parent, true, idle.w, idle.h) {
	position = { x,y,idle.w,idle.h };
	idle_rect = idle;
	hovered_rect = hover;
	clicked_rect = push;

}


void CheckBox::InnerDraw()
{
	if (drawable) {
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &idle_rect,0.0F, false, SDL_FLIP_NONE, true);

		if (box_clicked) {
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x + (position.w - clicked_rect.w) / 2, draw_offset.y + (position.h - clicked_rect.h) / 2, &clicked_rect, 0.0F, false, SDL_FLIP_NONE, true);
		}
	}
}



void CheckBox::AddOptions(CheckBox* option) {
	if (options.find(option) == -1) {
		options.add(option);
	}
	if (option->options.find(this) == -1) {
		option->options.add(this);
	}
}
void CheckBox::Clicked() {
	box_clicked = !box_clicked;
	if (is_option) {
		for (p2List_item<CheckBox*>* item = options.start; item; item = item->next) {
			item->data->box_clicked = false;
		}
	}
}

bool CheckBox::GetIsClicked()const {
	return box_clicked;
}

void CheckBox::SetRects(const SDL_Rect &std_rect, const SDL_Rect &hl_rect, const SDL_Rect &click_rect)
{
	idle_rect = std_rect;
	hovered_rect = hl_rect;
	clicked_rect = click_rect;

	if (position.w == 0 && position.h == 0) {
		position.w = std_rect.w;
		position.h = std_rect.h;
	}
}