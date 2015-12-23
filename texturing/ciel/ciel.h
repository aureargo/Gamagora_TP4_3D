#ifndef CIEL_H
#define CIEL_H

#include "texturing/lumiere.h"
#include "texturing/material.h"
#include "lib/sphere.h"
#include "soleil.h"


/**ciel sans soleil*/
class Ciel: public SourceLumiere
{
public:
    Ciel(const SourceLumiere& l, int nbLumiereMax = 10000, float rayonProche = 0.01f);
    virtual ~Ciel() {}
    virtual vec3 phongCiel(const Material& m, const vec3& normal, const vec3& dirOeil);

    virtual float getPuissance(int i) const;

    std::vector<vec3> dirOcclusion; //direction pour le calcul d'oclution;  préféré à une liste de LumiereDir car, pour un ciel constant, on peut utiliser que la lumière générale du ciel.
};




#endif // CIEL_H
