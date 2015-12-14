#ifndef MATERIAL_H
#define MATERIAL_H

#include "lib/vector3.h"
#define HERBE vec3(0.15,1.0,0.1)
#define ROCHE vec3(0.36f,0.24f,0.07f)
#define NEIGE vec3(0.9,0.9,0.9)


class Material
{
public:
    Material():
        Material(GRIS)
    {}
    Material(const vec3& color, float intensiteA = 0.1, float intensiteD = 0.7, float intensiteS = 0.2, float brillance = 10):
            color(color), ia(intensiteA), id(intensiteD), is(intensiteS), brillance(brillance)
    {}

    vec3 getColor() const;
    vec3 getAmbiant() const;
    vec3 getDiffus() const;
    vec3 getSpeculaire() const;
    float getBrillance() const;

    void setIntensite(float ambiant = 0.2f, float diffus = 0.7f, float speculaire = 0.1f);

protected:

    vec3 color;
    float ia, id, is;   //intensité ambiante, diffuse et spéculaire
    float brillance;
};




class Roche: public Material
{
public:
    Roche():
        Material(ROCHE,0.2, 0.9, 0.2, 2)
    {}
    Roche(float niveau):
        Roche()
    {
        color*=niveau;
    }
};

class Herbe: public Material
{
public:
    Herbe():
        Material(HERBE, 0.3, 0.9, 0.6, 5)
    {}
    Herbe(float niveau):
        Herbe()
    {
        color*=niveau;
    }
};

class Neige: public Material
{
public:
    Neige():
        Material(NEIGE, 0.2, 1, 1 ,10)
    {}
    Neige(float niveau):
        Neige()
    {
        color*=niveau;
    }
};


#endif // MATERIAL_H
