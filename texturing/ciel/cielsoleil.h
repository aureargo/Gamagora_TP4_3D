#ifndef CIELSOLEIL_H
#define CIELSOLEIL_H

#include "ciel.h"
#include "soleil.h"

/**ciel avec un soleil*/
class CielSoleil:   public Ciel
{
public:
    CielSoleil(const SourceLumiere& l, const Soleil& s, float rayon = DISTANCE_MAX_LUMIERE/2.f):
        Ciel(l,rayon),   soleil(s)
    {}


    Lumiere getLumiere(const vec3& pos, const vec3& n) const;

    Soleil soleil;
};

#endif // CIELSOLEIL_H
