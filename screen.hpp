#pragma once

#include <SDL.h>

class Screen {
public:
	// explicit initialize/destroy v.s. constructor/destructor
	virtual bool initialize(RenderContext *context)
	{
		m_context = context;
		return true;
	}
	virtual void destroy() {};
	virtual void handleEvent(const SDL_Event &) {}
	virtual void frameStep(unsigned long tickMS) = 0;
	virtual void render(unsigned long tickMS) = 0;

protected:
	RenderContext *m_context;
};
