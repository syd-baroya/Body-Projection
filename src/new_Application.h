

#pragma once

#ifndef APPLICATION_H

#define APPLICATION_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include <string>

#include <array>

#include "Shape.h"

#include "RenderSystem.h"

#include "CameraEntity.h"

#include <iostream>
#include "Time.h"
#include "KinectSystem.h"
#include "Framebuffer.h"

/**

 * Application.h adapted from Kolton Yager's Application.hpp

 */

class StartupState {
public:
    StartupState() {}
    StartupState(int argc, char** argv);

    bool developer_mode = false;
    std::vector<std::string> input_paths;

protected:
    void parseArgs(int argc, char** argv);

    std::vector <std::string>::iterator parseOption(
        std::vector<std::string>::iterator carg,
        const std::vector<std::string>& args
    );

    std::vector<std::string>::iterator parseInput(
        std::vector<std::string>::iterator carg,
        const std::vector<std::string>& args
    );
};

class Application {

public:

    static Application& getInstance() {
        static Application instance;
        return(instance);
    }

    void init(int argc, char** argv);
    void run();
    void cleanup();

    const GLFWwindow* getWindow() const { return (_window); }
    bool shouldExit() const;
    bool isFullscreen();
    void setFullScreen(bool fullscreen);
    glm::ivec2 getCurrScreenSize();

    std::array< int, 2 > _wndPos{ 0, 0 };
    std::array< int, 2 > _wndSize{ 0, 0 };

    Application(Application const&) = delete;

    void operator=(Application const&) = delete;

    CameraEntity camera;

protected:

    Application() {}

    void initGL();
    void initGLFW();
    void initShaders();
    void initScene();

    void initGeom();

    void initProgs();

    void generateFramebuffers();

    std::vector<TrackedBodyEntity*> body_entities;
    std::unordered_map<std::string, TexturedMeshEntity*> fbo_entities;
    std::vector<SceneComponent> scene_comps;
    std::vector<AnimationComponent> anim_comps;
    std::unordered_map<std::string, Framebuffer*> frame_buffers;
    //ShaderStorageBuffer ssbo;
    ssbo_data ssbo_CPUMEM;

    struct Configuration {
        StartupState startup_state;
        int w_width = 1280;
        int w_height = 800;
    } config;

    bool m_should_exit = false;
    GLFWwindow* _window = nullptr;
    GLFWmonitor* _monitor = nullptr;

private:
    ShaderLibrary& shlib = ShaderLibrary::getInstance();
    bool fullscreen_mode = false;

};

#endif