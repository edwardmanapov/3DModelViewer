#pragma once

#include "Renderable.h"
#include "Listener.h"

class Core : public Renderable, public Listener
{
	public:
		Core();
		~Core();
};

