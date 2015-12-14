#ifndef CIEL_H
#define CIEL_H

#include "texturing/lumiere.h"
#include "lib/sphere.h"
#include "soleil.h"


/**ciel sans soleil*/
class Ciel: public SourceLumiere
{
public:
    Ciel(const SourceLumiere& l, float rayon = DISTANCE_MAX_LUMIERE/2.f):
        SourceLumiere(l),   rayon(rayon)
    {}

    virtual ~Ciel()
    {}

    virtual Lumiere getLumiere(const vec3& pos, const vec3& n) const;


    float rayon;    //distance à laquelle on positionnera les soleils/lumiere afin de faire les calculs de phong
};




#endif // CIEL_H
