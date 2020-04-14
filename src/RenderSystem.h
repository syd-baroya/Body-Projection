#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <GLFW\glfw3.h>
#include "Scene.h"
#include "Program.h"

class Render_System {
public:
    static Render_System& getInstance() {
        static Render_System _instance_;
        return(_instance_);
    }

    virtual void init(GLFWwindow* window);
    virtual void process(SceneComponent& scene);
    virtual void processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader = nullptr);

};

#endif