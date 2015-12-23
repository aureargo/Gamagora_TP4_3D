#ifndef CIELSOLEIL_H
#define CIELSOLEIL_H

#include "ciel.h"
#include "soleil.h"

/**ciel avec un soleil*/
class CielSoleil:   public Ciel
{
public:
    CielSoleil(const SourceLumiere& l, const Soleil& s, int nbLumiereMax = 10000, float rayonProche = 0.01f);
    CielSoleil(const SourceLumiere& sl, const std::vector<Soleil>& soleils, int nbLumiereMax = 10000, float rayonProche = 0.01f);
    ~CielSoleil() {}

    float getPuissance(int i) const;
    vec3 phongCiel(const Material& m, const vec3& normal, const vec3& dirOeil);

    std::vector<LumiereDir> lumieres;
};

#endif // CIELSOLEIL_H
