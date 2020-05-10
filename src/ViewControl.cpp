// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "ViewControl.h"



using namespace glm;

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

static inline float DegreesToRadians(float angleInDegrees)
{
    return (float)(angleInDegrees / 180.f * M_PI);
}

// Default camera values
const float kDefaultSensitivity = 0.2f;
const float kDefaultFOV = 65.0f;                // FOV (degrees)

const float kFullPerspectiveFactor = 1.0f;      // Perspective projection
const float kLeastPerspectiveFactor = 0.1f;     // Orthographic projection
const float kDefaulPerspectiveFactor = kFullPerspectiveFactor;
const float kObserverPerspectiveFactor = 0.3f;

// K4A coordinate system uses OpenCV camera convention: X-right, Y-down, Z-forward
const float kTargetDepth = 1.5f;

const ViewParameters kDefaultView(
    0.f, 0.f, kTargetDepth,                     // Target depth (meters)
    0.f, -1.f, 0.f,                             // WorldUp
    0.f, 0.f);                                  // Yaw and Pitch (degrees)

const ViewParameters kRightView(
    0.f, 0.f, kTargetDepth,                     // Target depth (meters)
    0.f, -1.f, 0.f,                             // WorldUp
    75.f, 10.0f);                               // Yaw and Pitch (degrees)

const ViewParameters kBackView(
    0.f, 0.f, kTargetDepth,                     // Target depth (meters)
    0.f, -1.f, 0.f,                             // WorldUp
    180.f, 0.f);                                // Yaw and Pitch (degrees)

const ViewParameters kLeftView(
    0.f, 0.f, kTargetDepth,                     // Target depth (meters)
    0.f, -1.f, 0.f,                             // WorldUp
    -75.0f, 10.0f);                             // Yaw and Pitch (degrees)

const ViewParameters kTopView(
    0.f, 0.f, kTargetDepth,                     // Target depth (meters)
    0.f, -1.f, 0.f,                             // WorldUp
    0.f, 45.0f);                                // Yaw and Pitch (degrees)

ViewParameters::ViewParameters(const ViewParameters& v)
{
    targetPos = vec3(v.targetPos.x, v.targetPos.y, v.targetPos.z);
    targetDepth = v.targetDepth;
    yaw = v.yaw;
    pitch = v.pitch;
    front = vec3(v.front.x, v.front.y, v.front.z);
    up = vec3(v.up.x, v.up.y, v.up.z);
    right = vec3(v.right.x, v.right.y, v.right.z);
    worldUp = vec3(v.worldUp.x, v.worldUp.y, v.worldUp.z);
}

ViewParameters::ViewParameters(
    float targetX, float targetY, float targetZ,
    float upX, float upY, float upZ,
    float _yaw, float _pitch)
{
    targetPos = vec3(targetX, targetY, targetZ);
    targetDepth = length(targetPos);
    worldUp = vec3(upX, upY, upZ);
    yaw = _yaw;
    pitch = _pitch;
    UpdateRotationVectors();
}

// Update camera position and rotation vectors from Euler angles and target point.
// It needs to be called every time after updating the primary camera attributes.
void ViewParameters::UpdateRotationVectors()
{
    // Calculate the new front vector (relative to +Z)
    vec3 frontTemp;
    frontTemp[0] = (float)(sin(DegreesToRadians(yaw)) * cos(DegreesToRadians(pitch)));
    frontTemp[1] = (float)(sin(DegreesToRadians(pitch)));
    frontTemp[2] = (float)(cos(DegreesToRadians(yaw)) * cos(DegreesToRadians(pitch)));

    front = normalize(frontTemp);

    // Also re-calculate the Right and Up vector
    vec3 rightTemp;
    rightTemp = cross(front, worldUp);
    right =  normalize(rightTemp); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

    vec3 upTemp;
    upTemp = cross(right, front);
    up = normalize(upTemp);
}

void ViewParameters::PrintViewInfo()
{
    printf("---------------------------------------\n");
    printf("TargetPos: %f, %f, %f;\n", targetPos[0], targetPos[1], targetPos[2]);
    printf("Front: %f, %f, %f;\n", front[0], front[1], front[2]);
    printf("Right: %f, %f, %f;\n", right[0], right[1], right[2]);
    printf("Up: %f, %f, %f;\n", up[0], up[1], up[2]);
    printf("WorldUp: %f, %f, %f;\n", worldUp[0], worldUp[1], worldUp[2]);
    printf("Yaw: %f; Pitch: %f;\n", yaw, pitch);
}

ViewControl::ViewControl()
    : m_viewParams(kDefaultView)
    , m_mouseSensitivity(kDefaultSensitivity)
    , m_defaultFOV(kDefaultFOV)
    , m_perspectiveFactor(kDefaulPerspectiveFactor)
{
}

void ViewControl::SetDefaultVerticalFOV(float degrees)
{
    m_defaultFOV = degrees;
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
mat4 ViewControl::GetViewMatrix()
{
    // Update position
    vec3 v = m_viewParams.front * PerspectiveTargetDepthForRendering();
    vec3 pos = m_viewParams.targetPos - v;

    return lookAt(pos, m_viewParams.targetPos, m_viewParams.up);
}

mat4 ViewControl::GetPerspectiveMatrix()
{
    mat4 perspectiveMatrix = perspective(DegreesToRadians(PerspectiveFOVForRendering()), m_viewport.width / (float)m_viewport.height, 0.1f, 150.f);

    if (m_enableMirrorMode)
    {
        perspectiveMatrix[0][0] = -perspectiveMatrix[0][0];
    }
    return perspectiveMatrix;
}

float ViewControl::PerspectiveTargetDepthForRendering()
{
    return m_viewParams.targetDepth / m_perspectiveFactor;
}

float ViewControl::PerspectiveFOVForRendering()
{
    return m_defaultFOV * m_perspectiveFactor;
}

vec3 ViewControl::GetTargetPosition( )
{
    return vec3(m_viewParams.targetPos.x, m_viewParams.targetPos.y, m_viewParams.targetPos.z);
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void ViewControl::ProcessRotationalMovement(const vec2 screenOffset)
{
    float xoffset = screenOffset.x;
    float yoffset = screenOffset.y;

    if (m_enableMirrorMode)
    {
        xoffset = -xoffset;
    }

    m_viewParams.yaw += xoffset * m_mouseSensitivity;
    m_viewParams.pitch -= yoffset * m_mouseSensitivity;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    m_viewParams.pitch = std::clamp(m_viewParams.pitch, -89.5f, 89.5f);

    // Update m_viewParams.front, right and up Vectors using the updated Euler angles
    m_viewParams.UpdateRotationVectors();
}

void ViewControl::ProcessPositionalMovement(const vec2 startScreenPos, const vec2 endScreenPos)
{
    vec3 startRay = UnprojectFromScreen(startScreenPos, PerspectiveTargetDepthForRendering());
    vec3 endRay = UnprojectFromScreen(endScreenPos, PerspectiveTargetDepthForRendering());

    vec3 offset = endRay - startRay;

    vec3 oldTargetPos = vec3(m_viewParams.targetPos.x, m_viewParams.targetPos.y, m_viewParams.targetPos.z);
    m_viewParams.targetPos = oldTargetPos - offset;
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void ViewControl::ProcessMouseScroll(GLFWwindow* window, float yoffset)
{
    const bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    if (shift)
    {
        m_perspectiveFactor += yoffset * m_mouseSensitivity;
        m_perspectiveFactor = std::clamp(m_perspectiveFactor, kLeastPerspectiveFactor, kFullPerspectiveFactor);
    }
    else
    {
        m_viewParams.targetDepth += yoffset * m_mouseSensitivity;
        m_viewParams.targetDepth = std::clamp(m_viewParams.targetDepth, 0.2f, 10.0f);
    }
}

void ViewControl::SetViewTarget(const vec3 target)
{
    m_viewParams.targetPos = vec3(target.x, target.y, target.z);
}

void ViewControl::Reset()
{
    m_viewParams = kDefaultView;
    m_perspectiveFactor = kDefaulPerspectiveFactor;
}

void ViewControl::SetViewPoint(ViewPoint viewPoint)
{
    switch (viewPoint)
    {
    case ViewPoint::FrontView:
        m_viewParams = kDefaultView;
        m_perspectiveFactor = kDefaulPerspectiveFactor;
        break;
    case ViewPoint::RightView:
        m_viewParams = kRightView;
        m_perspectiveFactor = kObserverPerspectiveFactor;
        break;
    case ViewPoint::LeftView:
        m_viewParams = kLeftView;
        m_perspectiveFactor = kObserverPerspectiveFactor;
        break;
    case ViewPoint::BackView:
        m_viewParams = kBackView;
        m_perspectiveFactor = kObserverPerspectiveFactor;
        break;
    case ViewPoint::TopView:
        m_viewParams = kTopView;
        m_perspectiveFactor = kObserverPerspectiveFactor;
        break;
    }
}

bool ViewControl::ProjectToScreen(vec2* screen, const vec3 viewPoint)
{
    vec4 p = vec4(viewPoint.x, viewPoint.y, viewPoint.z, 1.0f);

    mat4 projectionMatrix = GetPerspectiveMatrix();
    mat4 viewMatrix = GetViewMatrix();
    mat4 M = projectionMatrix * viewMatrix;

    vec4 r = M * p;
    if (r.a == 0)
    {
        return false;
    }

    screen->x = (1.f + r.x / r.a) / 2.f * m_viewport.width + m_viewport.x + 0.5f;
    screen->y = (1.f + r.y / r.a) / 2.f * m_viewport.height + m_viewport.y + 0.5f;
    return true;
}

vec3 ViewControl::UnprojectFromScreen(const vec2 screen, float zDepth)
{
    vec4 s;
    s[0] = (screen[0] - 0.5f - m_viewport.x) / m_viewport.width * 2.f - 1.f;
    s[1] = (screen[1] - 0.5f - m_viewport.y) / m_viewport.height * 2.f - 1.f;
    s[2] = 0.0f;
    s[3] = 1.0f;

    vec4 r = s * zDepth;

    mat4x4 projectionMatrix =  GetPerspectiveMatrix();
    mat4x4 viewMatrix = GetViewMatrix();
    mat4x4 M = projectionMatrix * viewMatrix;
    mat4x4 invM = inverse(M);

    return invM * r;
}

bool Viewport::ContainsScreenPoint(vec2 screenPos) const
{
    int vx = (int)floor(screenPos[0]) - x;
    int vy = (int)floor(screenPos[1]) - y;
    return 0 <= vx && vx < width && 0 <= vy && vy < height;
}
