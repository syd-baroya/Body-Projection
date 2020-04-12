#pragma once
#include "Texture.h"

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {

public:
	Animation(){}

	float getStartTime() { return(start_time); }
	float getCoolDownTime() { return(cooldown_time); }

protected:
	float start_time = 0;
	float cooldown_time = 0;

	Texture animation_texture;
};

#endif
