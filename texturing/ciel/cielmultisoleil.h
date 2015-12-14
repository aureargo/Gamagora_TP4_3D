#ifndef CIELMULTISOLEIL_H
#define CIELMULTISOLEIL_H

#include "ciel.h"
#include "soleil.h"

class CielMultiSoleil: public Ciel
{
public:
    CielMultiSoleil(const SourceLumiere& l, const std::vector<Soleil>& soleils, float rayon = DISTANCE_MAX_LUMIERE/2.f):
        Ciel(l,rayon),   soleils(soleils)
    {}


    Lumiere getLumiere(const vec3& pos, const vec3& n) const;

    std::vector<Soleil> soleils;
};

#endif // CIELMULTISOLEIL_H
