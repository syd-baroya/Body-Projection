#pragma once
#include <k4abt.h>
#ifndef KINECT_SYSTEM_HPP
#define KINECT_SYSTEM_HPP

#include <vector>

const float MillimeterToMeter = 0.001f;
#define BODY_LOST_TIME 0.1

class KinectSystem
{
private:
	bool processEntity(float frametime, uint64_t nTime, int nBodyCount, new_bodypack_* bodypack, uint32_t deviceIndex);
	std::vector<k4a_device_t> devices;
	k4a_device_configuration_t deviceConfig;
	std::vector<k4abt_tracker_t> trackers;
	k4abt_tracker_configuration_t tracker_config;
	uint32_t device_count;
	size_t num_bodies;

public:
	std::vector<new_trackedbody_> trackedbody;
	int process(float frametime);
	void InitializeDefaultSensor();
	void CloseSensor();
	uint32_t getDeviceCount();
	void setDeviceCount(uint32_t device_count);
	size_t getNumBodies();
	KinectSystem()
	{
		num_bodies = 0;
		device_count = 0;
		deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
		deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
		deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
		tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
		tracker_config.processing_mode = K4ABT_TRACKER_PROCESSING_MODE_GPU;
	}
};

#endif
