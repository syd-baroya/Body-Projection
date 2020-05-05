#pragma once
#ifndef KINECT_SYSTEM_HPP
#define KINECT_SYSTEM_HPP

#include "WindowManager.h"
#include <k4arecord/playback.h>
#include <k4abt.h>
#include <iostream>


class KinectSystem
{
public:
	static KinectSystem& getInstance() {
		static KinectSystem _instance_;

		return(_instance_);
	}


	void VisualizeResult(k4abt_frame_t bodyFrame, WindowManager& window3d, int depthWidth, int depthHeight);

	//void PlayFile(InputSettings inputSettings);

	void processFromDevice(WindowManager& window3d);
	void initDevice(WindowManager& window3d);
	void init();

	void shutdown();

private:
	k4a_device_t device = nullptr;
	k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	k4a_calibration_t sensorCalibration;
	k4abt_tracker_t tracker = nullptr;
	k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
	int depthWidth;
	int depthHeight;
};

#endif
