// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#ifndef RENDERER_BASE_H
#define RENDERER_BASE_H
#include "Program.h"
#include <GLFW/glfw3.h>

namespace Visualization
{
    class RendererBase
    {
    public:
        virtual ~RendererBase() = default;
        virtual void Create(GLFWwindow* window) = 0;
        virtual void Delete() = 0;

        virtual void UpdateViewProjection(
            glm::mat4 view,
            glm::mat4 projection);

        virtual void Render() = 0;

    protected:
        bool m_initialized = false;

        glm::mat4 m_view;
        glm::mat4 m_projection;

        // Basic OpenGL resources
        GLFWwindow* m_window;
        //GLuint m_shaderProgram;
        //GLuint m_vertexShader;
        //GLuint m_fragmentShader;

        Program* VSFSProgram;
        std::string vertShaderPath = "../resources/";
        std::string fragShaderPath = "../resources/";
    };
}
#endif
