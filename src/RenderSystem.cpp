#include "RenderSystem.h"


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

void RenderSystem::processFireToFBO(TexturedMeshEntity* screen_entity, AnimationComponent* anim, std::vector<TrackedBodyEntity*> body_entities, ShaderLibrary& shlib, Framebuffer* fb_to_write, Framebuffer* fb_to_draw, double frametime, ivec2 screensize)
{
    fb_to_write->setDrawBuffers(2);
    glViewport(0, 0, screensize.x, screensize.y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Program* screenproc = shlib.getPtr(screen_entity->getProgName());
    screenproc->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_to_draw->getFBO("FBOcolorbut").getTextureID());
    screen_entity->draw(screenproc);

    Program* progfire = shlib.getPtr("progfire");
    for (TrackedBodyEntity* entity : body_entities)
    {
        anim->update(frametime);
        entity->update(frametime);
        progfire->bind();
        anim->draw(progfire);
        entity->draw(progfire);
        progfire->unbind();
    }

    fb_to_write->writeToDrawBuffers();

}

void RenderSystem::processBodyToFBO(SceneComponent* scene, std::vector<TrackedBodyEntity*> body_entities, ShaderLibrary& shlib, Framebuffer* fb_to_write, double frametime, ivec2 screensize)
{
    //fb_to_write->setDrawBuffers(1);
    glViewport(0, 0, screensize.x, screensize.y);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Program* progbody = shlib.getPtr("progbody");
    for (TrackedBodyEntity* entity : body_entities)
    {
        scene->update(frametime);
        entity->update(frametime);
        progbody->bind();
        scene->draw(progbody);

        entity->draw(progbody);
        progbody->unbind();
    }

    //fb_to_write->writeToDrawBuffers();
}

void RenderSystem::processFBOtoScreen(TexturedMeshEntity* screen_entity, Framebuffer* fb_to_draw, ShaderLibrary& shlib, ivec2 screensize, bool black)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glViewport(0, 0, screensize.x, screensize.y);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Program* postprog = shlib.getPtr(screen_entity->getProgName());
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

void RenderSystem::process(SceneComponent* scene, AnimationComponent* anim, std::vector<TrackedBodyEntity*> body_entities, std::unordered_map<std::string,
    TexturedMeshEntity*> fbo_entities, std::unordered_map<std::string, Framebuffer*> frame_buffers, ivec2 screensize, double frametime, double time_since_last_body_tracked, int bodytracked)
{

    // Clear framebuffer.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ShaderLibrary& shlib = ShaderLibrary::getInstance();

    if (bodytracked > 0)
    {
        processBodyToFBO(scene, body_entities, shlib, frame_buffers.at("fbbut"), frametime, screensize);
        //processFireToFBO(fbo_entities.at("rect"), anim, body_entities, shlib, frame_buffers.at("fb"), frame_buffers.at("fbbut"), frametime, screensize);
    }

    bool black = false;
    if (time_since_last_body_tracked > 1.0)
        black = true;
    //processFBOtoScreen(fbo_entities.at("post_proc_rect"), frame_buffers.at("fb"), shlib, screensize, black);
}

//void RenderSystem::processEntity(SceneComponent& scene, const MVPset& MVP, Entity* entity, Program* shader)
//{
//}
