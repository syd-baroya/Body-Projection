#pragma once
#ifndef CAMERA_ENTITY_H
#define CAMERA_ENTITY_H

#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraEntity : public Entity {
public:

	CameraEntity() : fov(45.0f), near_plane(.01f), far_plane(100.0f), position(glm::vec3(0.0f)) {}
	CameraEntity(float fov, const glm::vec3& loc) : fov(fov), near_plane(.01f), far_plane(100.0f), position(loc) {}
	CameraEntity(float fov, const glm::vec3& loc, float near_plane, float far_plane) : fov(fov), near_plane(near_plane), far_plane(far_plane), position(loc) {}

	void updatePostion(glm::vec3 pos) { position += pos;  }
	void updateRotation(glm::vec3 rot) { rotation += rot; }
	virtual glm::mat4 getPerspective(float aspect) const {
		return(glm::perspective(glm::radians(fov), aspect, near_plane, far_plane));
	}

	virtual glm::mat4 getOrthographic() const {
		return(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane));
	}

	glm::mat4 getOrthographic(float left, float right, float bottom, float top, float near_plane, float far_plane) {
		return(glm::ortho(left, right, bottom, top, near_plane, far_plane));
	}

	virtual glm::mat4 getView() const;
	virtual glm::vec3 getViewDir() const;
	virtual glm::vec3 getViewDir(const Pose& aPoseOverride) const;
	virtual glm::mat4 getView(const Pose& aPoseOverride) const;

	virtual void setNearPlane(float near_plane) { this->near_plane = near_plane; }
	virtual void setFarPlane(float far_plane) { this->far_plane = far_plane; }


private:
	glm::vec3 position;
	glm::vec3 rotation = glm::vec3(0);
	float fov;
	float near_plane;
	float far_plane;
};

#endif
