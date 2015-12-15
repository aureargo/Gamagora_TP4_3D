#include "sphere.h"
#include <QDebug>
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

/**************************************************************************************************/

vec3 aleaSphere()
{
    float alpha = (rand()%10001)/10000.0;
    alpha *= PI;  //sur les 180° sur l'axe XZ
    float theta = (rand()%10001)/10000.0;
    theta *= 2.f*PI;  //sur les 360° sur l'axe XY

    return vec3(sinf(alpha)*cosf(theta),  sinf(alpha)*sinf(theta),  cosf(alpha));
    //return vec3(sinf(alpha)*cosf(theta),  cosf(alpha),  sinf(alpha)*sinf(theta));   //Y vers le haut
}

vec3 aleaSphere(float rayon)
{
    return aleaSphere()*rayon;
}

vec3 aleaDemiSphere()
{
    float alpha = (rand()%10001)/10000.0;
    alpha *= PI/2.f;  //sur les 90° en dessus de l'axe XY
    float theta = (rand()%10001)/10000.0;
    theta *= 2.f*PI;  //sur les 360° sur l'axe XY

    return vec3(sinf(alpha)*cosf(theta),  sinf(alpha)*sinf(theta),  cosf(alpha));
    //return vec3(sinf(alpha)*cosf(theta),  cosf(alpha),  sinf(alpha)*sinf(theta));   //Y vers le haut
}

vec3 aleaDemiSphere(float rayon)
{
    return aleaDemiSphere()*rayon;
}

vec3 aleaDemiSphere(const vec3& normal)
{
    vec3 dir(aleaSphere());
    if(glm::dot(dir, normal) < 0)   //si la direction aleatoire sur la sphere n'est pas dans la demi sphère conduit par la normale, on l'inverse;
        return -dir;
    return dir;
}

vec3 aleaDemiSphere(const vec3& normal, float rayon)
{
    return aleaDemiSphere(normal)*rayon;
}


std::vector<vec3> poissonSphere(int nbIteration, float rayonProche)
{
    std::vector<vec3> res;
    res.reserve(nbIteration);

    for(int i = 0;  i < nbIteration;  i++)    {
        vec3 p = aleaSphere();
        if(!vec3proche(p, res, rayonProche))
            res.push_back(p);
    }
    //res.shrink_to_fit();
    return res;
}

std::vector<vec3> poissonDemiSphere(int nbIteration, float rayonProche)
{
    std::vector<vec3> res;
    res.reserve(nbIteration);

    for(int i = 0;  i < nbIteration;  i++)    {
        vec3 p = aleaDemiSphere();
        //if(!vec3proche(p, res, rayonProche))
            res.push_back(p);
    }
    //res.shrink_to_fit();
    return res;
}

std::vector<vec3> poissonDemiSphere(vec3 normal, int nbIteration, float rayonProche)
{
    /*
    std::vector<vec3> res2;
    res2.reserve(nbIteration);
    float angle = 3.14f/(float)nbIteration;
    for(float i = 0; i<3.14f; i+=angle){
        vec3 p(sin(angle),0,cos(angle));
        res2.push_back(p);
    }
    return res2;
*/

    if(normal == HAUT)
        return poissonDemiSphere(nbIteration, rayonProche);
    std::vector<vec3> res;
    res.reserve(nbIteration);

    for(int i = 0;  i < nbIteration;  i++)    {
        vec3 p = aleaDemiSphere(normal);
        //if(!vec3proche(p, res, rayonProche))
            res.push_back(p);
    }
    //res.shrink_to_fit();
    return res;
}
