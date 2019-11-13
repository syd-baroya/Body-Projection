#include "new_body.h"

void new_body_::InitializeDefaultSensor()
	{
	VERIFY(k4a_device_open(0, &this->device), "Open K4A Device failed!");

	// Start camera. Make sure depth camera is enabled.
	VERIFY(k4a_device_start_cameras(this->device, &this->deviceConfig), "Start K4A cameras failed!");
	k4a_calibration_t sensorCalibration;

	VERIFY(k4a_device_get_calibration(this->device, this->deviceConfig.depth_mode, this->deviceConfig.color_resolution, &sensorCalibration),
		"Get depth camera calibration failed!");

	VERIFY(k4abt_tracker_create(&sensorCalibration, this->tracker_config, &this->tracker), "Body tracker initialization failed!");

}

int new_body_::Update(float frametime)
{
	int trackedbodies = 0;
	k4a_capture_t sensor_capture=nullptr;

	k4a_wait_result_t get_capture_result = k4a_device_get_capture(this->device, &sensor_capture, 0);
	if (get_capture_result == K4A_WAIT_RESULT_SUCCEEDED)
	{
		k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(this->tracker, sensor_capture, 0);
		k4a_capture_release(sensor_capture); // Remember to release the sensor capture once you finish using it
		//if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)
		//{
		//	// It should never hit timeout when K4A_WAIT_INFINITE is set.
		//	printf("Error! Add capture to tracker process queue timeout!\n");
		//	return 0;
		//}
		if (queue_capture_result == K4A_WAIT_RESULT_FAILED)
		{
			printf("Error! Add capture to tracker process queue failed!\n");
			return 0;
		}
	}
	else if (get_capture_result == K4A_WAIT_RESULT_TIMEOUT)
	{
		// It should never hit time out when K4A_WAIT_INFINITE is set.
		printf("Error! Get depth frame time out!\n");
		return 0;
	}
	/*else
	{
		printf("Get depth capture returned error: %d\n", get_capture_result);
		return 0;
	}*/

	k4abt_frame_t body_frame = nullptr;
	k4a_wait_result_t pop_frame_result = k4abt_tracker_pop_result(this->tracker, &body_frame, 0);
	if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
	{
		// Successfully popped the body tracking result. Start your processing

		uint64_t nTime = k4abt_frame_get_device_timestamp_usec(body_frame);
		new_bodypack_ bodypack;


		size_t num_bodies = k4abt_frame_get_num_bodies(body_frame);
		//printf("%i bodies are detected!\n", num_bodies);


		for (size_t i = 0; i < num_bodies; i++)
		{
			k4abt_body_t body;
			VERIFY(k4abt_frame_get_body_skeleton(body_frame, i, &body.skeleton), "Get skeleton from body frame failed!");
			body.id = k4abt_frame_get_body_id(body_frame, i);
			if (body.id != K4ABT_INVALID_BODY_ID)
			{
				bodypack.bTracked[i] = true;
				bodypack.ppBodies[i] = {body.id, body.skeleton };
				trackedbodies++;
			}
			else 
			{
				cout << "Body id is not valid." << endl;
				bodypack.bTracked[i] = false;
			}
			
		}

		if (trackedbodies > 0) {
			ProcessBody(frametime, nTime, num_bodies, &bodypack);
		}
		else
			trackedbody.time_till_last_tracked += frametime;

		k4abt_frame_release(body_frame); // Remember to release the body frame once you finish using it

		if (trackedbody.time_till_last_tracked > BODY_LOST_TIME)
			trackedbody.reset();
		
	}
	//else if (pop_frame_result == K4A_WAIT_RESULT_TIMEOUT)
	//{
	//	//  It should never hit timeout when K4A_WAIT_INFINITE is set.
	//	printf("Error! Pop body frame result timeout!\n");
	//	return 0;
	//}
	//else
	//{
	//	printf("Pop body frame result failed!\n");
	//	return 0;
	//}
	
	
	return trackedbodies;
}

bool new_body_::ProcessBody(float frametime, uint64_t nTime, int nBodyCount, new_bodypack_* bodypack)
{
	k4abt_body_t* p_closest_Body_to_track = NULL;
	float closest = -1;
	for (int i = 0; i < nBodyCount; i++) 
	{
		if (bodypack->bTracked[i])
		{
			k4abt_body_t pBody = bodypack->ppBodies[i];
			k4abt_joint_t* joints;
			joints = pBody.skeleton.joints;
			if (closest<0 || closest>joints[0].position.xyz.z)
			{
				closest = joints[0].position.xyz.z;
				p_closest_Body_to_track = &pBody;
			}
		}
	}

	if (!p_closest_Body_to_track)
	{
		trackedbody.time_till_last_tracked += frametime;
		return false;
	}
	k4abt_joint_t* joints = NULL;
	joints = p_closest_Body_to_track->skeleton.joints;
	trackedbody.init_tracked = true;
	trackedbody.time_till_last_tracked = 0;
	if (joints != NULL) {
		for (int j = 0; j < static_cast<int>(K4ABT_JOINT_COUNT); j++)
		{
			if (joints[j].confidence_level == K4ABT_JOINT_CONFIDENCE_NONE)
			{
				trackedbody.jointTracked[j] = false;
				trackedbody.time_till_last_joint_tracked[j] += frametime;
				cout << "lost track on joint: " << j << endl;
			}
			else
			{

				trackedbody.jointTracked[j] = true;
				trackedbody.time_till_last_joint_tracked[j] = 0;
				trackedbody.joint_positions[j].x = joints[j].position.xyz.x*MillimeterToMeter;
				trackedbody.joint_positions[j].y = -joints[j].position.xyz.y*MillimeterToMeter;
				trackedbody.joint_positions[j].z = -joints[j].position.xyz.z*MillimeterToMeter;
			}
		}
		trackedbody.cascade();
	}
	else
		trackedbody.time_till_last_tracked += frametime;
	return true;

}


void new_body_::CloseSensor()
{

	printf("Finished body tracking processing!\n");

	k4abt_tracker_shutdown(tracker);
	k4abt_tracker_destroy(tracker);
	k4a_device_stop_cameras(device);
	k4a_device_close(device);
}

	