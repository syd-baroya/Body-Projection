#pragma once
#ifndef CAMERA_ENTITY_H
#define CAMERA_ENTITY_H

#include "Entity.h"


class CameraEntity : public Entity {
public:
	CameraEntity() : Entity() {}
	CameraEntity(const std::string& name) : Entity(name) {}

	void updatePostion(glm::vec3 pos) { position += pos;  }
	void updateRotation(glm::vec3 rot) { rotation += rot; }

private:
	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
};

#endif
