#include "TrackedBodyEntity.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

glm::vec3 TrackedBodyEntity::getJoint(float forecast_fact, int j)
{
	float cur_time = glfwGetTime();
	float future_time = glfwGetTime() + +0.000000001f;
	static glm::vec3 oldv = glm::vec3(0);
	glm::vec3 sumv = joint_speed[0] + joint_speed[1] + joint_speed[2];
	sumv /= 3.;
	oldv = oldv + (sumv - oldv) * 0.03f;
	glm::vec3 delta_d = oldv * (future_time - cur_time);
	joint_positions[j] = joint_positions[j] + delta_d;
	return joint_positions[j];// -oldv * forecast_fact;// -joint_speed[0] * forecast_fact;
}

void TrackedBodyEntity::reset()
{
	std::cout << "body resetted" << std::endl;
	init_tracked = false;
	time_till_last_tracked = -1;
	for (int i = 0; i < K4ABT_JOINT_COUNT; i++)
	{
		cascade_count[i] = 0;
		joint_tracked[i] = false;
		joint_positions[i] = glm::vec3(0);
		time_till_last_joint_tracked[i] = -1;
		for (int ii = 0; ii < CASCADE; ii++)
			old_joint_positions[i][ii] = glm::vec3(0);
		joint_speed[i] = glm::vec3(0);
	}

}

void TrackedBodyEntity::cascade()
{
	bool stepcascade = false;


	for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
	{
		if (joint_tracked[ii])
		{
			if (cascade_count[ii] == 0)
			{
				joint_speed[ii] = glm::vec3(0);
				old_joint_positions[ii][4] = joint_positions[ii];
			}
			else
			{
				glm::vec3 sum = glm::vec3(0);
				for (int i = CASCADE - 1; i >= CASCADE - cascade_count[ii]; i--)
					sum += old_joint_positions[ii][i];
				sum /= cascade_count[ii];

				old_joint_positions[ii][CASCADE - 1] = joint_positions[ii];
				joint_speed[ii] = sum - joint_positions[ii];
				joint_positions[ii] = sum;

			}
			stepcascade = true;
		}
		else if (cascade_count[ii] > 0 && time_till_last_joint_tracked[ii] <= RECOVER_JOINT_POSITION_TIME)
		{
			glm::vec3 sum = glm::vec3(0);
			for (int i = 0; i < CASCADE - cascade_count[ii]; i++)
				sum += old_joint_positions[ii][i];
			sum /= cascade_count[ii];
			joint_positions[ii] = sum;
			joint_speed[ii] = glm::vec3(0);
		}
		else
		{
			joint_speed[ii] = glm::vec3(0);
			if (time_till_last_joint_tracked[ii] > RECOVER_JOINT_POSITION_TIME)
				cascade_count[ii] = 0;
		}

	}
	if (stepcascade)
	{
		for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
		{
			for (int i = 0; i < CASCADE - 1; i++)
				old_joint_positions[ii][i] = old_joint_positions[ii][i + 1];

			if (cascade_count[ii] < CASCADE)
				cascade_count[ii]++;

		}
	}
}
