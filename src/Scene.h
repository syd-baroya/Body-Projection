#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <random>
#include "Texture.h"
#include <unordered_map>
#include "Entity.h"

float frand()
{
	return (float)rand() / 32768.0;
}

class Scene {
public:
	Scene() {}

	void pauseAll();
	void unpauseAll();
	void reset();

protected:

	float effect_time = 0;

	bool add_tones = false;

	glm::vec3 red_tone;
	glm::vec3 green_tone;
	glm::vec3 blue_tone;

	glm::vec3 color_scaling;

	Texture scene_texture;

	std::unordered_map<uuid_t, Entity*> entities;


};

#endif
