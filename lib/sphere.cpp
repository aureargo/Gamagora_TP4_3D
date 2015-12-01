#include "sphere.h"

Sphere::Sphere():
    Sphere(vec3(0.f,0.f,0.f), 0)
{
}

Sphere::Sphere(const vec3 &centre, float rayon):
    centre(centre), rayon(rayon)
{

}


bool Sphere::intersect(const Rayon& r, float& minDist, float& maxDist) const
{
    vec3 diff = centre-r.getOrigine();

    vec3 l = diff*r.getDirection();
    float diff2 = length2(diff);    //distance² entre le centre de la sphere et l'origine du rayon
    float h2 = diff2-length2(l);    //distance² entre le centre de la sphere et son point le plus proche de ce centre sur le rayon.
    if(h2> rayon*rayon)     //le point le plus plus proche sur le rayon n'ai pas dans la sphère
        return false;

    float a2 = rayon*rayon-h2;   //pythagore: a2+h2 = rayon²    pa = point sur la sphère,  a2 = distance² entre le point du rayon le plus proche du centre de la sphère (qui forme un angle à 90°) et un des 2 points d'intersection entre la sphère et le rayon

    float b2 = diff2 - h2;      //pythagore: h2+b2 = diff2      b2 = distance² entre le point du rayon le plus proche du centre de la sphère et le point d'origine du rayon

    minDist = sqrt(b2)-sqrt(a2);
    maxDist = sqrt(b2)+sqrt(a2);
    return true;

    /*vec3 op = centre - r.getOrigine();		// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    float t,
        b = dot(r.getDirection(),op),
        det = b * b - dot(op,op) + rayon * rayon;
    if (det < 0)
        return false;
    else
        det = std::sqrt (det);

    minDist = b - det;
    if(minDist < 0)
        minDist = b + det;
    if(minDist < 0)
        return false;
    return true;*/
    //return (t = b - det) >= 0 ? t : ((t = b + det) >= 0 ? t : noIntersect);
}

bool Sphere::inOut(const vec3& p) const
{
    return distance2(p,centre) <= rayon*rayon;
}

float Sphere::distance(const vec3& p) const
{
    float dst2 = glm::distance2(p, centre);

    if(dst2 > rayon*rayon)
        return sqrt(dst2)-rayon;
    else
        return 0;
}



float Sphere::getRayon() const
{
    return rayon;
}


vec3 Sphere::getNormal(const vec3& p) const
{
    return normalize(p-centre);
}


Box Sphere::getBox() const
{
    vec3 tmp(rayon,rayon,rayon);
    return Box(centre-tmp, centre+tmp);
}
