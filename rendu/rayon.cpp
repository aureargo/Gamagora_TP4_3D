#include "rayon.h"

Rayon::Rayon()
{

}

Rayon::Rayon(const glm::vec3& origine, const glm::vec3& direction):
        _origine(origine),  _direction(glm::normalize(direction))
{
}

glm::vec3 Rayon::Reflect(const glm::vec3& normal) {
    const float cosI = glm::dot(normal, _direction);
    return _direction + 2*cosI * normal;
}
