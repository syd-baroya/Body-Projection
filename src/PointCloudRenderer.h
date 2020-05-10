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

        void UpdatePointClouds(
            GLFWwindow* window,
            const Visualization::PointCloudVertex* point3ds,
            uint32_t numPoints,
            const uint16_t* depthFrame,
            uint32_t width, uint32_t height,
            bool useTestPointClouds = false);

        void SetShading(bool enableShading);

        void Render() override;
        void Render(int width, int height);

        void ChangePointCloudSize(float pointCloudSize);

    private:
        int ssbo_size = 1024;
        class ssbo_data
        {
        public:
            glm::ivec4 positions_list[1024];
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

        GLuint m_xyTableTextureObject = 0;
        GLuint m_depthTextureObject = 0;

        GLuint m_viewIndex = 0;
        GLuint m_projectionIndex = 0;
        GLuint m_enableShadingIndex = 0;
        GLuint m_xyTableSamplerIndex = 0;
        GLuint m_depthSamplerIndex = 0;
        GLuint m_bufferSize = 0;
        // Lock
        std::mutex m_mutex;

        ComputeProgram* computeProgram;
        std::string computeShaderPath = "../resources/";

        AtomicCounterBuffer* atomicCounterBuff;
        ShaderStorageBuffer* ssBuffObject;
    };
}
#endif
