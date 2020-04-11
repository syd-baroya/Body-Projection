#pragma once
#include <map>
#include <k4abttypes.h>
#ifndef TRACKED_BODY_ENTITY_H
#define TRACKED_BODY_ENTITY_H
#include <glm/glm.hpp>

#define CASCADE 5

class TrackedBodyEntity
{
private:
	int cascade_count[K4ABT_JOINT_COUNT];
	glm::vec3 old_joint_positions[K4ABT_JOINT_COUNT][CASCADE];


public:
	glm::vec3 joint_speed[K4ABT_JOINT_COUNT];
	glm::vec3 get_joint(float forecast_fact, int j);

	glm::vec3 new_get_joint(float forecast_fact, int j);

	bool init_tracked;
	float time_till_last_tracked;
	bool jointTracked[K4ABT_JOINT_COUNT];
	glm::vec3 joint_positions[K4ABT_JOINT_COUNT];
	float time_till_last_joint_tracked[K4ABT_JOINT_COUNT];
	std::map<k4abt_joint_id_t, long double> jointAngleMap;

	TrackedBodyEntity() { reset(); }
	void OnLostTrack();
	void reset();

	void cascade();

};


#endif
