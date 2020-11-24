// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "BodyTrackingHelpers.h"

#include "WindowController.h"
#include <k4a/k4a.h>

// This is a wrapper library that convert the types from the k4abt types to the window3d visualization library types
class WindowManager
{
public:
    ~WindowManager();

    // Create Window3d wrapper without point cloud shading
    void Create(
        const char* name,
        k4a_depth_mode_t depthMode,
        int windowWidth = -1,
        int windowHeight = -1);

    // Create Window3d wrapper with point cloud shading
    void Create(
        const char* name,
        const k4a_calibration_t& sensorCalibration);

    //void SetCloseCallback(
    //    Visualization::CloseCallbackType closeCallback,
    //    void* closeCallbackContext = nullptr);

    //void SetKeyCallback(
    //    Visualization::KeyCallbackType keyCallback,
    //    void* keyCallbackContext = nullptr);

    void Delete();

    void UpdatePointClouds(k4a_image_t depthImage, std::vector<Color> pointCloudColors = std::vector<Color>());

    void CleanJointsAndBones();

    void AddJoint(k4a_float3_t position, k4a_quaternion_t orientation, Color color);

    void AddBone(k4a_float3_t joint1Position, k4a_float3_t joint2Position, Color color);

    // Helper function to directly add the whole body for rendering instead of adding separate joints and bones
    void AddBody(const k4abt_body_t& body, Color color);

    void Render();

    // Window Configuration Functions
    void SetFloorRendering(bool enableFloorRendering, float floorPositionX, float floorPositionY, float floorPositionZ);
    void SetFloorRendering(bool enableFloorRendering, float floorPositionX, float floorPositionY, float floorPositionZ, float normalX, float normalY, float normalZ);

    void SetWindowPosition(int xPos, int yPos);

    // Render Setting Functions
    void SetLayout3d(Visualization::Layout3d layout3d);
    void SetJointFrameVisualization(bool enableJointFrameVisualization);
    Visualization::WindowController m_window3d;


private:
    void InitializeCalibration(const k4a_calibration_t& sensorCalibration);

    glm::vec4 BlendBodyColor(glm::vec4 blendColor, Color bodyColor);

    void UpdateDepthBuffer(k4a_image_t depthImage);

    bool CreateXYDepthTable(const k4a_calibration_t& sensorCalibration);

private:

    bool m_pointCloudUpdated = false;
    std::vector<uint16_t> m_depthBuffer;
    std::vector<Visualization::PointCloudVertex> m_pointClouds;
    float maxDepthZ = 0.f;
    float minDepthZ = 500.f;

    struct XY
    {
        float x;
        float y;
    };
    std::vector<XY> m_xyDepthTable;
    uint32_t m_depthWidth = 0;
    uint32_t m_depthHeight = 0;
    k4a_transformation_t m_transformationHandle = nullptr;
    k4a_image_t m_pointCloudImage = nullptr;
};

#endif