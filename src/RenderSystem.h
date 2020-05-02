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
#include "Framebuffer.h"
#include "TrackedBodyEntity.h"

class RenderSystem {
public:
    static RenderSystem& getInstance() {
        static RenderSystem _instance_;
        return(_instance_);
    }

    void init(GLFWwindow* window);

    void process(SceneComponent* scene, AnimationComponent* anim, std::vector<TrackedBodyEntity*> body_entities, std::unordered_map<std::string, TexturedMeshEntity*> fbo_entities, 
        std::unordered_map<std::string, Framebuffer*> frame_buffers, ivec2 screensize, double frametime, double time_since_last_body_tracked, int bodytracked, TextureBuffer* depth_image);

    //virtual void processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader = nullptr);

private:

    void processFireToFBO(TexturedMeshEntity* screen_entity, AnimationComponent* anim, std::vector<TrackedBodyEntity*> body_entities, ShaderLibrary& shlib, Framebuffer* fb_to_write, Framebuffer* fb_to_draw, double frametime, ivec2 screensize);
    void processBodyToFBO(SceneComponent* scene, std::vector<TrackedBodyEntity*> body_entities, ShaderLibrary& shlib, Framebuffer* fb_to_write, double frametime, ivec2 screensize);
	//void processFBOtoScreen(SceneComponent* scene, TexturedMeshEntity* screen_entity, Framebuffer* fb_to_draw, ShaderLibrary& shlib, ivec2 screensize, bool black);
    void processFBOtoScreen(TexturedMeshEntity* screen_entity, Framebuffer* fb_to_draw, ShaderLibrary& shlib, ivec2 screensize, bool black);

	void processDepthtoScreen(TextureBuffer* depth_image, TexturedMeshEntity* screen_entity, ShaderLibrary& shlib, ivec2 screensize);


    GLFWwindow* _mWindow = nullptr;

    int w_width;

    int w_height;
};

#endif