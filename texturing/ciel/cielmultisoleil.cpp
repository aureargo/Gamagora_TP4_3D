#include "cielmultisoleil.h"

Lumiere CielMultiSoleil::getLumiere(const vec3& pos, const vec3& n) const
{
    Lumiere l = Lumiere(SourceLumiere(*this),pos+n*rayon, rayon*2);
    l.puissance /= NB_RAYONS_CIEL;
    if(soleils.empty())
        return l;

    SourceLumiere sl;

    l.color = l.color*l.puissance;
    l.ia = l.ia*l.puissance;
    l.id = l.id*l.puissance;
    l.is = l.is*l.puissance;
    for(const Soleil& soleil: soleils)
    {
        if(soleil.calculLumiere(n, sl))
        {
            l.color += sl.color*sl.puissance;
            l.ia += sl.ia*sl.puissance;
            l.id += sl.id*sl.puissance;
            l.is += sl.is*sl.puissance;
            l.puissance += sl.puissance;
        }
    }
    float invPuis = 1/l.puissance;
    l.color*= invPuis;
    l.ia*= invPuis;
    l.id*= invPuis;
    l.is*= invPuis;
    return l;
}
