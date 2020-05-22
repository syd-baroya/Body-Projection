#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <random>
#include "Texture.h"
#include <unordered_map>
#include <map>
#include <k4abt.h>
#include <glm/glm.hpp>
#include "HelperFunctions.h"
#include "Component.h"
#include <glm\ext\matrix_transform.hpp>
#include <math.h>
//#include "GeometryComponent.h"
//#include "TrackedBodyEntity.h"
//#include "Entity.h"

using namespace glm;
using namespace HelperFunctions;


class SceneComponent : public Component {
public:
	SceneComponent(){}
	SceneComponent(std::string tex_file) {
		this->scene_texture = new SimpleTexture2D(tex_file);
		this->scene_texture->initParams();
		this->init();
	}

	virtual void init() override;
	virtual void update(double frametime) override;
	virtual void draw(Program* prog) override;

	virtual void activateTexture(Program* prog);

	void setOffset(int length_of_x_anims, int length_of_y_anims);

	SimpleTexture2D* getTexture() { return(scene_texture); }

	void pauseAll();
	void unpauseAll();
	void reset();
	float getTotalEffectTime() { return(effect_time); }

	vec3 red_tone;
	vec3 green_tone;
	vec3 blue_tone;
protected:

	float effect_time = 0;

	ivec2 offset = ivec2(0);
	bool has_offset = false;
	bool add_tones = false;
	bool is_paused = false;


	dvec3 color_scaling;

	SimpleTexture2D* scene_texture;



};



#define FURMAXTEX 7
#define FURCHANGETIME 2.0//11.0
#define FURBLENDTIME 1.0//10.0

class FurScene : public SceneComponent {
public:
	FurScene(SimpleTexture2D* tex) {}


	void init() override;
	void update(double frametime) override;
	void draw(Program* prog) override;

private:
	SimpleTexture2D scene_texture[FURMAXTEX];
	float blend;
	vec3 actual_redtone;
	vec3 actual_greentone;
	vec3 actual_bluetone;
	GLuint tex1;
	GLuint tex2;
};


//#define BUTTERFLYCOUNT 50
//
//
//class butterfly_
//{
//public:
//	float rotz;
//	float scale;
//	int iA, iB;
//	float rationAB;
//	float startanim;
//	vec3 red, green, blue;
//};
//
//class ButterflyScene : public SceneComponent {
//public:
//	ButterflyScene(Texture tex, float total_time) : SceneComponent(tex, total_time) {}
//
//	butterfly_* getButterfly() { return(butterfly); }
//	void updateCurrButterfly() { curr_butterfly++; }
//
//	void init() override;
//	void update(double frametime) override;
//	void draw(Program* prog) override;
//
//private:
//	butterfly_ butterfly[BUTTERFLYCOUNT];
//	int curr_butterfly = 0;
//	int actual_butterfly_size = 0;
//	vec4 texoff = vec4(4, 4, 0, 0);
//
//
//};
//
//class SkeletonHeartScene : public SceneComponent {
//public:
//	SkeletonHeartScene(Texture tex, float total_time) : SceneComponent(tex, total_time) {}
//
//
//	//void init() override;
//	//void update(double frametime) override;
//	void draw(Program* prog) override;
//
//private:
//	Texture heart_texture;
//};

#endif
