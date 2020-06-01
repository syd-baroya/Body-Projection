#include "KinectSystem.h"


void KinectSystem::VisualizeResult(k4abt_frame_t bodyFrame, WindowManager& window3d, int depthWidth, int depthHeight) {

    // Obtain original capture that generates the body tracking result
    k4a_capture_t originalCapture = k4abt_frame_get_capture(bodyFrame);
    k4a_image_t depthImage = k4a_capture_get_depth_image(originalCapture);

    std::vector<Color> pointCloudColors(depthWidth * depthHeight, { 0.f, 0.f, 0.f, 1.f });

    // Read body index map and assign colors
    k4a_image_t bodyIndexMap = k4abt_frame_get_body_index_map(bodyFrame);
    const uint8_t* bodyIndexMapBuffer = k4a_image_get_buffer(bodyIndexMap);
    for (int i = 0; i < depthWidth * depthHeight; i++)
    {
        uint8_t bodyIndex = bodyIndexMapBuffer[i];
        if (bodyIndex != K4ABT_BODY_INDEX_MAP_BACKGROUND)
        {
            uint32_t bodyId = k4abt_frame_get_body_id(bodyFrame, bodyIndex);
            pointCloudColors[i] = g_bodyColors[bodyId % g_bodyColors.size()];
        }
    }
    k4a_image_release(bodyIndexMap);

    // Visualize point cloud
    window3d.UpdatePointClouds(depthImage, pointCloudColors);

    // Visualize the skeleton data
    //window3d.CleanJointsAndBones();
    //uint32_t numBodies = k4abt_frame_get_num_bodies(bodyFrame);
    //for (uint32_t i = 0; i < numBodies; i++)
    //{
    //    k4abt_body_t body;
    //    VERIFY(k4abt_frame_get_body_skeleton(bodyFrame, i, &body.skeleton), "Get skeleton from body frame failed!");
    //    body.id = k4abt_frame_get_body_id(bodyFrame, i);

    //    // Assign the correct color based on the body id
    //    Color color = g_bodyColors[body.id % g_bodyColors.size()];
    //    color.a = 0.4f;
    //    Color lowConfidenceColor = color;
    //    lowConfidenceColor.a = 0.1f;

    //    // Visualize joints
    //    for (int joint = 0; joint < static_cast<int>(K4ABT_JOINT_COUNT); joint++)
    //    {
    //        if (body.skeleton.joints[joint].confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW)
    //        {
    //            const k4a_float3_t& jointPosition = body.skeleton.joints[joint].position;
    //            const k4a_quaternion_t& jointOrientation = body.skeleton.joints[joint].orientation;

    //            window3d.AddJoint(
    //                jointPosition,
    //                jointOrientation,
    //                body.skeleton.joints[joint].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM ? color : lowConfidenceColor);
    //        }
    //    }

    //    // Visualize bones
    //    for (size_t boneIdx = 0; boneIdx < g_boneList.size(); boneIdx++)
    //    {
    //        k4abt_joint_id_t joint1 = g_boneList[boneIdx].first;
    //        k4abt_joint_id_t joint2 = g_boneList[boneIdx].second;

    //        if (body.skeleton.joints[joint1].confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW &&
    //            body.skeleton.joints[joint2].confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW)
    //        {
    //            bool confidentBone = body.skeleton.joints[joint1].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM &&
    //                body.skeleton.joints[joint2].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM;
    //            const k4a_float3_t& joint1Position = body.skeleton.joints[joint1].position;
    //            const k4a_float3_t& joint2Position = body.skeleton.joints[joint2].position;

    //            window3d.AddBone(joint1Position, joint2Position, confidentBone ? color : lowConfidenceColor);
    //        }
    //    }
    //}

    k4a_capture_release(originalCapture);
    k4a_image_release(depthImage);

}

//void KinectSystem::PlayFile(InputSettings inputSettings) {
//    // Initialize the 3d window controller
//    WindowManager window3d;
//
//    //create the tracker and playback handle
//    k4a_calibration_t sensor_calibration;
//    k4abt_tracker_t tracker = NULL;
//    k4a_playback_t playback_handle = NULL;
//
//    const char* file = inputSettings.FileName.c_str();
//    if (k4a_playback_open(file, &playback_handle) != K4A_RESULT_SUCCEEDED)
//    {
//        printf("Failed to open recording: %s\n", file);
//        return;
//    }
//
//
//    if (k4a_playback_get_calibration(playback_handle, &sensor_calibration) != K4A_RESULT_SUCCEEDED)
//    {
//        printf("Failed to get calibration\n");
//        return;
//    }
//
//
//    k4a_capture_t capture = NULL;
//    k4a_stream_result_t result = K4A_STREAM_RESULT_SUCCEEDED;
//
//    k4abt_tracker_configuration_t tracker_config = { K4ABT_SENSOR_ORIENTATION_DEFAULT };
//
//    tracker_config.processing_mode = inputSettings.CpuOnlyMode ? K4ABT_TRACKER_PROCESSING_MODE_CPU : K4ABT_TRACKER_PROCESSING_MODE_GPU;
//
//    VERIFY(k4abt_tracker_create(&sensor_calibration, tracker_config, &tracker), "Body tracker initialization failed!");
//
//    k4abt_tracker_set_temporal_smoothing(tracker, 1);
//
//    int depthWidth = sensor_calibration.depth_camera_calibration.resolution_width;
//    int depthHeight = sensor_calibration.depth_camera_calibration.resolution_height;
//
//    window3d.Create("3D Visualization", sensor_calibration);
//    window3d.SetCloseCallback(CloseCallback);
//    window3d.SetKeyCallback(ProcessKey);
//
//    while (result == K4A_STREAM_RESULT_SUCCEEDED)
//    {
//        result = k4a_playback_get_next_capture(playback_handle, &capture);
//        // check to make sure we have a depth image
//        // if we are not at the end of the file
//        if (result != K4A_STREAM_RESULT_EOF) {
//            k4a_image_t depth_image = k4a_capture_get_depth_image(capture);
//            if (depth_image == NULL) {
//                //If no depth image, print a warning and skip to next frame
//                printf("Warning: No depth image, skipping frame\n");
//                k4a_capture_release(capture);
//                continue;
//            }
//            // Release the Depth image
//            k4a_image_release(depth_image);
//        }
//        if (result == K4A_STREAM_RESULT_SUCCEEDED)
//        {
//
//            //enque capture and pop results - synchronous
//            k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(tracker, capture, K4A_WAIT_INFINITE);
//
//            // Release the sensor capture once it is no longer needed.
//            k4a_capture_release(capture);
//
//            k4abt_frame_t bodyFrame = NULL;
//            k4a_wait_result_t pop_frame_result = k4abt_tracker_pop_result(tracker, &bodyFrame, K4A_WAIT_INFINITE);
//            if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
//            {
//                size_t num_bodies = k4abt_frame_get_num_bodies(bodyFrame);
//                printf("%zu bodies are detected\n", num_bodies);
//                /************* Successfully get a body tracking result, process the result here ***************/
//                VisualizeResult(bodyFrame, window3d, depthWidth, depthHeight);
//                //Release the bodyFrame
//                k4abt_frame_release(bodyFrame);
//            }
//            else
//            {
//                printf("Pop body frame result failed!\n");
//                break;
//            }
//
//        }
//
//        window3d.SetLayout3d(s_layoutMode);
//        window3d.SetJointFrameVisualization(s_visualizeJointFrame);
//        window3d.Render();
//
//        if (result == K4A_STREAM_RESULT_EOF)
//        {
//            // End of file reached
//            break;
//        }
//    }
//    k4abt_tracker_shutdown(tracker);
//    k4abt_tracker_destroy(tracker);
//    window3d.Delete();
//    printf("Finished body tracking processing!\n");
//    k4a_playback_close(playback_handle);
//
//}

void KinectSystem::processFromDevice(WindowManager& window3d) {
    
    k4a_capture_t sensorCapture = nullptr;
    k4a_wait_result_t getCaptureResult = k4a_device_get_capture(device, &sensorCapture, 0); // timeout_in_ms is set to 0

    if (getCaptureResult == K4A_WAIT_RESULT_SUCCEEDED)
    {
        // timeout_in_ms is set to 0. Return immediately no matter whether the sensorCapture is successfully added
        // to the queue or not.
        k4a_wait_result_t queueCaptureResult = k4abt_tracker_enqueue_capture(tracker, sensorCapture, 0);

        // Release the sensor capture once it is no longer needed.
        k4a_capture_release(sensorCapture);

        if (queueCaptureResult == K4A_WAIT_RESULT_FAILED)
        {
            std::cout << "Error! Add capture to tracker process queue failed!" << std::endl;
            exit(1);
        }
    }
    else if (getCaptureResult != K4A_WAIT_RESULT_TIMEOUT)
    {
        std::cout << "Get depth capture returned error: " << getCaptureResult << std::endl;
        exit(1);
    }

    // Pop Result from Body Tracker
    k4abt_frame_t bodyFrame = nullptr;
    k4a_wait_result_t popFrameResult = k4abt_tracker_pop_result(tracker, &bodyFrame, 0); // timeout_in_ms is set to 0
    if (popFrameResult == K4A_WAIT_RESULT_SUCCEEDED)
    {
        /************* Successfully get a body tracking result, process the result here ***************/
        VisualizeResult(bodyFrame, window3d, depthWidth, depthHeight);
        //Release the bodyFrame
        k4abt_frame_release(bodyFrame);
    }

}

void KinectSystem::initDevice(WindowManager& window3d)
{
    VERIFY(k4a_device_open(0, &device), "Open K4A Device failed!");

    // Start camera. Make sure depth camera is enabled.

    VERIFY(k4a_device_start_cameras(device, &deviceConfig), "Start K4A cameras failed!");

    // Get calibration information

    VERIFY(k4a_device_get_calibration(device, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensorCalibration),
        "Get depth camera calibration failed!");
    depthWidth = sensorCalibration.depth_camera_calibration.resolution_width;
    depthHeight = sensorCalibration.depth_camera_calibration.resolution_height;

    // Create Body Tracker

    VERIFY(k4abt_tracker_create(&sensorCalibration, tracker_config, &tracker), "Body tracker initialization failed!");


    window3d.Create("3D Visualization", sensorCalibration);
    //window3d.SetCloseCallback(CloseCallback);
    //window3d.SetKeyCallback(ProcessKey);

}

void KinectSystem::init()
{
    deviceConfig.depth_mode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
    deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
    tracker_config.processing_mode = K4ABT_TRACKER_PROCESSING_MODE_CPU;

}

void KinectSystem::shutdown() {
    std::cout << "Finished body tracking processing!" << std::endl;

    k4abt_tracker_shutdown(tracker);
    k4abt_tracker_destroy(tracker);

    k4a_device_stop_cameras(device);
    k4a_device_close(device);
}
