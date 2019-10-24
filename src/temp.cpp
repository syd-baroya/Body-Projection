int frame_count = 0;
do
{
	k4a_wait_result_t get_capture_result = k4a_device_get_capture(device, &sensor_capture, K4A_WAIT_INFINITE);
	if (get_capture_result == K4A_WAIT_RESULT_SUCCEEDED)
	{
		frame_count++;
		k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(tracker, sensor_capture, K4A_WAIT_INFINITE);
		k4a_capture_release(sensor_capture); // Remember to release the sensor capture once you finish using it
		if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)
		{
			// It should never hit timeout when K4A_WAIT_INFINITE is set.
			printf("Error! Add capture to tracker process queue timeout!\n");
			break;
		}
		else if (queue_capture_result == K4A_WAIT_RESULT_FAILED)
		{
			printf("Error! Add capture to tracker process queue failed!\n");
			break;
		}


		k4a_wait_result_t pop_frame_result = k4abt_tracker_pop_result(tracker, &body_frame, K4A_WAIT_INFINITE);
		if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
		{
			// Successfully popped the body tracking result. Start your processing

			size_t num_bodies = k4abt_frame_get_num_bodies(body_frame);
			for (size_t i = 0; i < num_bodies; i++)
			{
				k4abt_skeleton_t skeleton;
				k4abt_frame_get_body_skeleton(body_frame, i, &skeleton);
				uint32_t id = k4abt_frame_get_body_id(body_frame, i);
				printf("%i bodies are detected!\n", i);

			}

			k4abt_frame_release(body_frame); // Remember to release the body frame once you finish using it
		}
		else if (pop_frame_result == K4A_WAIT_RESULT_TIMEOUT)
		{
			//  It should never hit timeout when K4A_WAIT_INFINITE is set.
			printf("Error! Pop body frame result timeout!\n");
			break;
		}
		else
		{
			printf("Pop body frame result failed!\n");
			break;
		}
	}
	else if (get_capture_result == K4A_WAIT_RESULT_TIMEOUT)
	{
		// It should never hit time out when K4A_WAIT_INFINITE is set.
		printf("Error! Get depth frame time out!\n");
		break;
	}
	else
	{
		printf("Get depth capture returned error: %d\n", get_capture_result);
		break;
	}

} while (frame_count < 100);
