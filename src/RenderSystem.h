#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <GLFW\glfw3.h>
#include "Entity.h"
#include "Program.h"
#include <vector>
#include "ShaderLibrary.hpp"

class RenderSystem {
public:
    static RenderSystem& getInstance() {
        static RenderSystem _instance_;
        return(_instance_);
    }

    virtual void init(GLFWwindow* window);
    virtual void process(std::vector<Entity*> entities);
    //virtual void processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader = nullptr);

private:
    GLFWwindow* _mWindow = nullptr;

    int w_width;

    int w_height;
};

#endif