// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#ifndef POINT_CLOUD_RENDERER_H
#define POINT_CLOUD_RENDERER_H
#include <mutex>
#include <array>

#include "RendererBase.h"
#include "Utilities.h"
#include "App3DTypes.h"
#include <optional>
#include "SceneComponent.h"


namespace Visualization
{
    class PointCloudRenderer : public RendererBase
    {
    public:

        PointCloudRenderer();
        ~PointCloudRenderer();
        void Create(GLFWwindow* window)  override;
        void Delete() override;

        void InitializeDepthXYTable(const float* xyTableInterleaved, uint32_t width, uint32_t height);

        void InitializeSSBO();

        void addColor(glm::vec4 color, bool in_point_cloud);

        void UpdatePointClouds(
            GLFWwindow* window,
            Visualization::PointCloudVertex* point3ds,
            uint32_t numPoints,
            const uint16_t* depthFrame,
            uint32_t width, uint32_t height,
            bool useTestPointClouds = false);

        void SetShading(bool enableShading);

        void Render() override;
        void Render(SceneComponent* scene, int width, int height);

        void ChangePointCloudSize(float pointCloudSize);

    private:

        /*
        * make note in header class that we change the max point cloud size (aka resolution)
        * based on the depth mode the that is chosen. It can be dynamically changed on CPU side
        * but must be explicitly changed in the compute shader
        */
        int ssbo_index = 0;
        class ssbo_data
        {
        public:
            glm::vec4 colorInput[320 * 288];
            glm::vec4 colorOutput[320 * 288];
            int colorIndex[320 * 288] = { 0 };
        };
        ssbo_data ssbo_CPUMEM;
        // Render settings
        const GLfloat m_defaultPointCloudSize = 0.5f;
        std::optional<GLfloat> m_pointCloudSize;
        bool m_enableShading = false;

        // Point Array Size
        GLsizei m_drawArraySize = 0;

        // Depth Frame Information
        uint32_t m_width = 0;
        uint32_t m_height = 0;

        // OpenGL resources
        GLuint m_vertexArrayObject = 0;
        GLuint m_vertexBufferObject = 0;
        GLuint billboard_vertex_buffer = 0;


        GLuint m_xyTableTextureObject = 0;
        GLuint m_depthTextureObject = 0;

        GLuint m_viewIndex = 0;
        GLuint m_projectionIndex = 0;
        GLuint m_enableShadingIndex = 0;
        GLuint m_xyTableSamplerIndex = 0;
        GLuint m_depthSamplerIndex = 0;
        GLuint m_sceneTextureIndex = 0;
        // Lock
        std::mutex m_mutex;

        ComputeProgram* computeProgram;
        std::string computeShaderPath = "../resources/";

        AtomicCounterBuffer* atomicCounterBuff;
        ShaderStorageBuffer* ssBuffObject;
    };
}
#endif
