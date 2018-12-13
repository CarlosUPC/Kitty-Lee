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
#include "j1Window.h"
#include "p2Queue.h"


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

	screen = CreateImage(0, 0, { 0,0,(int)App->win->GetWindowWidth(),(int)App->win->GetWindowHeight() }, nullptr);

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
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	UIElement* element = GetElemOnMouse(mouse.x*App->win->GetScale(), mouse.y*App->win->GetScale());
	if (element != nullptr)
		element->Update();
	
	return true;
}

bool j1Gui::PostUpdate()
{
	p2List<UIElement*> visited;
	BFS(visited, screen);

	for (p2List_item<UIElement*>* item = visited.start; item; item = item->next) {
		item->data->Draw();
		LOG("%i", item->data->position.x);
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	for (p2List_item<UIElement*>* item = ui_elements.start; item; item = item->next) {

			RELEASE(item->data);
	}
	ui_elements.clear();

	return true;
}

bool j1Gui::DeleteUIElement(UIElement * element) {

	int index = ui_elements.find(element);
	if (index != -1){
		p2List_item<UIElement*>* elem = ui_elements.At(index);

		p2List<UIElement*> visited;
		BFS(visited, elem->data);

		for (p2List_item<UIElement*>* item = visited.end; item; item = item->prev) {
			if (item == visited.start) {
				index = item->data->parent->childs.find(item->data);
				if (index != -1) {
					item->data->parent->childs.del(item->data->parent->childs.At(index));
					delete item->data;
				}
			}
			else {
				index = ui_elements.find(item->data);
				ui_elements.del(ui_elements.At(index));
				delete item->data;
			}
		}
		visited.clear();
	
		return true;
	}

	LOG("Element not found to delete");

	return false;
}

UIElement* j1Gui::FindElement(UIElement *elem)
{
	int index = ui_elements.find(elem);
	if (index == -1) {
		LOG("Cannot find ui element");
	}
	else {
		return ui_elements.At(index)->data;
	}
	
	return nullptr;
}

void j1Gui::BFS(p2List<UIElement *> &visited, UIElement * elem) //It will fill a list from parent to children
{
	p2DynArray<UIElement*> frontier;
	UIElement* item = nullptr;
	visited.add(elem);					//Add from we want to start to visited and frontier list
	frontier.PushBack(elem);
	while (frontier.Count() > 0) {
		if (frontier.Pop(item)) {			//Pop las item of array
			for (p2List_item<UIElement*>*it = item->childs.start; it; it = it->next) { //iterate for all childs of node
				if (visited.find(it->data) == -1) {	//if child is not on visited list we added on it and on prontier to search its childs
					frontier.PushBack(it->data);
					visited.add(it->data);
				}
			}
		}

	}
}

bool j1Gui::DeleteAllUIElements() {
	bool ret = false;

	for (p2List_item<UIElement*>* item = ui_elements.start; item; item = item->next) {

		RELEASE(item->data);
	}
	ui_elements.clear();

	return ret;
}

UIElement* j1Gui::GetElemOnMouse(int x, int y)
{
	UIElement* ret = nullptr;
	for (p2List_item<UIElement*>* item = ui_elements.start; item; item = item->next) {
		if (item->data->interactable)
		{
			if ((x > item->data->GetPosition().x && x < item->data->GetPosition().x + item->data->position.w) &&
				(y > item->data->GetPosition().y && y < item->data->GetPosition().y + item->data->position.h))
			{
				App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) ? item->data->current_state = CLICKED : item->data->current_state = HOVER;

				ret = item->data;
			}
			else {
				item->data->current_state = NONE;
			}
		}
	}
	
	return ret;
}

Button * j1Gui::CreateButton(const int &pos_x, const int &pos_y, const SDL_Rect &idle, UIElement* parent, const SDL_Rect &hover, const SDL_Rect &push)
{
	Button* ret = nullptr;
	ret = new Button(pos_x, pos_y, idle, hover, push, parent);
	ui_elements.add(ret);
	return ret;
}

Image * j1Gui::CreateImage(const int & pos_x, const int & pos_y, const SDL_Rect & rect, UIElement * parent)
{
	Image* ret = nullptr;
	ret = new Image(pos_x, pos_y, rect, parent);
	ui_elements.add(ret);
	return ret;
}

Label* j1Gui::CreateLabel(const int &pos_x, const int &pos_y, const char* text, Color color, UIElement* parent, const uint &size, const char* font)
{
	Label* ret = nullptr;
	ret = new Label(pos_x, pos_y, text, color, font, size, parent);
	ui_elements.add(ret);
	return ret;
}

void j1Gui::UI_Events(UIElement* element, Mouse_Event action){

}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

//bool UIImage::Draw()
//{
//	bool ret = false;
//
//	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &dimension, 0.0F);
//
//	return ret;
//}
//
//bool UIButton::Draw()
//{
//	bool ret = false;
//	switch (mouse) {
//	case Mouse::ONHOVER:
//		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &hover, 0.0f);
//		break;
//	case Mouse::IDLE:
//		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &idle, 0.0f);
//		break;
//	case Mouse::PUSH:
//		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &push, 0.0f);
//		break;
//	}
//	return ret;
//}
//
//bool UILabel::Draw()
//{
//	App->render->Blit(texture, position.x, position.y, NULL, 0.0F, false);
//	return false;
//}
