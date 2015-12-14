#include "vector3.h"

bool vec3proche(const vec3& pos, const vec3& pos2, float rayonProche)
{
    return (glm::distance2(pos, pos2) < rayonProche*rayonProche);
}

bool vec3proche(const vec3& pos, const std::vector<vec3>& liste, float rayonProche)
{
    for(const vec3& p2 : liste)
        if(vec3proche(pos, p2, rayonProche))
            return true;
    return false;
}
