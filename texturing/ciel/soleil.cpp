#include "soleil.h"



bool Soleil::calculLumiere(const vec3& n, SourceLumiere& l) const
{
    float d1 = glm::dot(dir,n);
    if(d1 <= 0)
        return false;

    float d2 = 1-d1*d1;
    if(d2 < angleMax*angleMax)    //l'angle entre n et la direction du soleil est faible
    {
        l = SourceLumiere(*this);
        l.puissance = interp::interp_cos1D(d2,this->puissance,0,0,angleMax); //plus l'angle est faible, plus la puissance est grande.
        return true;
    }
    return false;
}
