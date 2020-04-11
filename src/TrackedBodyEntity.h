#pragma once

#ifndef TRACKED_BODY_ENTITY_H
#define TRACKED_BODY_ENTITY_H
#include <glm/glm.hpp>
#include <map>
#include <k4abttypes.h>
#include "Entity.h"

#define CASCADE 5
#define RECOVER_JOINT_POSITION_TIME 1.0

class TrackedBodyEntity : public Entity
{

public:
	TrackedBodyEntity() : Entity() { reset();  }
	TrackedBodyEntity(const std::string& name) : Entity(name) { reset();  }

	glm::vec3 getJoint(float forecast_fact, int j);

	//glm::vec3 new_get_joint(float forecast_fact, int j);


	TrackedBodyEntity() { reset(); }
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

};


#endif
