#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 300

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
<<<<<<< HEAD
	p2DynArray<FrameList> frames;
	const char* name;
=======
	SDL_Rect frames[MAX_FRAMES];
>>>>>>> 0131b7610a2e4f935de47cf632ab723543e97e8b

private:
	int last_frame = 0;
	float current_frame = 0;
	int frame = -1;
	int loops = 0;


public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(int frame = -1)
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			if (frame == -1)
				current_frame = (loop) ? 0.0f : last_frame - 1;
			else {
				current_frame = frame;
			}
			loops++;
		}

		return frames[(int)current_frame];

	}

	bool Finished() const
	{
		return loops > 0;
	}

	void reset() {
		current_frame = 0;
	}

	bool isInFrame(int f) {
		return (current_frame >= f && current_frame < f + 1);
	}

	bool isBetween(int f1, int f2) {
		return (current_frame >= f1 && current_frame < f2);
	}

};

#endif