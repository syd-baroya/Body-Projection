#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Entity.h"
//#include "Program.h"
#include <vector>
#include "ShaderLibrary.hpp"
#include "SceneComponent.h"
#include <GLFW/glfw3.h>
#include "AnimationComponent.h"

class RenderSystem {
public:
    static RenderSystem& getInstance() {
        static RenderSystem _instance_;
        return(_instance_);
    }

    void init(GLFWwindow* window);
    void process(SceneComponent* scene, AnimationComponent* anim, std::vector<Entity*> entities, double frametime);
    //virtual void processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader = nullptr);

private:
    GLFWwindow* _mWindow = nullptr;

    int w_width;

    int w_height;
};

#endif