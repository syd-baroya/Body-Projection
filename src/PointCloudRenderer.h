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

  //      void addColor(glm::vec4 color, bool in_point_cloud);
		//bool pixelDying(int pixelIndex);
        void update(float frametime);

        void UpdatePointClouds(
            GLFWwindow* window, 
            PointCloudVertex* point3ds, 
            uint32_t numPoints, 
            const uint16_t* depthFrame, 
            uint32_t width, uint32_t height, 
            float maxDepthZ, float minDepthZ,
            bool drawOnlyPointCloudOutline = false,
            bool useTestPointClouds = false);

        void SetShading(bool enableShading);

        void Render() override;
        void Render(int width, int height);

        void ChangePointCloudSize(float pointCloudSize);

        GLuint getPointCloudMask();

    private:
        std::ofstream myfile;

        std::vector<PointCloudVertex> pointCloudOutline;
        std::vector<PointCloudVertex> allPixelsInOutline;
        /*
        * make note in header class that we change the max point cloud size (aka resolution)
        * based on the depth mode the that is chosen. It can be dynamically changed on CPU side
        * but must be explicitly changed in the compute shader
        */
        float m_elapsedTime = 0.0f;
        //int ssbo_index = 0;
        class ssbo_data
        {
        public:
            glm::vec4 colorInput[320 * 288];
          /*  int outlineIndices[(320 * 288) / 2];
            int last_index = 0;*/
        };
        //int colorIndex[320 * 288] = { 0 };
        ssbo_data ssbo_CPUMEM;

        std::vector<PointCloudVertex> animatingPixels;
        bool resetAnimators = true;

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
        GLuint m_vertexTrans = 0;
        GLuint m_outlineBuffer = 0;
        GLuint billboard_position_buffer = 0;
        GLuint billboard_texture_buffer = 0;
        GLuint VertexArrayIDBox = 0;
        GLuint VertexBufferIDBox = 0;
        GLuint VertexBufferTex = 0;
        GLuint fb = 0;
        GLuint FBOcol1 = 0;
        GLuint FBOcol2 = 0;
        GLuint depth_rb = 0;

        GLuint m_xyTableTextureObject = 0;
        GLuint m_depthTextureObject = 0;

        // Lock
        std::mutex m_mutex;

        ComputeProgram* computeProgram;
        std::string computeShaderPath = "../resources/";

        Program* blendProgram;
        std::string blendVertPath = "../resources/";
        std::string blendFragPath = "../resources/";

        AtomicCounterBuffer* atomicCounterBuff;
        ShaderStorageBuffer* ssBuffObject;
    };
}
#endif
