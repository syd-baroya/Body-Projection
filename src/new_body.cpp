#include "new_body.h"

void new_body_::InitializeDefaultSensor()
	{
	VERIFY(k4a_device_open(0, &device), "Open K4A Device failed!");

	// Start camera. Make sure depth camera is enabled.
	VERIFY(k4a_device_start_cameras(device, &deviceConfig), "Start K4A cameras failed!");

	VERIFY(k4a_device_get_calibration(device, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration),
		"Get depth camera calibration failed!");

	VERIFY(k4abt_tracker_create(&sensorCalibration, tracker_config, &tracker), "Body tracker initialization failed!");


}

int new_body_::Update(float frametime)
{
	int trackedbodies = 0;
	//k4a_wait_result_t get_capture_result = k4a_device_get_capture(device, &sensor_capture, K4A_WAIT_INFINITE);
	//if (get_capture_result == K4A_WAIT_RESULT_SUCCEEDED)
	//{
	//	k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(tracker, sensor_capture, K4A_WAIT_INFINITE);
	//	k4a_capture_release(sensor_capture); // Remember to release the sensor capture once you finish using it
	//	if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)
	//	{
	//		// It should never hit timeout when K4A_WAIT_INFINITE is set.
	//		printf("Error! Add capture to tracker process queue timeout!\n");
	//		break;
	//	}
	//	else if (queue_capture_result == K4A_WAIT_RESULT_FAILED)
	//	{
	//		printf("Error! Add capture to tracker process queue failed!\n");
	//		break;
	//	}
	//}
	//if (SUCCEEDED(hr))
	//	{
	//	//cout << "tracked" << endl;
	//	INT64 nTime = 0;

	//	hr = pBodyFrame->get_RelativeTime(&nTime);

	////	IBody* ppBodies[BODY_COUNT] = { 0 };
	//	bodypack_ bodypack;

	//	if (SUCCEEDED(hr))
	//		{
	//		hr = pBodyFrame->GetAndRefreshBodyData(_countof(bodypack.ppBodies), bodypack.ppBodies);
	//		}
	//
	//	for (int i = 0; i < BODY_COUNT; ++i)
	//		{
	//		IBody* pBody = bodypack.ppBodies[i];
	//		if (pBody)
	//			{
	//			hr = pBody->get_IsTracked(&bodypack.bTracked[i]);
	//			if(SUCCEEDED(hr) && bodypack.bTracked[i] == true)
	//				trackedbodies++;
	//			}
	//		}	

	//		if (SUCCEEDED(hr) && trackedbodies > 0)
	//		{
	//			ProcessBody(frametime, nTime, BODY_COUNT, &bodypack);
	//		}
	//		else
	//			trackedbody.time_till_last_tracked += frametime;
	//	
	//	bodypack.saverelease();
	//	}
	///*else
	//	cout << "NOT tracked" << endl;*/

	//SafeRelease(pBodyFrame);

	////handle timeouts:

	//if (trackedbody.time_till_last_tracked > BODY_LOST_TIME)
	//	trackedbody.reset();

	return trackedbodies;
}


void new_body_::CloseSensor()
{

	printf("Finished body tracking processing!\n");

	k4abt_tracker_shutdown(tracker);
	k4abt_tracker_destroy(tracker);
	k4a_device_stop_cameras(device);
	k4a_device_close(device);
}

	