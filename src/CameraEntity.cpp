#include "CameraEntity.h"

glm::mat4 CameraEntity::getView() const
{
    return glm::mat4(1);
}

glm::vec3 CameraEntity::getViewDir() const
{
    return glm::vec3(0, 0, 0);
}


