#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "j1Module.h"
#include "j1App.h"
#include "j1Window.h"
#include "SDL\include\SDL_rect.h"

enum Fade {
	STAGE,
	MODULE
};
class j1FadeToBlack : public j1Module
{
public:

	j1FadeToBlack();

	// Destructor
	virtual ~j1FadeToBlack();

	// Called before the first frame
	bool Start();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 1.0f);
	void ChangeStage();
	
	bool IsFading();
	bool fading = false;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

public:

	int num_level = 1;

private:

	uint start_time = 0;
	uint total_time = 0;
	SDL_Rect screen;

	j1Module* fade_out = nullptr;
	j1Module* fade_in = nullptr;

	Fade to_fade = MODULE;

	bool have_to_load = false;
};


#endif // __j1FADETOBLACK_H__
