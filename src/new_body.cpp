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

	k4a_wait_result_t get_capture_result = k4a_device_get_capture(device, &sensor_capture, K4A_WAIT_INFINITE);
	if (get_capture_result == K4A_WAIT_RESULT_SUCCEEDED)
	{
		k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(tracker, sensor_capture, K4A_WAIT_INFINITE);
		k4a_capture_release(sensor_capture); // Remember to release the sensor capture once you finish using it
		if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)
		{
			// It should never hit timeout when K4A_WAIT_INFINITE is set.
			printf("Error! Add capture to tracker process queue timeout!\n");
			return 0;
		}
		else if (queue_capture_result == K4A_WAIT_RESULT_FAILED)
		{
			printf("Error! Add capture to tracker process queue failed!\n");
			return 0;
		}

		k4abt_frame_t body_frame = NULL;
		k4a_wait_result_t pop_frame_result = k4abt_tracker_pop_result(tracker, &body_frame, K4A_WAIT_INFINITE);
		if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
		{
			// Successfully popped the body tracking result. Start your processing

			uint64_t nTime = k4abt_frame_get_device_timestamp_usec(body_frame);
			new_bodypack_ bodypack;


			size_t num_bodies = k4abt_frame_get_num_bodies(body_frame);
			for (size_t i = 0; i < num_bodies; i++)
			{
				k4abt_skeleton_t skeleton;
				k4a_result_t skeleton_result = k4abt_frame_get_body_skeleton(body_frame, i, &skeleton);
				if (skeleton_result == K4A_RESULT_SUCCEEDED) {
					uint32_t id = k4abt_frame_get_body_id(body_frame, i);
					if (id != K4ABT_INVALID_BODY_ID) {
						bodypack.bTracked[i] = true;
						bodypack.ppBodies[i] = {id, skeleton};
						trackedbodies++;
						printf("%i bodies are detected!\n", i);
					}
				}

				if (trackedbodies > 0) {
					ProcessBody(frametime, nTime, num_bodies, &bodypack);
				}
				else
					trackedbody.time_till_last_tracked += frametime;
			}

			k4abt_frame_release(body_frame); // Remember to release the body frame once you finish using it

			if (trackedbody.time_till_last_tracked > BODY_LOST_TIME)
				trackedbody.reset();
		
		}
		else if (pop_frame_result == K4A_WAIT_RESULT_TIMEOUT)
		{
			//  It should never hit timeout when K4A_WAIT_INFINITE is set.
			printf("Error! Pop body frame result timeout!\n");
			return 0;
		}
		else
		{
			printf("Pop body frame result failed!\n");
			return 0;
		}
	}
	else if (get_capture_result == K4A_WAIT_RESULT_TIMEOUT)
	{
		// It should never hit time out when K4A_WAIT_INFINITE is set.
		printf("Error! Get depth frame time out!\n");
		return 0;
	}
	else
	{
		printf("Get depth capture returned error: %d\n", get_capture_result);
		return 0;
	}
	
	return trackedbodies;
}

bool new_body_::ProcessBody(float frametime, uint64_t nTime, int nBodyCount, new_bodypack_* bodypack)
{
	k4a_result_t result;

	k4abt_body_t* p_closest_Body_to_track = NULL;
	float closest = -1;
	for (int i = 0; i < nBodyCount; ++i) 
	{
		if (bodypack->ppBodies[i] && bodypack->bTracked[i])
		{
			k4abt_body_t* pBody = bodypack->ppBodies[i];
			k4abt_joint_t* joints;
			joints = pBody->skeleton.joints;
			if (closest<0 || closest>joints[0].position.xyz.z)
			{
				closest = joints[0].position.xyz.z;
				p_closest_Body_to_track = pBody;
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
		for (int j = 0; j < K4ABT_JOINT_COUNT; ++j)
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
				trackedbody.joint_positions[j].x = joints[j].position.xyz.x;
				trackedbody.joint_positions[j].y = joints[j].position.xyz.y;
				trackedbody.joint_positions[j].z = -joints[j].position.xyz.z;
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

	