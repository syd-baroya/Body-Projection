#include "RenderSystem.h"
#include "Framebuffer.h"
#include "TrackedBodyEntity.h"

void RenderSystem::init(GLFWwindow* window)
{
    _mWindow = window;

    glfwGetFramebufferSize(_mWindow, &w_width, &w_height);

    glViewport(0, 0, w_width, w_height);

    //glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void processFireToFBO(TexturedMeshEntity* screen_entity, AnimationComponent* anim, std::vector<Entity*> entities, ShaderLibrary& shlib, Framebuffer* fb_to_write, Framebuffer* fb_to_draw, double frametime, ivec2 screensize)
{
    fb_to_write->setDrawBuffers(2);
    glViewport(0, 0, screensize.x, screensize.y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Program* screenproc = shlib.getPtr("screenproc");
    screenproc->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_to_draw->getFBO("FBOcolorbut").getTextureID());
    screen_entity->draw(screenproc);

    Program* progfire = shlib.getPtr("progfire");
    for (Entity* entity : entities)
    {
        
        entity->update(frametime);
        progfire->bind();
        entity->draw(progfire);
        anim->draw(progfire);
        progfire->unbind();
    }

    fb_to_write->writeToDrawBuffers();

}

void processBodyToFBO(SceneComponent* scene, std::vector<Entity*> entities, ShaderLibrary& shlib, Framebuffer* fb_to_write, double frametime, ivec2 screensize)
{
    fb_to_write->setDrawBuffers(1);
    glViewport(0, 0, screensize.x, screensize.y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Program* progbody = shlib.getPtr("progbody");
    for (Entity* entity : entities)
    {

        entity->update(frametime);
        progbody->bind();
        entity->draw(progbody);
        scene->draw(progbody);
        progbody->unbind();
    }

    fb_to_write->writeToDrawBuffers();
}

void processFBOtoScreen(TexturedMeshEntity* screen_entity, Framebuffer* fb_to_draw, ShaderLibrary& shlib, ivec2 screensize, bool black)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, screensize.x, screensize.y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Program* postprog = shlib.getPtr("postprog");
    postprog->bind();
    if (black)
    {
        glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        glActiveTexture(GL_TEXTURE0);		glBindTexture(GL_TEXTURE_2D, fb_to_draw->getFBO("FBOcolor").getTextureID());
        glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, fb_to_draw->getFBO("FBOmask").getTextureID());
    }
    screen_entity->draw(postprog);
    postprog->unbind();

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
