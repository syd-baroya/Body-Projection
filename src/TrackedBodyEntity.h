#pragma once

#ifndef TRACKED_BODY_ENTITY_H
#define TRACKED_BODY_ENTITY_H
#include <glm/glm.hpp>
#include <map>
#include <k4abttypes.h>
#include "Entity.h"
#include <glm/glm.hpp>

using namespace glm;

#define CASCADE 5
#define RECOVER_JOINT_POSITION_TIME 1.0
#define FORECASTFACT 1.7 //0.1

class TrackedBodyEntity : public Entity
{

public:
	TrackedBodyEntity() : Entity() { reset();  }
	TrackedBodyEntity(const std::string& name) : Entity(name) { reset();  }

	glm::vec3 getJoint(float forecast_fact, int j);

	//glm::vec3 new_get_joint(float forecast_fact, int j);


	long double calculateJointAngles(vec3 a, vec3 b, vec3 c);

	std::map< k4abt_joint_id_t, long double> averageJointAngles(vector<TrackedBodyEntity> tracked_body);

	void generateBodyVertices(vector<vec3>* pos, vector<vec3> app_pos);

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

private:
	void generateJointAngles(std::map<k4abt_joint_id_t, long double>* joint_angles);
	void angleHierarchy(std::map <k4abt_joint_id_t, vector<k4abt_joint_id_t>>* angle_heirarchy);
};


#endif
