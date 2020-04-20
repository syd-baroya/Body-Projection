#pragma once



#ifndef DRAWABLE_COMPONENT_H
#define DRAWABLE_COMPONENT_H

#include <glm/glm.hpp>
#include "Component.h"


struct MVPSet {

    glm::mat4 M;

    glm::mat4 V;

    glm::mat4 P;

};



class DrawableComponent : public Component {

public:

    DrawableComponent() : Component() 
    {
        MVP = { glm::mat4(1), glm::mat4(0), glm::mat4(1) };
    }

    //    void draw(Program* program) override;

    MVPSet* getMVP() { return(&MVP); }

    void setMVP(MVPSet matrices) { MVP = matrices; }



private:

    MVPSet MVP;

};

#endif