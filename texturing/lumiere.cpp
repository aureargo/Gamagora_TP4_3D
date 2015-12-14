#include "lumiere.h"

vec3 SourceLumiere::getAmbiant() const
{
    return color*ia;
}
vec3 SourceLumiere::getDiffus() const
{
    return color*id;
}
vec3 SourceLumiere::getSpeculaire() const
{
    return color*is;
}

float SourceLumiere:: getPuissance() const
{
    return puissance;
}

void SourceLumiere::setIntensite(float ambiant, float diffus, float speculaire)
{
    ia = ambiant;
    id = diffus;
    is = speculaire;
}


void SourceLumiere::operator=(const SourceLumiere& l){
    color = l.color;
    ia = l.ia;
    id = l.id;
    is = l.is;
    puissance = l.puissance;
}

void SourceLumiere::operator=(SourceLumiere&& l)
{
    color = l.color;
    ia = l.ia;
    id = l.id;
    is = l.is;
    puissance = l.puissance;
}
/*********************************************/

float Lumiere::getDistMax() const
{
    return distMax;
}

void Lumiere::operator=(const Lumiere& l){
    color = l.color;
    ia = l.ia;
    id = l.id;
    is = l.is;
    puissance = l.puissance;
    pos = l.pos;
    distMax = l.distMax;
}

void Lumiere::operator=(Lumiere&& l)
{
    color = l.color;
    ia = l.ia;
    id = l.id;
    is = l.is;
    puissance = l.puissance;
    pos = l.pos;
    distMax = l.distMax;
}

/*bool lumiereProche(const Lumiere& l, const vec3& pos, float rayonProche)
{
    return (glm::distance2(pos, l.pos) < rayonProche*rayonProche);
}

bool lumiereProche(const std::vector<Lumiere>& lumieres, const vec3& pos, float rayonProche)
{
    for(const Lumiere& l : lumieres)
        if(glm::distance2(pos, l.pos) < rayonProche*rayonProche)
            return true;
    return false;
}*/


/*std::vector<Lumiere> poissonLumieres(const vec3& center, float rayonSphere, int nbIteration, float rayonProche)
{
    std::vector<Lumiere> lumieres;
    lumieres.reserve(nbIteration);

    for(int i = 0;  i < nbIteration;  i++)
    {
        vec3 p = aleaDemiSphere(rayonSphere) + center;
        if(!lumiereProche(lumieres, p, rayonProche))
        {
            lumieres.push_back(Lumiere(p));
        }
    }
    lumieres.shrink_to_fit();
    return lumieres;
}*/
