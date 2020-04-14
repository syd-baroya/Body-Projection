#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <glm/glm.hpp>
#include "Component.h"
#include <unordered_map>

class Entity {
public:
	Entity() : uuid(generate_uuid()) {}
	Entity(const std::string& name) : uuid(generate_uuid()), _name(name) {}

	std::string getName() { return(_name); }
	uuid_t getUUID() { return(uuid); }

	virtual void update() {}
	virtual void draw() {}
	bool isActive() { return active; }
	void setActive(bool a) { active = a; }
	void addComponent(Component c) { components.emplace(c.getUUID(), c); }

protected:
	std::string _name;

private:
	uuid_t uuid;
	bool active = true;
	std::unordered_map<uuid_t, Component> components;
	bool active = true;
};

#endif