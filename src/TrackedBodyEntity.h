#pragma once

#ifndef TRACKED_BODY_ENTITY_H
#define TRACKED_BODY_ENTITY_H
#include <glm/glm.hpp>
#include <map>
#include <k4abttypes.h>
#include <vector>
#include "Entity.h"
#include "GeometryComponent.h"
#include "SceneComponent.h"
#include "AnimationComponent.h"
#include "DrawableComponent.h"

#include <GLFW/glfw3.h>
#include <iostream>
//#include "HelperFunctions.h"
//#include "BodyTrackingHelpers.h"

using namespace glm;
using namespace std;

#define CASCADE 5
#define RECOVER_JOINT_POSITION_TIME 1.0
#define FORECASTFACT 1.7 //0.1

class MeshEntity : public Entity {
public:
	MeshEntity(){}

	virtual void draw(Program* prog)
	{
		DrawableComponent dc = this->getComponent<DrawableComponent>();
		const MVPSet* MVP = dc.getMVP();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &MVP->P[0][0]);
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &MVP->V[0][0]);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &MVP->M[0][0]);
	}

};

class TexturedMeshEntity : public MeshEntity {
public:
	TexturedMeshEntity(){}
	void draw(Program* prog) override;

};

class TrackedBodyEntity : public TexturedMeshEntity
{

public:
	TrackedBodyEntity() : TexturedMeshEntity() { reset();  }

	void update(double frametime) override;
	void draw(Program* prog) override;


	glm::vec3 getJoint(float forecast_fact, int j);


	void generateBodyVertices();

	void reset();

	void cascade();

	

	int cascade_count[K4ABT_JOINT_COUNT];
	glm::vec3 old_joint_positions[K4ABT_JOINT_COUNT][CASCADE];
	bool init_tracked;
	float time_till_last_tracked;
	bool joint_tracked[K4ABT_JOINT_COUNT];
	glm::vec3 joint_positions[K4ABT_JOINT_COUNT];
	float time_till_last_joint_tracked[K4ABT_JOINT_COUNT];
	std::map<k4abt_joint_id_t, long double> joint_angle_map;
	glm::vec3 joint_speed[K4ABT_JOINT_COUNT];

	float throat_scale = 1.0f;

	float torso_width_scale = 1.2;
	float torso_height_scale = 0.5;
	float arm_thickness_scale = 1.2;
	float leg_thickness_scale = 0.4;
	float foot_thickness_scale = 0.5;
	float head_thickness_scale = 2.0;

	float chinstart = 0.5;
	float sidechinstart = 1.35;

	glm::vec3 modelpos = glm::vec3(0);
	glm::vec3 modelscale = glm::vec3(1);
	float forecastfact = FORECASTFACT;

private:
	void generateJointAngles(std::map<k4abt_joint_id_t, long double>* joint_angles);
	void angleHierarchy(std::map <k4abt_joint_id_t, vector<k4abt_joint_id_t>>* angle_heirarchy);

	long double calculateJointAngles(vec3 a, vec3 b, vec3 c);

	std::map< k4abt_joint_id_t, long double> averageJointAngles(vector<TrackedBodyEntity> tracked_body);

	//	M = glm::translate(glm::mat4(1.0f), tracked_body_entity->modelpos) * glm::scale(glm::mat4(1.0f), tracked_body_entity->modelscale);


};


#endif
