#include "Label.h"
#include "j1Textures.h"

Label::Label(int pos_x, int pos_y, const char * txt, const Color & c, const char * path_font, const uint & size, UIElement * parent, bool interactable, bool draggable, uint32 wrap_length)
	: UIElement(LABEL, pos_x, pos_y, parent, interactable, draggable, true)
{
	font = App->fonts->Load(path_font, size);
	text.create(txt);

	SetColor(c);
	if(wrap_length = 0)
		texture = App->fonts->Print(text.GetString(), color, font);
	else texture = App->fonts->PrintWrapped(text.GetString(), color, font, wrap_length);

	App->fonts->CalcSize(txt, position.w, position.h, font);
}

void Label::InnerDraw()
{
	if (drawable)
		App->render->Blit(texture, draw_offset.x, draw_offset.y, NULL, 0.0F, false, SDL_FLIP_NONE, true);
}

void Label::SetText(const char * txt)
{
	text.create(txt);
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(text.GetString(), color, font);
	App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
}

void Label::SetColor(Color c)
{
	switch (c) {
	case RED:
		color = { 255,0,0,color.a };
		break;
	case GREEN:
		color = { 0,255,0,color.a };
		break;
	case BLUE:
		color = { 0,0,255,color.a };
		break;
	case YELLOW:
		color = { 255,255,0,color.a };
		break;
	case GREY:
		color = { 150,150,150,color.a };
		break;
	case BLACK:
		color = { 0,0,0,color.a };
		break;
	case WHITE:
		color = { 255,255,255,color.a };
		break;
	default:
		color = { 255,255,255,color.a };
		break;
	}
	texture = App->fonts->Print(text.GetString(), color, font);
}

void Label::SetColor(const SDL_Color & c)
{
	color = c;
	texture = App->fonts->Print(text.GetString(), color, font);
}

void Label::ChangeFont(const char * f, const int & size)
{
	font = App->fonts->Load(f, size);
	texture = App->fonts->Print(text.GetString(), color, font);
	App->fonts->CalcSize(text.GetString(), position.w, position.h, font);
}