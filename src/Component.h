#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H
#include <string>
#include "Program.h"


typedef uintmax_t uuid_t;

uuid_t generate_uuid();

std::string uuid_to_string(uuid_t uuid);

class Component {

public:
	Component() : uuid(generate_uuid()) {}
	virtual void init() {}
	virtual void update(double frametime) {}
	virtual void draw(Program* prog) {}
	uuid_t getUUID() { return(uuid); }


	virtual ~Component() {}

private:
	uuid_t uuid;

};

#endif