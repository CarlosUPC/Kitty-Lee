#include "Slider.h"
#include "j1App.h"
#include "Button.h"
#include "j1Gui.h"


Slider::Slider(const int &x, const int &y, const SDL_Rect &slider_rect, UIElement* parent, Slider_TYPE slider_type) : UIElement(SLIDER, x, y, parent, true, slider_rect.w, slider_rect.h)
{
	position = { x,y,slider_rect.w,slider_rect.h };
	image = slider_rect;
	value = 0.0F;
	type = slider_type;
	draggable = false;
	interactable = false;
}


void Slider::AddTargets(UIElement * target)
{
	control.add(target);
}

void Slider::AddThumb(Button *thmb)
{
	if (thumb == nullptr) {
		thumb = thmb;
	}
	else {
		thumb->to_delete = true;
	}
}

void Slider::SetSliderValueStart(float slider_value) {
	
	if (slider_value >= 0.0f && slider_value <= 1.0f) {
		value = slider_value;
	}
	else {
		value = 0;
	}

	thumb->SetPos(position.x * value, position.y * value);
}

void Slider::InnerDraw()
{
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), draw_offset.x, draw_offset.y, &image, 0.0f, false, SDL_FLIP_NONE, true);
}

void Slider::PostUpdate()
{
	if (thumb->position.x < 0)
		thumb->position.x = 0;

	if (thumb->position.y < 0)
		thumb->position.y = 0;

	if (thumb->position.x > position.w - thumb->position.w)
		thumb->position.x = position.w - thumb->position.w;

	if (thumb->position.y > position.h - thumb->position.h)
		thumb->position.y = position.h - thumb->position.h;

	if (type == Slider_TYPE::X)
		value = (float)thumb->position.x / (float)position.w;
	else
		value = (float)thumb->position.y / (float)position.h;
}

float Slider::GetSliderValue() const
{
	return value;
}

Button * Slider::GetSliderButton() const
{
	return thumb;
}