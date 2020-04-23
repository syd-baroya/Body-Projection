#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H
#include <string>
#include "Program.h"

class Component {

public:
	Component() {}
	virtual void init() {}
	virtual void update(double frametime) {}
	virtual void draw(Program* prog) {}


	virtual ~Component() {}

};

#endif