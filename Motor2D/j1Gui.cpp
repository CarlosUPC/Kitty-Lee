#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UIElement.h"
#include "Image.h"
#include "Label.h"
#include "Button.h"


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{

	for (int i = 0; i < ui_elements.Count(); i++)
		if (ui_elements.At(i) != nullptr) ui_elements[i]->Update();
	
	//----------------------------------------------------------------
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {

		CheckMouse(item->data);
	}
	if (App->input->GetMouseButtonDown(1) == j1KeyState::KEY_DOWN && selected == nullptr)
		selected = Select();
	else if (App->input->GetMouseButtonDown(1) == j1KeyState::KEY_UP && selected != nullptr)
		selected = nullptr;
	LOG("%i", selected);

	return true;
}

bool j1Gui::PostUpdate()
{
	for (int i = ui_elements.Count() - 1; i >= 0; i--) {
		if (ui_elements.At(i) != nullptr) {
			if (ui_elements[i]->to_delete) {

				delete(ui_elements[i]);
				ui_elements[i] = nullptr;

			}
			else
				ui_elements[i]->Draw();
		}
	}

	//----------------------------------------------------------------
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
		item->data->Draw();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	for (uint i = 0; i < ui_elements.Count(); ++i)
	{
		if (ui_elements[i] != nullptr)
		{
			delete ui_elements[i];
			ui_elements[i] = nullptr;
			
		}
	}

	ui_elements.Clear();

	//----------------------------------------------------------------
	p2List_item<UI*>* item;
	for (item = objects.start; item; item = item->next) {
		RELEASE(item->data);
	}

	return true;
}

bool j1Gui::DeleteUIElement(UIElement &element) {

	for (int i = 0; i < ui_elements.Count(); i++) {
		if (*ui_elements.At(i) == &element) {
			ui_elements[i]->to_delete = true;
			return true;
		}
	}

	return false;
}

bool j1Gui::DeleteAllUIElements() {
	bool ret = false;

	for (int i = 0; i < ui_elements.Count(); i++) {
		if (ui_elements.At(i) != nullptr) ui_elements[i]->to_delete = true;
		ret = true;
	}

	return ret;
}

UIElement* j1Gui::GetElemOnMouse(int x, int y)
{
	//ui_elements.Flip();
	for (int i = 0; i < ui_elements.Count(); i++) {
		if (ui_elements[i] != nullptr && ui_elements[i]->interactable)
		{
			if ((x > ui_elements[i]->GetPosition().x && x < ui_elements[i]->GetPosition().x + ui_elements[i]->position.w) && (y > ui_elements[i]->GetPosition().y && y < ui_elements[i]->GetPosition().y + ui_elements[i]->position.h))
			{
				UIElement* tmp_element_to_return = ui_elements[i];
				//ui_elements.Flip();
				return tmp_element_to_return;
			}
		}
	}
	//ui_elements.Flip();
	return nullptr;
}

UIElement * j1Gui::CreateUIElement(UI_type type, int pos_x, int pos_y, int w, int h, UIElement* parent)
{
	UIElement* element = nullptr;

	switch (type)
	{
	case CHECKBOX:
		break;
	case INPUT_TEXT:
		break;
	case BUTTON:
		element = new Button(pos_x, pos_y, w, h, parent);
		break;
	case LABEL:
		element = new Label(pos_x, pos_y, w, h, parent);
		break;
	case IMAGE:
		element = new Image(pos_x, pos_y, w, h, parent);
		break;
	case SCROLLBAR:
		
		break;
	case UNKNOW:
		break;
	default:
		break;
	}

	if (element != nullptr) {
		//element->AddListener(this);
		ui_elements.PushBack(element);
	}

	return element;
}


//UIButton * j1Gui::CreateButton(const fPoint & pos, const SDL_Rect & idle, const SDL_Rect & hover, const SDL_Rect & push)
//{
//	UIButton* ret = nullptr;
//	ret = new UIButton(pos, idle, hover, push);
//	objects.add(ret);
//	return ret;
//}
//
//UIImage* j1Gui::CreateImage(const fPoint & pos, const SDL_Rect & rect)
//{
//	UIImage* ret = nullptr;
//	ret = new UIImage(pos, rect);
//	objects.add(ret);
//	return ret;
//}
//
//UILabel* j1Gui::CreateLabel(const fPoint & pos, const char* text, const uint &size, const char* font)
//{
//	UILabel* ret = nullptr;
//	ret = new UILabel(pos, text, font, size);
//	objects.add(ret);
//	return ret;
//}

bool j1Gui::DestroyUI(UI *ui)
{
	bool ret = false;
	int index = objects.find(ui);
	if (index != -1)
		ret = objects.del(objects.At(index));
	return ret;
}

void j1Gui::CheckMouse(UI *b)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > b->position.x&&x<b->position.x + b->width &&
		y>b->position.y&&y < b->position.y + b->height) {
		if (App->input->GetMouseButtonDown(1)) {
			b->mouse = UI::Mouse::PUSH;

		}
		else {
			b->mouse = UI::Mouse::ONHOVER;
		}


	}
	else if (b->mouse != UI::Mouse::IDLE) {
		b->mouse = UI::Mouse::IDLE;

	}


}

UI* j1Gui::Select() const
{
	p2List_item<UI*>* item = objects.start;
	for (; item; item = item->next) {
		if (item->data->mouse == UI::Mouse::PUSH)
			return  item->data;
	}
}


void j1Gui::UI_Events(UIElement* element, Mouse_Event action){

	switch (action){
	case MOUSE_ENTER:
		
		break;
	case MOUSE_LEAVE:
		
		break;
	case RIGHT_CLICK_DOWN:
		
		break;
	case LEFT_CLICK_DOWN:
		
		break;
	case RIGHT_CLICK_UP:
		
		break;
	case LEFT_CLICK_UP:
		
		break;
	case TAB:
		break;
	case NONE:
		break;
	default:
		break;
	}
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

bool UIImage::Draw()
{
	bool ret = false;

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &dimension, 0.0F);

	return ret;
}

bool UIButton::Draw()
{
	bool ret = false;
	switch (mouse) {
	case Mouse::ONHOVER:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &hover, 0.0f);
		break;
	case Mouse::IDLE:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &idle, 0.0f);
		break;
	case Mouse::PUSH:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, 0.0f);
		break;
	}
	return ret;
}

bool UILabel::Draw()
{
	App->render->Blit(texture, position.x, position.y, NULL, 0.0F, false);
	return false;
}
