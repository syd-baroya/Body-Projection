// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "WindowManager.h"
#include <iostream>



const float MillimeterToMeter = 0.001f;

glm::vec3 ConvertMillimeterToMeter(k4a_float3_t positionInMM)
{
    return glm::vec3(positionInMM.v[0] * MillimeterToMeter, positionInMM.v[1] * MillimeterToMeter, positionInMM.v[2] * MillimeterToMeter);
}

WindowManager::~WindowManager()
{
    Delete();
}

void WindowManager::Create(
    const char* name,
    k4a_depth_mode_t depthMode,
    int windowWidth,
    int windowHeight)
{
    m_window3d.Create(name, true, windowWidth, windowHeight);
    m_window3d.SetMirrorMode(true);

    switch (depthMode)
    {
    case K4A_DEPTH_MODE_WFOV_UNBINNED:
    case K4A_DEPTH_MODE_WFOV_2X2BINNED:
        m_window3d.SetDefaultVerticalFOV(120.0f);
        break;
    case K4A_DEPTH_MODE_NFOV_2X2BINNED:
    case K4A_DEPTH_MODE_NFOV_UNBINNED:
    default:
        m_window3d.SetDefaultVerticalFOV(65.0f);
        break;
    }
    m_window3d.InitializeScenes();
}

void WindowManager::Create(
    const char* name,
    const k4a_calibration_t& sensorCalibration)
{
    Create(name, sensorCalibration.depth_mode);
    InitializeCalibration(sensorCalibration);
}

//void WindowManager::SetCloseCallback(
//    Visualization::CloseCallbackType closeCallback,
//    void* closeCallbackContext)
//{
//    m_window3d.SetCloseCallback(closeCallback, closeCallbackContext);
//}
//
//void WindowManager::SetKeyCallback(
//    Visualization::KeyCallbackType keyCallback,
//    void* keyCallbackContext)
//{
//    m_window3d.SetKeyCallback(keyCallback, keyCallbackContext);
//}

void WindowManager::Delete()
{
    m_window3d.Delete();

    if (m_transformationHandle != nullptr)
    {
        k4a_transformation_destroy(m_transformationHandle);
        m_transformationHandle = nullptr;
    }

    if (m_pointCloudImage != nullptr)
    {
        k4a_image_release(m_pointCloudImage);
        m_pointCloudImage = nullptr;
    }
}

void WindowManager::UpdatePointClouds(k4a_image_t depthImage, std::vector<Color> pointCloudColors)
{
    m_pointCloudUpdated = true;
    VERIFY(k4a_transformation_depth_image_to_point_cloud(m_transformationHandle,
        depthImage,
        K4A_CALIBRATION_TYPE_DEPTH,
        m_pointCloudImage), "Transform depth image to point clouds failed!");

    int width = k4a_image_get_width_pixels(m_pointCloudImage);
    int height = k4a_image_get_height_pixels(m_pointCloudImage);

    int16_t* pointCloudImageBuffer = (int16_t*)k4a_image_get_buffer(m_pointCloudImage);
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int pixelIndex = h * width + w;
            k4a_float3_t position = {
                static_cast<float>(pointCloudImageBuffer[3 * pixelIndex + 0]),
                static_cast<float>(pointCloudImageBuffer[3 * pixelIndex + 1]),
                static_cast<float>(pointCloudImageBuffer[3 * pixelIndex + 2]) };

            // When the point cloud is invalid, the z-depth value is 0.
            if (!m_window3d.drawOnlyPointCloudOutline() && position.v[2] == 0)
            {
                continue;
            }

            glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.0f );
            glm::ivec2 pixelLocation = glm::ivec2(w, h );

            if (pointCloudColors.size() > 0)
            {
                color = BlendBodyColor(color, pointCloudColors[pixelIndex]);
            }

            glm::vec3 positionInMeter = ConvertMillimeterToMeter(position);
            Visualization::PointCloudVertex pointCloud;
            pointCloud.Position = glm::vec3(positionInMeter.x, positionInMeter.y, positionInMeter.z);
            pointCloud.Color = glm::vec4(color.x, color.y, color.z, color.a);
            pointCloud.PixelLocation.x = pixelLocation.x;
            pointCloud.PixelLocation.y = pixelLocation.y;
            pointCloud.Animate = 0;

            m_pointClouds.push_back(pointCloud);
        }
    }

    UpdateDepthBuffer(depthImage);
}

void WindowManager::CleanJointsAndBones()
{
    m_window3d.CleanJointsAndBones();
}

void WindowManager::AddJoint(k4a_float3_t position, k4a_quaternion_t orientation, Color color)
{
    glm::vec3 jointPositionInMeter = ConvertMillimeterToMeter(position);
    m_window3d.AddJoint({
        glm::vec3(jointPositionInMeter.x, jointPositionInMeter.y, jointPositionInMeter.z),
        glm::quat(orientation.v[0], orientation.v[1], orientation.v[2], orientation.v[3]),
        glm::vec4(color.r, color.g, color.b, color.a) });
}

void WindowManager::AddBone(k4a_float3_t joint1Position, k4a_float3_t joint2Position, Color color)
{
    glm::vec3 joint1PositionInMeter = ConvertMillimeterToMeter(joint1Position);
    glm::vec3 joint2PositionInMeter = ConvertMillimeterToMeter(joint2Position);
    Visualization::Bone bone;
    bone.Joint1Position = glm::vec3(joint1PositionInMeter.x, joint1PositionInMeter.y, joint1PositionInMeter.z);
    bone.Joint2Position = glm::vec3(joint2PositionInMeter.x, joint2PositionInMeter.y, joint2PositionInMeter.z);
    bone.Color.x = color.r;
    bone.Color.y = color.g;
    bone.Color.z = color.b;
    bone.Color.a = color.a;

    m_window3d.AddBone(bone);
}

void WindowManager::AddBody(const k4abt_body_t& body, Color color)
{
    Color lowConfidenceColor = color;
    lowConfidenceColor.a = color.a / 4;

    for (int joint = 0; joint < static_cast<int>(K4ABT_JOINT_COUNT); joint++)
    {
        if (body.skeleton.joints[joint].confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW)
        {
            const k4a_float3_t& jointPosition = body.skeleton.joints[joint].position;
            const k4a_quaternion_t& jointOrientation = body.skeleton.joints[joint].orientation;

            AddJoint(
                jointPosition,
                jointOrientation,
                body.skeleton.joints[joint].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM ? color : lowConfidenceColor);
        }
    }

    for (size_t boneIdx = 0; boneIdx < g_boneList.size(); boneIdx++)
    {
        k4abt_joint_id_t joint1 = g_boneList[boneIdx].first;
        k4abt_joint_id_t joint2 = g_boneList[boneIdx].second;

        if (body.skeleton.joints[joint1].confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW &&
            body.skeleton.joints[joint2].confidence_level >= K4ABT_JOINT_CONFIDENCE_LOW)
        {
            bool confidentBone = body.skeleton.joints[joint1].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM &&
                body.skeleton.joints[joint2].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM;
            const k4a_float3_t& joint1Position = body.skeleton.joints[joint1].position;
            const k4a_float3_t& joint2Position = body.skeleton.joints[joint2].position;

            AddBone(joint1Position, joint2Position, confidentBone ? color : lowConfidenceColor);
        }
    }
}

void WindowManager::Render()
{
    if (m_pointCloudUpdated || m_pointClouds.size() != 0)
    {
        m_window3d.UpdatePointClouds(m_pointClouds.data(), (uint32_t)m_pointClouds.size(), m_depthBuffer.data(), m_depthWidth, m_depthHeight);
        m_pointClouds.clear();
        m_pointCloudUpdated = false;
    }
    m_window3d.UpdateScene();
    m_window3d.Render();
}

void WindowManager::SetWindowPosition(int xPos, int yPos)
{
    m_window3d.SetWindowPosition(xPos, yPos);
}


void WindowManager::SetLayout3d(Visualization::Layout3d layout3d)
{
    m_window3d.SetLayout3d(layout3d);
}

void WindowManager::SetJointFrameVisualization(bool enableJointFrameVisualization)
{
    Visualization::SkeletonRenderMode skeletonRenderMode = enableJointFrameVisualization ?
        Visualization::SkeletonRenderMode::SkeletonOverlayWithJointFrame : Visualization::SkeletonRenderMode::SkeletonOverlay;

    m_window3d.SetSkeletonRenderMode(skeletonRenderMode);
}

void WindowManager::SetFloorRendering(bool enableFloorRendering, float floorPositionX, float floorPositionY, float floorPositionZ)
{
    glm::vec3 position = glm::vec3(floorPositionX, floorPositionY, floorPositionZ);
    m_window3d.SetFloorRendering(enableFloorRendering, position, glm::quat(1.f, 0.f, 0.f, 0.f ));
}

void WindowManager::SetFloorRendering(bool enableFloorRendering, float floorPositionX, float floorPositionY, float floorPositionZ, float normalX, float normalY, float normalZ)
{
    glm::vec3 position = glm::vec3(floorPositionX, floorPositionY, floorPositionZ);
    glm::vec3  n = glm::vec3(normalX, normalY, normalZ);
    n = glm::normalize(n);
    glm::vec3 up = glm::vec3(0, -1, 0);

    glm::vec3 ax = glm::cross(up, n);
    ax = glm::normalize(ax);

    float ang = acos(glm::dot(up, n));
    float hs = sin(ang / 2);
    glm::quat q = { cos(ang / 2), hs * ax[0], hs * ax[1], hs * ax[2] };
    m_window3d.SetFloorRendering(enableFloorRendering, position, q);
}

void WindowManager::InitializeCalibration(const k4a_calibration_t& sensorCalibration)
{

    m_depthWidth = static_cast<uint32_t>(sensorCalibration.depth_camera_calibration.resolution_width);
    m_depthHeight = static_cast<uint32_t>(sensorCalibration.depth_camera_calibration.resolution_height);

    // Cache the 2D to 3D unprojection table
    EXIT_IF(!CreateXYDepthTable(sensorCalibration), "Create XY Depth Table failed!");
    m_window3d.InitializePointCloudRenderer(
        true,   // Enable point cloud shading for better visualization effect
        reinterpret_cast<float*>(m_xyDepthTable.data()),
        m_depthWidth,
        m_depthHeight);

    // Create transformation handle
    if (m_transformationHandle == nullptr)
    {
        m_transformationHandle = k4a_transformation_create(&sensorCalibration);

        if (m_pointCloudImage == nullptr)
        {
            VERIFY(k4a_image_create(K4A_IMAGE_FORMAT_CUSTOM,
                m_depthWidth,
                m_depthHeight,
                m_depthWidth * 3 * (int)sizeof(int16_t),
                &m_pointCloudImage), "Create Point Cloud Image failed!");
        }
    }
}

glm::vec4 WindowManager::BlendBodyColor(glm::vec4 blendColor, Color bodyColor)
{
    float darkenRatio = 0.8f;
    float instanceAlpha = 0.8f;
    glm::vec4 color;
    color.x = bodyColor.r * instanceAlpha;// +blendColor.x * darkenRatio;
    color.y = bodyColor.g * instanceAlpha;// + blendColor.y * darkenRatio;
    color.z = bodyColor.b * instanceAlpha;// + blendColor.z * darkenRatio;
    color.a = blendColor.a;
    return color;
}

void WindowManager::UpdateDepthBuffer(k4a_image_t depthFrame)
{
    int width = k4a_image_get_width_pixels(depthFrame);
    int height = k4a_image_get_height_pixels(depthFrame);
    uint16_t* depthFrameBuffer = (uint16_t*)k4a_image_get_buffer(depthFrame);
    m_depthBuffer.assign(depthFrameBuffer, depthFrameBuffer + width * height);
}

bool WindowManager::CreateXYDepthTable(const k4a_calibration_t& sensorCalibration)
{
    int width = sensorCalibration.depth_camera_calibration.resolution_width;
    int height = sensorCalibration.depth_camera_calibration.resolution_height;

    m_xyDepthTable.resize(width * height);

    auto xyTablePtr = m_xyDepthTable.begin();

    k4a_float3_t pt3;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            k4a_float2_t pt = { static_cast<float>(w), static_cast<float>(h) };
            int valid = 0;
            k4a_result_t result = k4a_calibration_2d_to_3d(&sensorCalibration,
                &pt,
                1.f,
                K4A_CALIBRATION_TYPE_DEPTH,
                K4A_CALIBRATION_TYPE_DEPTH,
                &pt3,
                &valid);
            if (result != K4A_RESULT_SUCCEEDED)
            {
                return false;
            }

            if (valid == 0)
            {
                // Set the invalid xy table to be (0, 0)
                xyTablePtr->x = 0.f;
                xyTablePtr->y = 0.f;
            }
            else
            {
                xyTablePtr->x = pt3.xyz.x;
                xyTablePtr->y = pt3.xyz.y;
            }

            ++xyTablePtr;
        }
    }

    return true;
}

