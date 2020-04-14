#pragma once
#ifndef MESH_ENTITY_H
#define MESH_ENTITY_H

#include "Entity.h"
#include <vector>
#include <glm/glm.hpp>

class MeshEntity : public Entity {

public:
	MeshEntity() : Entity() {}
	MeshEntity(const std::string& name) : Entity(name) {}

private:
	std::vector<glm::vec3> vertex_coords;
	std::vector<glm::vec3> tex_coords;
	std::vector<glm::vec3> indices;

};

#endif