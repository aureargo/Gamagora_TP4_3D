#ifndef LUMIERE_H
#define LUMIERE_H

#include <vector>
#include "lib/vector3.h"

#define DISTANCE_MAX_LUMIERE 1000000
#define NB_RAYONS_CIEL 200

class SourceLumiere
{
public:
    SourceLumiere(const vec3& color = BLANC, float intensiteA = 0.2, float intensiteD = 1.0, float intensiteS = 1.0, float puissance = 1):
        color(color), ia(intensiteA), id(intensiteD), is(intensiteS), puissance(puissance)
    {}

    SourceLumiere(const SourceLumiere& copy):
        SourceLumiere(copy.color, copy.ia, copy.id, copy.is, copy.puissance)
    {}
    SourceLumiere(SourceLumiere&& copy):
        SourceLumiere(copy.color, copy.ia, copy.id, copy.is, copy.puissance)
    {}

    void operator=(const SourceLumiere& l);
    void operator=(SourceLumiere&& l);

    vec3 getAmbiant() const;
    vec3 getDiffus() const;
    vec3 getSpeculaire() const;
    float getPuissance() const;

    void setIntensite(float ambiant = 0.2f, float diffus = 1.0f, float speculaire = 1.0f);

//à mettre dans protected après
    vec3 color;
    float ia, id, is;
    float puissance;
protected:
};

/*********************************************************************************************/

class Lumiere : public SourceLumiere
{
public:

    Lumiere(const vec3& pos, const vec3& color = BLANC, float intensiteA = 0.2, float intensiteD = 1.0, float intensiteS = 1.0, float puissance = 1, float distanceMax = DISTANCE_MAX_LUMIERE):
        SourceLumiere(color, intensiteA, intensiteD, intensiteS, puissance), pos(pos), distMax(distanceMax)
    {}

    Lumiere(const Lumiere& copy):
        SourceLumiere(copy), pos(copy.pos), distMax(copy.distMax)
    {}
    Lumiere(Lumiere&& copy):
        SourceLumiere(copy), pos(copy.pos), distMax(copy.distMax)
    {}

    Lumiere(const SourceLumiere& sl, const vec3& pos, float distanceMax = DISTANCE_MAX_LUMIERE):
        SourceLumiere(sl),  pos(pos), distMax(distanceMax)
    {}


    void operator=(const Lumiere& l);
    void operator=(Lumiere&& l);

    float getDistMax() const;

    vec3 pos;

protected:
    float distMax;
};

/*********************************************************************************************/

std::vector<Lumiere> poissonLumieres(const vec3& center, float rayonSphere = DISTANCE_MAX_LUMIERE/2.f, int nbIteration = 10000, float rayonProche=10000);


bool lumiereProche(const Lumiere& l, const vec3& pos, float rayonProche);
bool lumiereProche(const std::vector<Lumiere>& l, const vec3& pos, float rayonProche);

#endif // LUMIERE_H
