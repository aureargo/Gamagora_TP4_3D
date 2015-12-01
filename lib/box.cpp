#include "box.h"


Box::Box()  : min(vec3(FLT_MAX,FLT_MAX,FLT_MAX)), max(vec3(FLT_MIN,FLT_MIN,FLT_MIN))
{
}

Box::Box(const vec3& _min, const vec3& _max)    :   min(_min),  max(_max)
{
}

Box::Box(const std::vector<vec3>& points)
{
    parcourtPoints(points);
}


void Box::updatePoint(const vec3& p)
{
    update(p);
}

bool Box::inOut(const vec3& p) const
{
    for(int i = 0;  i < 3;  i++)
    {
        if(p[i] < min[i])
            return false;
        if(p[i] > max[i])
            return false;
    }
    return true;
}


bool Box::intersect(const Rayon &r, float &distanceMin, float &distanceMax) const
{
    if(this->inOut(r.getOrigine()))  {
        distanceMin = 0;
        distanceMax = intersectIn(r);
        return true;
    }

    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float div;

    if(r.getDirection().x == 0)    {
        tmin = FLT_MIN;
        tmax = FLT_MAX;
    }
    else if(r.getDirection().x > 0)    {
        div = 1 / r.getDirection().x;
        tmin = (min.x - r.getOrigine().x) * div;
        tmax = (max.x - r.getOrigine().x) * div;
    }
    else    {
        div = 1 / r.getDirection().x;
        tmin = (max.x - r.getOrigine().x) * div;
        tmax = (min.x - r.getOrigine().x) * div;
    }

    if(r.getDirection().y == 0)    {
        tymin = FLT_MIN;
        tymax = FLT_MAX;
    }
    else if(r.getDirection().y >= 0)    {
        div = 1 / r.getDirection().y;
        tymin = (min.y - r.getOrigine().y) * div;
        tymax = (max.y - r.getOrigine().y) * div;
    }
    else    {
        div = 1 / r.getDirection().y;
        tymin = (max.y - r.getOrigine().y) * div;
        tymax = (min.y - r.getOrigine().y) * div;
    }

    if( (tmin > tymax) || (tymin > tmax) )
        return false;

    if(tymin > tmin)
        tmin = tymin;

    if(tymax < tmax)
        tmax = tymax;


    if(r.getDirection().z == 0)    {
        tzmin = FLT_MIN;
        tzmax = FLT_MAX;
    }
    else if(r.getDirection().z > 0)    {
        div = 1 / r.getDirection().z;
        tzmin = (min.z - r.getOrigine().z) * div;
        tzmax = (max.z - r.getOrigine().z) * div;
    }
    else    {
        div = 1 / r.getDirection().z;
        tzmin = (max.z - r.getOrigine().z) * div;
        tzmax = (min.z - r.getOrigine().z) * div;
    }

    if( (tmin > tzmax) || (tzmin > tmax) )
        return false;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    if(tmin>=0)
        distanceMin = tmin;

    if(tmax>0)
        distanceMax = tmax;

    return true;
}



inline float Box::intersectIn(const Rayon& r) const
{
    float tmax, tymax, tzmax;

    if(r.getDirection().x == 0)
        tmax = FLT_MAX;
    else if(r.getDirection().x > 0)
        tmax = (max.x - r.getOrigine().x) / r.getDirection().x;
    else
        tmax = (min.x - r.getOrigine().x) / r.getDirection().x;

    if(r.getDirection().y == 0)
        tymax = FLT_MAX;
    else if(r.getDirection().y >= 0)
        tymax = (max.y - r.getOrigine().y) / r.getDirection().y;
    else
        tymax = (min.y - r.getOrigine().y) / r.getDirection().y;

    if(tymax < tmax)
        tmax = tymax;


    if(r.getDirection().z == 0)
        return tmax;
    else if(r.getDirection().z > 0)
        tzmax = (max.z - r.getOrigine().z) / r.getDirection().z;
    else
        tzmax = (min.z - r.getOrigine().z) / r.getDirection().z;

    if(tzmax < tmax)
        return tzmax;
    return tmax;
}


/**********************inline********************/

inline void Box::setDefaultBox()
{
    min = vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    max = vec3(FLT_MIN,FLT_MIN,FLT_MIN);
}

inline void Box::update(const vec3& p)
{
    updateMin(p);
    updateMax(p);
}

inline void Box::updateMin(const vec3& p)
{
    for(int i = 0;  i < 3;  i++)
        if(p[i] < min[i])
            min[i] = p[i];

}

inline void Box::updateMax(const vec3& p)
{
    for(int i = 0;  i < 3;  i++)
        if(p[i] > max[i])
            max[i] = p[i];
}

inline void Box::parcourtPoints(const std::vector<vec3>& points)
{
    if(points.empty())
        setDefaultBox();
    else if(points.size() == 1)
    {
        min = points[0];
        max = points[0];
    }
    else
    {
        std::vector<vec3>::const_iterator it = points.begin();
        min = *it;
        max = points[points.size()-1];

        ++it;
        for(;  it != points.end()-1; ++it)
            update(*it);
    }
}

float Box::diffZ() const
{
    return max.z-min.z;
}

void Box::merge(const Box& box2)
{
    updateMin(box2.min);
    updateMax(box2.max);
}


void Box::operator+=(const vec3& t)
{
    min += t;
    max += t;
}


/**************************************************************************/

float Box::distance(const vec3 &p) const
{
    float dx = std::max(std::max(0.f, min.x - p.x), p.x - max.x); //soit en dessous de max, soit en dessus de min, soit entre les deux (dans ce cas, distance = 0)
    float dy = std::max(std::max(0.f, min.y - p.y), p.y - max.y);
    float dz = std::max(std::max(0.f, min.z - p.z), p.z - max.z);
    return sqrt(dx*dx + dy*dy + dz*dz);
}


vec3 Box::getNormal(const vec3& p) const
{
    vec3 cote = (max-min); //taille des coté
    vec3 centre(min+cote*0.5f);
    vec3 n(p-centre);
    n /= cote;  //la normal point dans la direction du cote le plus proche du point
    vec3 na = abs(n);

    int dir = 0;    //la normale est dans la direction de l'axe X
    if(na.x < na.y)
    {
        if(na.y < na.z)
            dir = 2;    //la normale est dans la direction de l'axe Z
        else
            dir = 1;    //la normale est dans la direction de l'axe Y
    }
    else if(na.x < na.z)
        dir = 2;        //la normale est dans la direction de l'axe Z

    switch(dir)
    {
        case 0: return (n.x < 0   ?   vec3(-1,0,0)    :   vec3(1,0,0));
        case 1: return (n.y < 0   ?   vec3(0,-1,0)    :   vec3(0,1,0));
        case 2: return (n.z < 0   ?   vec3(0,0,-1)    :   vec3(0,0,1));
    }
    return vec3(0,0,0);
}
