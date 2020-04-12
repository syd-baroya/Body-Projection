#pragma once
#include "Texture.h"
#include <unordered_map>
#include "Entity.h"

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {

public:
	Animation(){}

	float getStartTime() { return(start_time); }
	float getCoolDownTime() { return(cooldown_time); }
	void addEntity(Entity* entity);

protected:
	float start_time = 0;
	float cooldown_time = 0;

	Texture animation_texture;
	std::unordered_map<uuid_t, Entity*> entities;

};

#endif
