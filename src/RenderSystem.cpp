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

void RenderSystem::process(SceneComponent* scene, AnimationComponent* anim, std::vector<Entity*> entities, double frametime) {

    // Clear framebuffer.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLibrary& shlib = ShaderLibrary::getInstance();

    Program* prog = shlib.getPtr("prog");

    for (Entity* entity : entities)
    {
        entity->addComponent<SceneComponent>(scene);
        entity->addComponent<AnimationComponent>(anim);

        entity->update(frametime);
    }

    for (Entity* entity : entities)
    {
        prog->bind();
        entity->draw(prog);
        prog->unbind();
    }

}

//void RenderSystem::processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader)
//{
//}
