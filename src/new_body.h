#pragma once

#include <k4a/k4a.h>
#include <k4abt.h>
#include <k4abttypes.h>
#include <BodyTrackingHelpers.h>
#include <Utilities.h>

#include <iostream>
#include <glad/glad.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

#define RECOVER_JOINT_POSITION_TIME 1.0
#define BODY_LOST_TIME 0.1
#define FORECASTFACT 1.7 //0.1
#define CASCADE 5
#define BODY_COUNT 6

const float MillimeterToMeter = 0.001f;

class new_bodypack_
{
public:
	k4abt_body_t ppBodies[BODY_COUNT] = {};
	bool bTracked[BODY_COUNT];
	new_bodypack_()
		{
		for (int ii = 0; ii < BODY_COUNT; ii++)
			{
			bTracked[ii] = false;
			}
		}

};

class new_trackedbody_
{
	private:
		int cascade_count[K4ABT_JOINT_COUNT];
		vec3 old_joint_positions[K4ABT_JOINT_COUNT][CASCADE];
		
		
	public:
		vec3 joint_speed[K4ABT_JOINT_COUNT];
		vec3 get_joint(float forecast_fact, int j)
			{
			static vec3 oldv = vec3(0);
			vec3 sumv = joint_speed[0] + joint_speed[1] + joint_speed[2];
			sumv /= 3.;
			oldv = oldv + (sumv - oldv)*0.03f;
			//oldv = vec3(0);
			return joint_positions[j] - oldv * forecast_fact;// -joint_speed[0] * forecast_fact;
		
			}
		bool init_tracked;
		float time_till_last_tracked;
		bool jointTracked[K4ABT_JOINT_COUNT];
		vec3 joint_positions[K4ABT_JOINT_COUNT];
		float time_till_last_joint_tracked[K4ABT_JOINT_COUNT];
		new_trackedbody_() { reset(); }
		void OnLostTrack()
		{}
		void reset()
			{			
			cout << "body resetted" << endl;
			init_tracked = false;
			time_till_last_tracked = -1;
			for (int i = 0; i < K4ABT_JOINT_COUNT; i++)
				{
				cascade_count[i] = 0;
				jointTracked[i] = false;
				joint_positions[i] = vec3(0);
				time_till_last_joint_tracked[i] = -1;
				for (int ii = 0; ii < CASCADE; ii++)
					old_joint_positions[i][ii] = vec3(0);
				joint_speed[i] = vec3(0);
				}
			

		
			}
	

		void cascade()
		{
			bool stepcascade = false;
			

			for (int ii = 0; ii < K4ABT_JOINT_COUNT; ii++)
			{
				if (jointTracked[ii])
				{
					if (cascade_count[ii] == 0)
					{
						joint_speed[ii] = vec3(0);
						old_joint_positions[ii][4] = joint_positions[ii];
					}
					else
					{
						vec3 sum = vec3(0);
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
					vec3 sum = vec3(0);
					for (int i = 0; i < CASCADE - cascade_count[ii]; i++)
						sum += old_joint_positions[ii][i];
					sum /= cascade_count[ii];
					joint_positions[ii] = sum;
					joint_speed[ii] = vec3(0);
				}
				else
				{
					joint_speed[ii] = vec3(0);
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

};

class new_body_
	{
	private:
		bool ProcessBody(float frametime, uint64_t nTime, int nBodyCount, new_bodypack_* bodypack, uint32_t deviceIndex);
		vector<k4a_device_t> devices;
		k4a_device_configuration_t deviceConfig;
		vector<k4abt_tracker_t> trackers;
		k4abt_tracker_configuration_t tracker_config;
		uint32_t device_count;

	public:
		vector<new_trackedbody_> trackedbody;
		int Update(float frametime);
		void InitializeDefaultSensor();
		void CloseSensor();
		uint32_t getDeviceCount();
		void setDeviceCount(uint32_t device_count);
		new_body_()
		{
			device_count = 0;
			deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
			deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
			deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
			tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
			tracker_config.processing_mode = K4ABT_TRACKER_PROCESSING_MODE_CPU;
		}
	};