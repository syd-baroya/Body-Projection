#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <random>
#include "Texture.h"
#include <unordered_map>
#include "Entity.h"
#include <map>
#include <k4abt.h>
#include <glm/glm.hpp>

using namespace glm;

float frand()
{
	return (float)rand() / 32768.0;
}

class SceneComponent : public Component {
public:
	SceneComponent(Texture tex) : scene_texture(tex) {
		red_tone = normalize(vec3(frand(), frand(), frand()));
		green_tone = normalize(vec3(frand(), frand(), frand()));
		blue_tone = normalize(vec3(frand(), frand(), frand()));

		color_scaling = normalize(vec3(frand(), frand(), frand()));
		red_tone *= color_scaling.x;
		green_tone *= color_scaling.y;
		blue_tone *= color_scaling.z;
	}

	void init() override;
	void update() override;
	void draw() override;

	void pauseAll();
	void unpauseAll();
	void reset();

protected:

	float effect_time = 0;

	bool add_tones = false;

	vec3 red_tone;
	vec3 green_tone;
	vec3 blue_tone;

	dvec3 color_scaling;

	Texture scene_texture;



};

#endif
