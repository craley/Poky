#pragma once

#include <SDL.h>

class Screen {
// I prefer POD, but for this project, it may be too complex
public:
	Screen() {}
	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

	virtual ~Screen() {}
	// defer initialization because of emplace_back noexcept
	virtual bool initialize(RenderContext *context)
	{
		m_context = context;
		return true;
	}
	virtual void handleEvent(const SDL_Event &) {}
	virtual void frameStep(unsigned long tickMS) = 0;

protected:
	RenderContext *m_context;
};
