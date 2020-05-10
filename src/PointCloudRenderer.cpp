// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "PointCloudRenderer.h"


using namespace glm;
using namespace Visualization;

PointCloudVertex testVertices[] =
{
    {vec3(-0.5f, -0.5f, -2.5f), vec4(1.0f, 0.0f, 0.0f, 1.0f), ivec2(10, 0)},
    {vec3(0.5f, -0.5f, -2.5f), vec4(0.0f, 1.0f, 0.0f, 1.0f), ivec2(20, 0)},
    {vec3(-0.5f,  0.5f, -2.5f), vec4(0.0f, 0.0f, 1.0f, 1.0f), ivec2(30, 0)},
    { vec3(0.5f,  0.5f, -2.5f), vec4(1.0f, 1.0f, 0.0f, 1.0f), ivec2(40, 0) },

    { vec3(-0.5f, -0.5f, -3.5f), vec4(0.0f, 1.0f, 1.0f, 1.0f), ivec2(50, 0)},
    { vec3(0.5f, -0.5f, -3.5f), vec4(1.0f, 0.0f, 1.0f, 1.0f), ivec2(60, 0)},
    { vec3(-0.5f,  0.5f, -3.5f), vec4(1.0f, 1.0f, 0.5f, 1.0f), ivec2(70, 0)},
    { vec3(0.5f,  0.5f, -3.5f), vec4(0.5f, 0.5f, 1.0f, 1.0f), ivec2(80, 0)}
};

PointCloudRenderer::PointCloudRenderer()
{
    m_view = mat4(1);
    m_projection = mat4(1);

    vertShaderPath += "point_cloud.vert";
    fragShaderPath += "point_cloud.frag";
}

PointCloudRenderer::~PointCloudRenderer()
{
    Delete();
}

void PointCloudRenderer::Create(GLFWwindow* window)
{
    CheckAssert(!m_initialized);
    m_initialized = true;

    m_window = window;
    glfwMakeContextCurrent(window);

    // Context Settings
    glEnable(GL_PROGRAM_POINT_SIZE);

    VSFSProgram = new Program(vertShaderPath, fragShaderPath);

   /* m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertexShaderSources[] = { glslShaderVersion, glslPointCloudVertexShader };
    int numVertexShaderSources = sizeof(vertexShaderSources) / sizeof(*vertexShaderSources);
    glShaderSource(m_vertexShader, numVertexShaderSources, vertexShaderSources, NULL);
    glCompileShader(m_vertexShader);
    ValidateShader(m_vertexShader);

    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragmentShaderSources[] = { glslShaderVersion, glslPointCloudFragmentShader };
    int numFragmentShaderSources = sizeof(fragmentShaderSources) / sizeof(*fragmentShaderSources);
    glShaderSource(m_fragmentShader, numFragmentShaderSources, fragmentShaderSources, NULL);
    glCompileShader(m_fragmentShader);
    ValidateShader(m_fragmentShader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);
    ValidateProgram(m_shaderProgram);*/

    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    glGenBuffers(1, &m_vertexBufferObject);
    m_viewIndex = glGetUniformLocation(VSFSProgram->getPID(), "view");
    m_projectionIndex = glGetUniformLocation(VSFSProgram->getPID(), "projection");
    m_enableShadingIndex = glGetUniformLocation(VSFSProgram->getPID(), "enableShading");
    m_xyTableSamplerIndex = glGetUniformLocation(VSFSProgram->getPID(), "xyTable");
    m_depthSamplerIndex = glGetUniformLocation(VSFSProgram->getPID(), "depth");
}

void PointCloudRenderer::Delete()
{
    if (!m_initialized)
    {
        return;
    }

    m_initialized = false;
    glDeleteBuffers(1, &m_vertexBufferObject);

    VSFSProgram->Delete();
}

void PointCloudRenderer::InitializeDepthXYTable(const float* xyTableInterleaved, uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_xyTableTextureObject);
    glBindTexture(GL_TEXTURE_2D, m_xyTableTextureObject);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, m_width, m_height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RG, GL_FLOAT, xyTableInterleaved);

    glGenTextures(1, &m_depthTextureObject);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void PointCloudRenderer::UpdatePointClouds(
    GLFWwindow* window,
    const PointCloudVertex* point3ds,
    uint32_t numPoints,
    const uint16_t* depthFrame,
    uint32_t width, uint32_t height,
    bool useTestPointClouds)
{
    if (window != m_window)
    {
        Create(window);
    }

    if (m_width != width && m_height != height)
    {
        Fail("Width and Height (%u, %u) does not match the DepthXYTable settings: (%u, %u) are expected!", width, height, m_width, m_height);
    }

    glBindImageTexture(0, m_xyTableTextureObject, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG32F);

    glBindTexture(GL_TEXTURE_2D, m_depthTextureObject);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16UI, m_width, m_height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_SHORT, depthFrame);
    glBindImageTexture(1, m_depthTextureObject, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R16UI);

    glBindVertexArray(m_vertexArrayObject);
    // Create buffers and bind the geometry
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

    if (!useTestPointClouds)
    {
        glBufferData(GL_ARRAY_BUFFER, numPoints * sizeof(PointCloudVertex), point3ds, GL_STREAM_DRAW);
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(testVertices), testVertices, GL_STREAM_DRAW);
    }

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointCloudVertex), (void*)0);
    // Vertex Colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PointCloudVertex), (void*)offsetof(PointCloudVertex, Color));
    // Vertex Pixel Location
    // Notice: For GL_INT type, we need to use glVertexAttribIPointer instead of glVertexAttribPointer
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 2, GL_INT, sizeof(PointCloudVertex), (void*)offsetof(PointCloudVertex, PixelLocation));

    glBindVertexArray(0);

    m_drawArraySize = useTestPointClouds ? 8 : GLsizei(numPoints);
}

void PointCloudRenderer::SetShading(bool enableShading)
{
    m_enableShading = enableShading;
}

void PointCloudRenderer::Render()
{
    std::array<int, 4> data; // x, y, width, height

    glGetIntegerv(GL_VIEWPORT, data.data());
    Render(data[2], data[3]);
}

void PointCloudRenderer::Render(int width, int height)
{
    glEnable(GL_DEPTH_TEST);
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float pointSize;
    if (m_pointCloudSize)
    {
        pointSize = m_pointCloudSize.value();
    }
    else if (m_width == 0 || m_height == 0)
    {
        pointSize = m_defaultPointCloudSize;
    }
    else
    {
        pointSize = std::min(2.f * width / (float)m_width, 2.f * height / (float)m_height);
    }
    glPointSize(pointSize);

    //glUseProgram(m_shaderProgram);
    VSFSProgram->bind();

    // Update model/view/projective matrices in shader
    glUniformMatrix4fv(m_viewIndex, 1, GL_FALSE, &m_view[0][0]);
    glUniformMatrix4fv(m_projectionIndex, 1, GL_FALSE, &m_projection[0][0]);

    // Update render settings in shader
    glUniform1i(m_enableShadingIndex, (GLint)m_enableShading);

    // Render point cloud
    glBindVertexArray(m_vertexArrayObject);
    glDrawArrays(GL_POINTS, 0, m_drawArraySize);
    glBindVertexArray(0);
    
    VSFSProgram->unbind();
}

void PointCloudRenderer::ChangePointCloudSize(float pointCloudSize)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_pointCloudSize = pointCloudSize;
}
