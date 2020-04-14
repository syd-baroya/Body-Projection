#pragma once

#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <GLFW\glfw3.h>
#include "Scene.h"
#include "Program.h"

class AnimationSystem {
public:
    static AnimationSystem& getInstance() {
        static AnimationSystem _instance_;
        return(_instance_);
    }

    virtual void init(GLFWwindow* window);
    virtual void process(SceneComponent& scene);
    virtual void processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader = nullptr);

};

#endif