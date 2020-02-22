#include "new_body.h"

void new_body_::InitializeDefaultSensor()
{
	this->device_count = k4a_device_get_installed_count();
	bool add_master_at_end = false;
	uint8_t master_index = 0;
	k4a_device_t master_device = NULL;
	k4a_device_configuration_t master_deviceConfig;
	k4abt_tracker_t master_tracker = NULL;
	for (uint8_t deviceIndex = 0; deviceIndex < this->device_count; deviceIndex++) {

		k4a_device_t new_device = NULL;
		k4a_device_configuration_t deviceConfig = this->deviceConfig;
		k4abt_tracker_t new_tracker = NULL;


		VERIFY(k4a_device_open(deviceIndex, &new_device), "Open K4A Device failed!");

		bool sync_in_jack_connected = nullptr;
		bool sync_out_jack_connected = nullptr;

		k4a_result_t get_connector_status_result = k4a_device_get_sync_jack(new_device, &sync_in_jack_connected, &sync_out_jack_connected);
		if (get_connector_status_result == K4A_RESULT_SUCCEEDED)
		{
			if (sync_out_jack_connected && !sync_in_jack_connected) {
				if (this->device_count != 1) {
					deviceConfig.wired_sync_mode = K4A_WIRED_SYNC_MODE_MASTER;
					deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_720P;
					if (deviceIndex != 0) {
						add_master_at_end = true;
						master_index = deviceIndex;
						master_device = new_device;
						master_deviceConfig = deviceConfig;
						master_tracker = new_tracker;
					}
				}
				else {
					deviceConfig.wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;
				}
			}
			else if (sync_out_jack_connected && sync_in_jack_connected) {
				deviceConfig.wired_sync_mode = K4A_WIRED_SYNC_MODE_SUBORDINATE;
				deviceConfig.subordinate_delay_off_master_usec = 10;
			}
			else if (!sync_out_jack_connected && sync_in_jack_connected) {
				deviceConfig.wired_sync_mode = K4A_WIRED_SYNC_MODE_SUBORDINATE;
				deviceConfig.subordinate_delay_off_master_usec = 10;
			}
		}

		if (master_index != deviceIndex || !add_master_at_end) {
			// Start camera. Make sure depth camera is enabled.
			VERIFY(k4a_device_start_cameras(new_device, &deviceConfig), "Start K4A cameras failed!");
			k4a_calibration_t sensorCalibration;

			VERIFY(k4a_device_get_calibration(new_device, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration),
				"Get depth camera calibration failed!");

			VERIFY(k4abt_tracker_create(&sensorCalibration, this->tracker_config, &new_tracker), "Body tracker initialization failed!");

			k4abt_tracker_set_temporal_smoothing(new_tracker, 0.5f);

			this->devices.push_back(new_device);
			this->trackers.push_back(new_tracker);
			this->trackedbody.push_back(new_trackedbody_());
		}
	}
	if (add_master_at_end) {
		// Start camera. Make sure depth camera is enabled.
		VERIFY(k4a_device_start_cameras(master_device, &master_deviceConfig), "Start K4A cameras failed!");
		k4a_calibration_t sensorCalibration;

		VERIFY(k4a_device_get_calibration(master_device, master_deviceConfig.depth_mode, master_deviceConfig.color_resolution, &sensorCalibration),
			"Get depth camera calibration failed!");

		VERIFY(k4abt_tracker_create(&sensorCalibration, this->tracker_config, &master_tracker), "Body tracker initialization failed!");
		k4abt_tracker_set_temporal_smoothing(master_tracker, 0.5f);

		this->devices.insert(this->devices.begin(), master_device);
		this->trackers.insert(this->trackers.begin(), master_tracker);
		this->trackedbody.insert(this->trackedbody.begin(), new_trackedbody_());

	}
}

int new_body_::Update(float frametime)
{
	int trackedbodies = 0;
	for (uint32_t deviceIndex = 0; deviceIndex < this->device_count; deviceIndex++) {
		trackedbodies = 0;
		k4a_capture_t sensor_capture = nullptr;
		k4a_wait_result_t get_capture_result = k4a_device_get_capture(this->devices.at(deviceIndex), &sensor_capture, K4A_WAIT_INFINITE);
		if (get_capture_result == K4A_WAIT_RESULT_SUCCEEDED)
		{
			k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(this->trackers.at(deviceIndex), sensor_capture, K4A_WAIT_INFINITE);
			k4a_capture_release(sensor_capture); // Remember to release the sensor capture once you finish using it
			if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)
			{
				// It should never hit timeout when K4A_WAIT_INFINITE is set.
				printf("Error! Add capture to tracker process queue timeout!\n");
				return 0;
			}
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
		else
		{
			printf("Get depth capture returned error: %d\n", get_capture_result);
			return 0;
		}

		k4abt_frame_t body_frame = nullptr;
		k4a_wait_result_t pop_frame_result = k4abt_tracker_pop_result(this->trackers.at(deviceIndex), &body_frame, 0);
		if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
		{
			// Successfully popped the body tracking result. Start your processing

			uint64_t nTime = k4abt_frame_get_device_timestamp_usec(body_frame);
			new_bodypack_ bodypack;


			this->num_bodies = k4abt_frame_get_num_bodies(body_frame);
			//printf("%i bodies are detected!\n", num_bodies);


			for (size_t i = 0; i < this->num_bodies; i++)
			{
				k4abt_body_t body;
				VERIFY(k4abt_frame_get_body_skeleton(body_frame, i, &body.skeleton), "Get skeleton from body frame failed!");
				body.id = k4abt_frame_get_body_id(body_frame, i);
				if (body.id != K4ABT_INVALID_BODY_ID)
				{
					bodypack.bTracked[i] = true;
					bodypack.ppBodies[i] = { body.id, body.skeleton };
					trackedbodies++;
				}
				else
				{
					cout << "Body id is not valid." << endl;
					bodypack.bTracked[i] = false;
				}

			}

			if (trackedbodies > 0) {

				ProcessBody(frametime, nTime, this->num_bodies, &bodypack, deviceIndex);

			}
			else
				trackedbody.at(deviceIndex).time_till_last_tracked += frametime;

			k4abt_frame_release(body_frame); // Remember to release the body frame once you finish using it

			if (trackedbody.at(deviceIndex).time_till_last_tracked > BODY_LOST_TIME)
				trackedbody.at(deviceIndex).reset();

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
	return trackedbodies;
}

bool new_body_::ProcessBody(float frametime, uint64_t nTime, int nBodyCount, new_bodypack_* bodypack, uint32_t deviceIndex)
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
		trackedbody.at(deviceIndex).time_till_last_tracked += frametime;
		return false;
	}
	k4abt_joint_t* joints = NULL;
	joints = p_closest_Body_to_track->skeleton.joints;
	trackedbody.at(deviceIndex).init_tracked = true;
	trackedbody.at(deviceIndex).time_till_last_tracked = 0;
	if (joints != NULL) {
		for (int j = 0; j < static_cast<int>(K4ABT_JOINT_COUNT); j++)
		{
			if (joints[j].confidence_level == K4ABT_JOINT_CONFIDENCE_NONE)
			{
				trackedbody.at(deviceIndex).jointTracked[j] = false;
				trackedbody.at(deviceIndex).time_till_last_joint_tracked[j] += frametime;
				cout << "lost track on joint: " << j << ", on device: " << deviceIndex << endl;
			}
			else
			{

				trackedbody.at(deviceIndex).jointTracked[j] = true;
				trackedbody.at(deviceIndex).time_till_last_joint_tracked[j] = 0;
				trackedbody.at(deviceIndex).joint_positions[j].x = joints[j].position.xyz.x*MillimeterToMeter;
				trackedbody.at(deviceIndex).joint_positions[j].y = -joints[j].position.xyz.y*MillimeterToMeter;
				trackedbody.at(deviceIndex).joint_positions[j].z = -joints[j].position.xyz.z*MillimeterToMeter;
			}
		}
		trackedbody.at(deviceIndex).cascade();
	}
	else
		trackedbody.at(deviceIndex).time_till_last_tracked += frametime;
	return true;

}


void new_body_::CloseSensor()
{

	printf("Finished body tracking processing!\n");

	for (int deviceIndex = this->device_count - 1; deviceIndex >= 0 ; deviceIndex--) {
		k4abt_tracker_shutdown(this->trackers.at(deviceIndex));
		k4abt_tracker_destroy(this->trackers.at(deviceIndex));
		k4a_device_stop_cameras(this->devices.at(deviceIndex));
		k4a_device_close(this->devices.at(deviceIndex));
	}
}

uint32_t new_body_::getDeviceCount()
{
	return this->device_count;
}

void new_body_::setDeviceCount(uint32_t device_count)
{
	this->device_count = device_count;
}

size_t new_body_::getNumBodies()
{
	return this->num_bodies;
}
	