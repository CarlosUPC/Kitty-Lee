#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "p2DynArray.h"

struct FrameList {
	SDL_Rect rect;
	iPoint pivot;
};

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	//p2DynArray<FrameList> frames;
	const char* name;

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame), frames(anim.frames)
	{
		//SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect, const iPoint& pivot = { 0, 0 })
	{
		
		frames[last_frame++] = { rect,pivot };
	}

	FrameList& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
		
	}

	int getFrameIndex() const
	{
		return (int)current_frame;
	}

	bool isDone() {
		if (current_frame == last_frame - 1) return true;
		else return false;
	}

	int GetCurrentLoop() const
	{
		return loops;
	}
};

#endif
