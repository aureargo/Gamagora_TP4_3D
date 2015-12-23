#include "cielsoleil.h"

CielSoleil::CielSoleil(const SourceLumiere& sl, const Soleil& soleil, int nbLumiereMax, float rayonProche):
    Ciel(sl, nbLumiereMax, rayonProche)
{
    lumieres.reserve(dirOcclusion.size());
    for(const vec3& dir: dirOcclusion)
    {
        SourceLumiere lum2; //lumière produite par le soleil dans cette direction
        if(soleil.calculLumiere(dir,lum2))
        {
            lum2.color = lum2.color*lum2.puissance + sl.color*sl.puissance;
            lum2.ia = lum2.ia*lum2.puissance + sl.ia*sl.puissance;
            lum2.id = lum2.id*lum2.puissance + sl.id*sl.puissance;
            lum2.is = lum2.is*lum2.puissance + sl.is*sl.puissance;
            lum2.puissance += sl.puissance;

            float invPuis = 1.f/lum2.puissance;
            lum2.color*= invPuis;
            lum2.ia*= invPuis;
            lum2.id*= invPuis;
            lum2.is*= invPuis;
        }
        else
            lumieres.push_back(LumiereDir(sl, dir));
    }
}

CielSoleil::CielSoleil(const SourceLumiere& sl, const std::vector<Soleil>& soleils, int nbLumiereMax, float rayonProche):
    Ciel(sl, nbLumiereMax, rayonProche)
{
    lumieres.reserve(dirOcclusion.size());
    for(const vec3& dir: dirOcclusion)
    {

        if(soleils.empty())
            lumieres.push_back(LumiereDir(sl,dir));
        else
        {
            SourceLumiere l = sl; //lumière produite par le soleil dans cette direction


            l.color = l.color*l.puissance;
            l.ia = l.ia*l.puissance;
            l.id = l.id*l.puissance;
            l.is = l.is*l.puissance;
            for(const Soleil& soleil: soleils)
            {
                SourceLumiere l2;
                if(soleil.calculLumiere(dir, l2))
                {
                    l.color += l2.color*l2.puissance;
                    l.ia += l2.ia*l2.puissance;
                    l.id += l2.id*l2.puissance;
                    l.is += l2.is*l2.puissance;
                    l.puissance += l2.puissance;
                }
            }
            float invPuis = 1.f/l.puissance;
            l.color*= invPuis;
            l.ia*= invPuis;
            l.id*= invPuis;
            l.is*= invPuis;
            lumieres.push_back(LumiereDir(l,dir));
        }

    }
}

float CielSoleil::getPuissance(int i) const
{
    return lumieres[i].getPuissance();
}

vec3 CielSoleil::phongCiel(const Material& m, const vec3& normal, const vec3& dirOeil)
{
    if(lumieres.empty())
        return NOIR;    //m.getAmbiant();

    //vec3 dirOeil = glm::normalize(oeil-posT);

    vec3 color(0,0,0);  //l'ambiante du matériaux ne doit pas être divisé par la puissance de la lumière, on l'ajoute donc à la fin.
    float puissance = 0;
    for(unsigned int i = 0;  i < dirOcclusion.size();   i++)
    {
        LumiereDir l = lumieres[i];
        vec3 dirL = dirOcclusion[i];

        vec3 R = 2.f * glm::dot(normal,dirL) * normal - dirL;

        vec3 color2 = l.getAmbiant() +
                    l.getDiffus() * m.getDiffus() * std::max(0.f,glm::dot(dirL,normal)) +
                    l.getSpeculaire() * m.getSpeculaire() * powf(std::max(0.f,glm::dot(R,dirOeil)),m.getBrillance());   //calcul de phong

        float puis = l.getPuissance();// * (1.f - dist2/(l.getDistMax()*l.getDistMax()));    //plus l'objet est loin de la lumière, plus la puissance d'éclairement est faible [0;1]
        color2 *= puis;
        color += color2;
        puissance += puis;
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
