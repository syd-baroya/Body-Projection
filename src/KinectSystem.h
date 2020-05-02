#pragma once
#ifndef KINECT_SYSTEM_HPP
#define KINECT_SYSTEM_HPP

#include <vector>
#include "TrackedBodyEntity.h"
#include <k4abt.h>
#include <k4a\k4a.h>
#include <iostream>
#include "Utilities.h"

const float MillimeterToMeter = 0.001f;
#define BODY_LOST_TIME 0.1
#define BODY_COUNT 6

class bodypack_
{
public:
	k4abt_body_t ppBodies[BODY_COUNT] = {};
	bool bTracked[BODY_COUNT];
	bodypack_()
	{
		for (int ii = 0; ii < BODY_COUNT; ii++)
		{
			bTracked[ii] = false;
		}
	}

};

class KinectSystem
{
public:
	static KinectSystem& getInstance() {
		static KinectSystem _instance_;
		return(_instance_);
	}

	int process(float frametime, TrackedBodyEntity* tracked_body);
	void InitializeDefaultSensor();
	void CloseSensor();
	uint32_t getDeviceCount();
	void setDeviceCount(uint32_t device_count);
	size_t getNumBodies();
	uint8_t* getDepthImage() { return(depth_image); }
	ivec2 getDepthImageDim() { return(depth_image_dim); }
	KinectSystem()
	{
		num_bodies = 0;
		device_count = 0;
		deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
		deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
		deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
		tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
		tracker_config.processing_mode = K4ABT_TRACKER_PROCESSING_MODE_CPU;
	}

private:
	bool processEntity(float frametime, uint64_t nTime, int nBodyCount, TrackedBodyEntity* tracked_body, bodypack_* bodypack, uint32_t deviceIndex);
	std::vector<k4a_device_t> devices;
	k4a_device_configuration_t deviceConfig;
	std::vector<k4abt_tracker_t> trackers;
	k4abt_tracker_configuration_t tracker_config;
	uint32_t device_count;
	size_t num_bodies;
	uint8_t* depth_image = NULL;
	ivec2 depth_image_dim;

};

#endif
