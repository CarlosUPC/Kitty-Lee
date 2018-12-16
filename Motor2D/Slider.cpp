#include "j1App.h"
#include "Button.h"
#include "j1Gui.h"
#include "Slider.h"


Slider::Slider(const int &x, const int &y, const SDL_Rect &slider_rect, UIElement* parent) : UIElement(SLIDER, x, y, parent, true, slider_rect.w, slider_rect.h)
{
	position = { x,y,slider_rect.w,slider_rect.h };
	image = slider_rect;
	value = 0.0F;
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
	LOG("thumb%i position%i\n value %f", thumb->position.x, position.w, value);
	value = (float)thumb->position.x / (float)position.w;
}

float Slider::GetSliderValue() const
{
	return value;
}

Button * Slider::GetSliderButton() const
{
	return thumb;
}