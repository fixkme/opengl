#include "light.h"

Light::Light()
{
    this->position = glm::vec3(1.f, 5.f, 3.f);
    this->direction = glm::vec3(1.f, 5.f, 3.f);
    this->ambient = glm::vec3(0.5f);
    this->diffuse = glm::vec3(0.5f);
    this->specular = glm::vec3(1.f);
}
