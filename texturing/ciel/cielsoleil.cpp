#include "cielsoleil.h"

Lumiere CielSoleil::getLumiere(const vec3& pos, const vec3& n) const
{
    Lumiere l = Lumiere(SourceLumiere(*this),pos+n*rayon, rayon*2);
    l.puissance /= NB_RAYONS_CIEL;
    SourceLumiere sl;

    if(soleil.calculLumiere(n, sl))
    {
        l.color = l.color*l.puissance + sl.color*sl.puissance;
        l.ia = l.ia*l.puissance + sl.ia*sl.puissance;
        l.id = l.id*l.puissance + sl.id*sl.puissance;
        l.is = l.is*l.puissance + sl.is*sl.puissance;
        l.puissance += sl.puissance;

        float invPuis = 1/l.puissance;
        l.color*= invPuis;
        l.ia*= invPuis;
        l.id*= invPuis;
        l.is*= invPuis;
    }

    return l;
}
