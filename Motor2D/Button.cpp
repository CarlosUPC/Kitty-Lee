#include "Button.h"

Button::Button(const int & x, const int & y, const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push, UIElement * parent)
	: UIElement(BUTTON, x, y, parent, true, idle.w, idle.h) {
	position = { x,y,idle.w,idle.h };
	idle_rect = idle;
	hovered_rect = hover;
	clicked_rect = push;
}

void Button::InnerDraw()
{
	if (drawable)
		switch (current_state)
		{
		case HOVER:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &hovered_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		case CLICKED_DOWN:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &clicked_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		case CLICKED_REPEAT:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &clicked_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		case CLICKED_UP:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &hovered_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		case NONE:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &idle_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		default:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &idle_rect, 0.0F, false, SDL_FLIP_NONE, true);
			break;
		}
}

void Button::SetRects(const SDL_Rect &std_rect, const SDL_Rect &hl_rect, const SDL_Rect &click_rect)
{
	idle_rect = std_rect;
	hovered_rect = hl_rect;
	clicked_rect = click_rect;

	if (position.w == 0 && position.h == 0) {
		position.w = std_rect.w;
		position.h = std_rect.h;
	}
}