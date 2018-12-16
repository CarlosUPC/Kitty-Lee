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
#include "Slider.h"
#include "CheckBox.h"
#include "j1Window.h"


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

	CreateScreen();

	return ret;
}

void j1Gui::CreateScreen()
{
	if (ui_elements.find(screen) == -1)
		screen = CreateImage(0, 0, { 0,0,(int)App->win->GetWindowWidth(),(int)App->win->GetWindowHeight() }, nullptr, false, false, false);
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
	UIElement* element = nullptr;
	if (GetElemOnMouse(mouse.x*App->win->GetScale(), mouse.y*App->win->GetScale(), element))
 		element->Update();

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		ui_debug = !ui_debug;
	
	return true;
}

bool j1Gui::PostUpdate()
{
	for (int i = 0; i < ui_elements.count(); ++i) {
		if (ui_elements[i]->to_delete)
			DeleteUIElement(ui_elements[i]);
		else {
			ui_elements[i]->PostUpdate();
		}
	}

	p2List<UIElement*> visited;
	BFS(visited, screen);
	
	for (p2List_item<UIElement*>* item = visited.start; item; item = item->next) {
		if (item->data->drawable)
			item->data->Draw();
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
	if (index != -1){															//if element doesn't find in ui list it cannot be deleted
		p2List_item<UIElement*>* elem = ui_elements.At(index);

		p2List<UIElement*> tree;
		BFS(tree, elem->data);		//fills a list from element to delete to its childs using BFS algorithm

		for (p2List_item<UIElement*>* item = tree.end; item; item = item->prev) {	//iterate list from bottom to top
				if (item == tree.start && item->data->parent != nullptr) {				/*In case the item we will delete is the first element of the tree
																						  we have to delete him first from its parent child list
																						  the reason why we don't made that for other nodes is becuase
																						  other nodes and its parents will be deleted for complete*/
					index = item->data->parent->childs.find(item->data);
					if (index != -1) {
						item->data->parent->childs.del(item->data->parent->childs.At(index));
					}
				}
				index = ui_elements.find(item->data);	//find item on ui objects list
				if (index != -1) {						//if it is valid
					ui_elements.del(ui_elements.At(index)); //delete from list
					//delete item->data;						//and deallocate memory
					item->data = nullptr;
				}
		}
		tree.clear();
	
		return true;
	}

	LOG("Element not found to delete");

	return false;
}

void j1Gui::BFS(p2List<UIElement *> &visited, UIElement * elem) //It will fill a list from parent to children
{
	if (elem != nullptr) {
		p2DynArray<UIElement*> frontier;
		UIElement* item = nullptr;
		visited.add(elem);					//Add from we want to start to visited and frontier list
		frontier.PushBack(elem);
		while (frontier.Count() > 0) {
			if (frontier.Pop(item)) {			//Pop las item of array
				if(item != nullptr)
					for (p2List_item<UIElement*>*it = item->childs.start; it; it = it->next) { //iterate for all childs of node
						if (visited.find(it->data) == -1) {	//if child is not on visited list we added on it and on prontier to search its childs
							frontier.PushBack(it->data);
							visited.add(it->data);
						}
					}
			}

		}
	}
}

bool j1Gui::DeleteAllUIElements() {
	bool ret = false;
	
	DeleteUIElement(screen);

	return ret;
}

bool j1Gui::GetElemOnMouse(int x, int y, UIElement* & element)
{
	p2List<UIElement*> tree;
	BFS(tree, screen);

	for (p2List_item<UIElement*>* item = tree.end; item; item = item->prev) {
		if (item->data->interactable)
		{
			if (CheckCollision(x, y, item))
			{
				if (item->data->current_state != CLICKED_DOWN && item->data->current_state != CLICKED_REPEAT)
					App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) ? item->data->current_state = CLICKED_DOWN : item->data->current_state = HOVER;
				else {
					App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) ? item->data->current_state = CLICKED_REPEAT : item->data->current_state = CLICKED_UP;
				}
				element = item->data;
				return true;
			}
			else {
				item->data->current_state = NONE;
			}
		}
	}
	
	return false;
}

bool j1Gui::CheckCollision(int x, int y, p2List_item<UIElement *> * item)
{
	iPoint pos = item->data->GetGlobalPosition();
	return (x > pos.x && x < pos.x + item->data->position.w) &&
		(y > pos.y && y < pos.y + item->data->position.h);
}

CheckBox * j1Gui::CreateCheckBox(const int &pos_x, const int &pos_y, const SDL_Rect &idle, UIElement* parent, const SDL_Rect &hover, const SDL_Rect &push)
{
	CheckBox* ret = nullptr;
	ret = new CheckBox(pos_x, pos_y, idle, hover, push, parent);
	ui_elements.add(ret);
	return ret;
}

Button * j1Gui::CreateButton(const int &pos_x, const int &pos_y, const SDL_Rect &idle, UIElement* parent, const SDL_Rect &hover, const SDL_Rect &push)
{
	Button* ret = nullptr;
	ret = new Button(pos_x, pos_y, idle, hover, push, parent);
	ui_elements.add(ret);
	return ret;
}

Image * j1Gui::CreateImage(const int & pos_x, const int & pos_y, const SDL_Rect & rect, UIElement * parent, bool interactable, bool draggable, bool drawable, bool clipable)
{
	Image* ret = nullptr;
	ret = new Image(pos_x, pos_y, rect, parent, interactable, draggable, drawable, clipable);
	ui_elements.add(ret);
	return ret;
}

Label* j1Gui::CreateLabel(const int &pos_x, const int &pos_y, const char* text, bool interactable, bool draggable, UIElement* parent, Color color, const uint &size, const char* font, uint32 wrap_length)
{
	Label* ret = nullptr;
	ret = new Label(pos_x, pos_y, text, color, font, size, parent, interactable, draggable,wrap_length);
	ui_elements.add(ret);
	return ret;
}

Slider * j1Gui::CreateSlider(const int & pos_x, const int & pos_y, const SDL_Rect & slider_rect, Slider_TYPE type, UIElement * parent)
{
	Slider* ret = nullptr;
	ret = new Slider(pos_x, pos_y, slider_rect, parent, type);
	ui_elements.add(ret);
	return ret;
}


void j1Gui::UI_Events(UIElement* element){

}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}