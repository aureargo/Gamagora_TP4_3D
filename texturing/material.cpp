#include "texturing/material.h"

#include <algorithm>    // std::max

vec3 Material::getColor() const
{
    return color;
}

vec3 Material::getAmbiant() const
{
    return color*ia;
}
vec3 Material::getDiffus() const
{
    return color*id;
}
vec3 Material::getSpeculaire() const
{
    return color*is;
}

float Material::getBrillance() const
{
    return brillance;
}

void Material::setIntensite(float ambiant, float diffus, float speculaire)
{
    ia = ambiant;
    id = diffus;
    is = speculaire;
}


