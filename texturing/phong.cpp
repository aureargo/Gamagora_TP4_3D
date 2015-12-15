#include "phong.h"


/***************************************/

//(1+dot(-dRay, n))*0.5;    //version Galin

/*vec3 phong(const Lumiere& l, const Material& t, const vec3& posT, const vec3& n, const vec3& oeil)
{
    vec3 lt = l.pos-posT;
    float dist2 = glm::length2(lt);
    if(dist2 >= l.distMax*l.distMax)    //si la distance entre la lumière et le point est superieur au rayon maximal d'éclairement de la lumière, on retourne une couleur noire
        return vec3(0,0,0);

    vec3 dirL = glm::normalize(lt);
    vec3 dirOeil = glm::normalize(oeil-posT);
    vec3 R = 2.f * glm::dot(n,dirL) * n - dirL;

    float puissance = 1.f - dist2/(l.distMax*l.distMax);    //plus l'objet est loin de la lumière, plus la puissance d'éclairement est faible [0;1]

    return  (
                l.colorA * t.colorA +
                l.colorD * t.colorD * std::max(0.f,glm::dot(dirL,n)) +
                l.colorS * t.colorS * powf(std::max(0.f,glm::dot(R,dirOeil)),t.brillance)
            )
            * l.puissance
            * puissance;
}*/

vec3 phong(const Material& t, const vec3& posT, const std::vector<Lumiere>& lumieres, const vec3& n, const vec3& oeil)
{
    if(lumieres.empty())
        return t.getAmbiant();

    vec3 dirOeil = glm::normalize(oeil-posT);

    vec3 color(0,0,0);  //l'ambiante du matériaux ne doit pas être divisé par la puissance de la lumière, on l'ajoute donc à la fin.
    float puissance = 0;
    for(const Lumiere& l: lumieres)
    {
        vec3 v = l.pos-posT;
        float dist2 = glm::length2(v);
        if(dist2 < l.getDistMax()*l.getDistMax())
        {
            vec3 dirL = glm::normalize(v);
            vec3 R = 2.f * glm::dot(n,dirL) * n - dirL;

            vec3 color2 = l.getAmbiant() +
                        l.getDiffus() * t.getDiffus() * std::max(0.f,glm::dot(dirL,n)) +
                        l.getSpeculaire() * t.getSpeculaire() * powf(std::max(0.f,glm::dot(R,dirOeil)),t.getBrillance());   //calcul de phong

            float puis = l.getPuissance();// * (1.f - dist2/(l.getDistMax()*l.getDistMax()));    //plus l'objet est loin de la lumière, plus la puissance d'éclairement est faible [0;1]
            color2 *= puis;
            color += color2;
            puissance += puis;
        }
    }
    if(puissance > 0)
    {
        color /= puissance;
        color += t.getAmbiant();
    }
    else
        color = t.getAmbiant();

    return color;
}
