#ifndef TEXTURE_H
#define TEXTURE_H

#include <glm/vec3.hpp>
#include "glm/gtx/norm.hpp"

typedef glm::vec3 vec3;
#define GRIS vec3(0.5f,0.5f,0.5f)
#define HERBE vec3(0.1,0.75,0.1)
#define ROCHE vec3(0.36f,0.24f,0.07f)
#define NEIGE vec3(1,1,1)


class Texture
{
public:
    Texture():
        Texture(GRIS)
    {}
    Texture(const vec3& color, float brillance = 10):
        Texture(color,color,color,brillance)
    {}
    Texture(const vec3& colorA, const vec3& colorD, const vec3& colorS, float brillance = 10):
            colorA(colorA), colorD(colorD), colorS(colorS), brillance(brillance)
    {}

    vec3 colorA, colorD, colorS;
    float brillance;
};




class Roche: public Texture
{
public:
    Roche():
        Texture(ROCHE, ROCHE, vec3(0.75,0.75,0.75), 1)
    {}
    Roche(float niveau):
        Roche()
    {
        colorA*=niveau;
        colorD*=niveau;
        colorS*=niveau;
    }
};

class Herbe: public Texture
{
public:
    Herbe():
        Texture(HERBE, HERBE, vec3(0.75,0.75,0.75), 5)
    {}
    Herbe(float niveau):
        Herbe()
    {
        colorA*=niveau;
        colorD*=niveau;
        colorS*=niveau;
    }
};

class Neige: public Texture
{
public:
    Neige():
        Texture(NEIGE,NEIGE, vec3(0.75,0.75,0.75),20)
    {}
    Neige(float niveau):
        Neige()
    {
        colorA*=niveau;
        colorD*=niveau;
        colorS*=niveau;
    }
};


#include "lumiere.h"

vec3 phong(const Lumiere& l, const Texture& t, const vec3& posT, const vec3& n, const vec3& oeil);


#endif // TEXTURE_H
