#include "RenderSystem.h"

void RenderSystem::init(GLFWwindow* window)
{
    _mWindow = window;

    glfwGetFramebufferSize(_mWindow, &w_width, &w_height);

    glViewport(0, 0, w_width, w_height);

    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.279755f, 0.271146f, 0.2803f, 1.0f);
}

void RenderSystem::process(std::vector<Entity*> entities) {

    // Clear framebuffer.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLibrary& shlib = ShaderLibrary::getInstance();

    Program* prog = shlib.getPtr("prog");

    for (Entity* entity : entities)
    {
        entity->draw(prog);
    }

}

//void RenderSystem::processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader)
//{
//}
