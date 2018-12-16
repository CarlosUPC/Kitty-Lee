#include "Image.h"

Image::Image(int pos_x, int pos_y, const SDL_Rect & image, UIElement * parent, bool inter, bool drag, bool draw, bool clip)
	: UIElement(IMAGE, pos_x, pos_y, parent, inter, drag, image.w, image.h, draw, clip) {
	position = { pos_x,pos_y,image.w,image.h };
	image_rect = image;
}

void Image::InnerDraw()
{
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &image_rect, 0.0F, false, SDL_FLIP_NONE, true, 
						clipable, { parent->GetGlobalPosition().x, parent->GetGlobalPosition().y, parent->position.w,parent->position.h });
}

void Image::Scroll(char dir, float percentage)
{
	if (dir == 'h') {
		draw_offset.x = -(image_rect.w - position.w)*percentage;
	}
	if (dir == 'v') {
		draw_offset.y = -(image_rect.h - position.h)*percentage;
	}
}

void Image::SetRect(SDL_Rect img_rect)
{
	image_rect = img_rect;
	if (position.w == 0 && position.h == 0) {
		position.w = img_rect.w;
		position.h = img_rect.h;
	}
}