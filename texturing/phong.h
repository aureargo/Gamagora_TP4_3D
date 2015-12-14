#ifndef PHONG_H
#define PHONG_H

#include "lumiere.h"
#include "material.h"

//vec3 phong(const Lumiere& l, const Material& t, const vec3& posT, const vec3& n, const vec3& oeil);
vec3 phong(const Material& t, const vec3& posT, const std::vector<Lumiere>& lumieres, const vec3& n, const vec3& oeil);

#endif // PHONG_H
