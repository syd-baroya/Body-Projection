// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#ifndef APP_3D_TYPES_H
#define APP_3D_TYPES_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace Visualization
{
    struct PointCloudVertex
    {
        glm::vec3 Position;         // The position of the point cloud vertex specified in meters
        int Animate;
        glm::vec4 Color;
        glm::mat4 Transformations;
        glm::ivec2 PixelLocation;   // Pixel location of point cloud in the depth map (w, h)
    };

    struct MonoVertex
    {
        glm::vec3 Position;         // The position of the mono vertex specified in meters
        glm::vec3 Normal;
    };

    struct ColorVertex
    {
        glm::vec3 Position;         // The position of the color vertex specified in meters
        glm::vec3 Normal;
        glm::vec4 Color;
    };

    struct Joint
    {
        glm::vec3 Position;          // The position of the joint specified in meters
        glm::quat Orientation; // The orientation of the joint specified in normalized quaternion
        glm::vec4 Color;
    };

    struct Bone
    {
        glm::vec3 Joint1Position;
        glm::vec3 Joint2Position;
        glm::vec4 Color;
    };
}
#endif