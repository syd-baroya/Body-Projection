#pragma once



#ifndef DRAWABLE_COMPONENT_H
#define DRAWABLE_COMPONENT_H

#include <glm/glm.hpp>
#include "Component.h"
#include <glm\ext\matrix_clip_space.hpp>


struct MVPSet {

    glm::mat4 M;

    glm::mat4 V;

    glm::mat4 P;

};



class DrawableComponent : public Component {

public:

    DrawableComponent(int width, int height) : Component() 
    {
        glm::mat4 R = glm::rotate(glm::mat4(1), 0.f, glm::vec3(0, 1, 0));
        glm::vec4 dir = glm::vec4(0, 0, 0, 1);

        glm::mat4 T = glm::translate(glm::mat4(1), vec3(0));
        MVP = 
        {
            glm::mat4(1),
            R* T,
            glm::perspective(camfov, (float)((float)width / (float)height), 0.1f, 1000.0f)
        };
    }

    //    void draw(Program* program) override;

    MVPSet* getMVP() { return(&MVP); }

    void setMVP(MVPSet matrices) { MVP = matrices; }

    float camfov = 3.1415926 / 4.;

private:

    MVPSet MVP;

};

#endif