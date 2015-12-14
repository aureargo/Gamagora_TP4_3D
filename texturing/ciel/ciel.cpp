#include "ciel.h"

Lumiere Ciel::getLumiere(const vec3& pos, const vec3& n) const
{
    Lumiere l = Lumiere(SourceLumiere(*this),pos+n*rayon, rayon*2);
    l.puissance /= NB_RAYONS_CIEL;
    return l;
}
