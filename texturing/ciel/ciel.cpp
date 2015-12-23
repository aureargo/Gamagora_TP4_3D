#include "ciel.h"

Ciel::Ciel(const SourceLumiere& l, int nbLumiereMax, float rayonProche):
    SourceLumiere(l)
{
    dirOcclusion = poissonDemiSphere(nbLumiereMax, rayonProche);
}

float Ciel::getPuissance(int i) const
{
    (void) i;
    return SourceLumiere::getPuissance();
}

vec3 Ciel::phongCiel(const Material& m, const vec3& normal, const vec3& dirOeil)
{
    //vec3 R = 2.f * glm::dot(normal,dirL) * normal - dirL;
    vec3 R = normal;
    //return m.getAmbiant() +
    return  (
                this->getAmbiant() +
                this->getDiffus()*m.getDiffus() +
                this->getSpeculaire()*m.getSpeculaire() * powf(std::max(0.f,glm::dot(R,dirOeil)),m.getBrillance())
            ) *
            this->puissance;
}
