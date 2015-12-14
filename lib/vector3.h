#ifndef VECTOR3_H
#define VECTOR3_H

#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>

typedef glm::vec3 vec3;

#ifndef PI
const float PI = 3.1415927f;
#endif

const vec3 HAUT(0,0,1);
const vec3 BAS(0,0,-1);

/**************************/

const vec3 NOIR(0,0,0);
const vec3 BLANC(1,1,1);
const vec3 GRIS(0.5f,0.5f,0.5f);

/**************************/

bool vec3proche(const vec3& pos, const vec3& pos2, float rayonProche);
bool vec3proche(const vec3& pos, const std::vector<vec3>& liste, float rayonProche);

#endif // VECTOR3_H
