#pragma once
#include "Texture.h"
#include <unordered_map>
#include "Entity.h"

#ifndef ANIMATION_H
#define ANIMATION_H

class AnimationComponent : public Component {

public:
	AnimationComponent(){}
	AnimationComponent(Texture tex) : animation_texture(tex) {}

	void init() override;
	void update(double frametime) override;
	void draw(Program* prog) override;

	float getStartTime() { return(start_time); }
	float getCoolDownTime() { return(cooldown_time); }
	void setStartTime(float st) { start_time = st;  }
	void setCoolDownTime(float cdt) { cooldown_time = cdt; }
	void pauseAll();
	void unpauseAll();
	void reset();

protected:
	float start_time = 0;
	float cooldown_time = 0;

	Texture animation_texture;
	TextureArray animation_texture_array;

};

#endif
