#ifndef SOLEIL_H
#define SOLEIL_H

#include "lib/vector3.h"
#include "texturing/lumiere.h"
#include "lib/interpolation.h"

using namespace glm;

/**un ciel peut (pourra par la suite) avoir un ou plusieur soleil
un soleil n'est caractérisé que par sa couleur, son rayon d'influence dans le ciel et sa direction.
Sa position est calculé par intersection entre le rayon partant de l'objet ciblé avec la direction du soleil, et le ciel. Le ciel n'ayant pas non plus de position fixe*/
class Soleil: public SourceLumiere
{
public:
    Soleil(const SourceLumiere& l, const vec3& dir, float angleMax = 0.1):
        SourceLumiere(l),   dir(dir),   angleMax(angleMax)
    {}

    vec3 getDir() const  {    return dir; }
    float getAngleMax() const {   return angleMax;   }

    /**retourne si le soleil à une influence par rapport à une direction n,
     * et donne une source de lumiere en conséquence (avec plus ou moins de puissance par rapport à l'angle)*/
    bool calculLumiere(const vec3& n, SourceLumiere& l) const;

private:
    vec3 dir;   //direction du soleil dans le ciel;
    float angleMax;    //représentation de l'angle d'influence du soleil dans le ciel (entre une normal et dir)
};

#endif // SOLEIL_H
