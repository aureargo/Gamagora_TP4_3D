#ifndef LUMIERE_H
#define LUMIERE_H

#include <glm/vec3.hpp>
#include <vector>

typedef glm::vec3 vec3;
#define GRIS vec3(0.5f,0.5f,0.5f)
#define BLANC vec3(1,1,1)

#define DEFAULT_DISTANCEMAXLUMIERE 1000000

class Lumiere
{
public:

    Lumiere(const vec3& pos):
        Lumiere(pos,vec3(0.1,0.1,0.1),BLANC,GRIS)
    {}
    Lumiere(const vec3& pos, const vec3& colorA, const vec3& colorD, const vec3 colorS, float puissance = 1, float distanceMax = DEFAULT_DISTANCEMAXLUMIERE):
        pos(pos),   colorA(colorA), colorD(colorD), colorS(colorS), puissance(puissance), distMax(distanceMax)
    {}

    vec3 pos;
    vec3 colorA, colorD, colorS;
    float puissance;
    float distMax;

};

std::vector<Lumiere> poissonLumieres(const vec3& center, float rayonSphere = DEFAULT_DISTANCEMAXLUMIERE/2.f, int nbIteration = 10000, float rayonProche=10000);

#endif // LUMIERE_H
