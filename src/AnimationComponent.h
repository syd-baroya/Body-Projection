#pragma once


#ifndef ANIMATION_H
#define ANIMATION_H


//#include "Entity.h"
#include <glm/glm.hpp>
#include "Texture.h"
#include "Component.h"
#include <unordered_map>

class AnimationComponent : public Component {

public:
	AnimationComponent(){}
	AnimationComponent(Texture tex, TextureArray array_tex) : animation_texture(tex), animation_texture_array(array_tex) {}

	virtual void init() override;
	virtual void update(double phaseprogresstotaltime) override;
	virtual void draw(Program* prog) override;
	virtual void activateTexture();


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
	double phaseprogresstotaltime = 0;
	Texture animation_texture;
	TextureArray animation_texture_array;

};

class FireAnimation : public AnimationComponent {
public:
	FireAnimation() : AnimationComponent() {}
	FireAnimation(Texture tex, TextureArray tex_array) : AnimationComponent(tex, tex_array){}
	void init() override;
	void update(double frametime) override;
	void draw(Program* prog) override;
private:
	float firescale = 0;
	glm::vec2 texoff =glm::vec2(0, 0);
};

#endif
