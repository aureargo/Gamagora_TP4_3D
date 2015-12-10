#include "lumiere.h"

#include <glm/gtx/norm.hpp>
#include "lib/sphere.h"

bool lumiereProche(const std::vector<Lumiere>& lumieres, const vec3& pos, float rayonProche)
{
    for(const Lumiere& l : lumieres)
        if(glm::distance2(pos, l.pos) < rayonProche*rayonProche)
            return true;
    return false;
}

std::vector<Lumiere> poissonLumieres(const vec3& center, float rayonSphere, int nbIteration, float rayonProche)
{
    std::vector<Lumiere> lumieres;
    lumieres.reserve(nbIteration);

    for(int i = 0;  i < nbIteration;  i++)
    {
        vec3 p = aleaDemiSphere(rayonSphere) + center;
        if(!lumiereProche(lumieres, p, rayonProche))
        {
            lumieres.push_back(Lumiere(p));
        }
    }
    lumieres.shrink_to_fit();
    return lumieres;
}

