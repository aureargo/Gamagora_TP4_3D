#ifndef SPHERE_H
#define SPHERE_H

#include "lib/vector3.h"
#include "rendu/rayon.h"
#include "box.h"

using namespace glm;

class Sphere
{
public:
    Sphere();
    Sphere(const vec3& centre, float rayon);

    bool intersect(const Rayon& r, float& minDist, float& maxDist) const;
    bool inOut(const vec3& p) const;
    float distance(const vec3 &p) const;

    vec3 getNormal(const vec3& p) const;

    float getRayon() const;

    Box getBox() const;

    protected:

    vec3 centre;
    float rayon;
};


vec3 aleaSphere();
vec3 aleaSphere(float rayon);

vec3 aleaDemiSphere();
vec3 aleaDemiSphere(float rayon);
vec3 aleaDemiSphere(const vec3& normal);
vec3 aleaDemiSphere(const vec3& normal, float rayon);

std::vector<vec3> poissonSphere(int nbIteration, float rayonProche);
std::vector<vec3> poissonDemiSphere(int nbIteration, float rayonProche);
std::vector<vec3> poissonDemiSphere(vec3 normal, int nbIteration, float rayonProche);

#endif // SPHERE_H
