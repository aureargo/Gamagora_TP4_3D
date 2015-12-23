#include "phong.h"


/***************************************/

//(1+dot(-dRay, n))*0.5;    //version Galin


vec3 phong(const Material& m, const vec3& posM, const std::vector<Lumiere>& lumieres, const vec3& n, const vec3& oeil)
{
    if(lumieres.empty())
        return NOIR;    //return m.getAmbiant();

    vec3 dirOeil = glm::normalize(oeil-posM);

    vec3 color(0,0,0);  //l'ambiante du matériaux ne doit pas être divisé par la puissance de la lumière, on l'ajoute donc à la fin.
    float puissance = 0;
    for(const Lumiere& l: lumieres)
    {
        vec3 v = l.pos-posM;
        float dist2 = glm::length2(v);
        if(dist2 < l.getDistMax()*l.getDistMax())
        {
            vec3 dirL = glm::normalize(v);
            vec3 R = 2.f * glm::dot(n,dirL) * n - dirL;

            vec3 color2 = l.getAmbiant() +
                        l.getDiffus() * m.getDiffus() * std::max(0.f,glm::dot(dirL,n)) +
                        l.getSpeculaire() * m.getSpeculaire() * powf(std::max(0.f,glm::dot(R,dirOeil)),m.getBrillance());   //calcul de phong

            float puis = l.getPuissance();// * (1.f - dist2/(l.getDistMax()*l.getDistMax()));    //plus l'objet est loin de la lumière, plus la puissance d'éclairement est faible [0;1]
            color2 *= puis;
            color += color2;
            puissance += puis;
        }
    }
    if(puissance != 0)
    {
        color /= puissance;
        //color += m.getAmbiant();
        return color;
    }
    else
        return NOIR;//color = m.getAmbiant();
}
