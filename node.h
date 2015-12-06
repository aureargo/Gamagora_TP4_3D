#ifndef NODE_H
#define NODE_H
#include "rendu/rayon.h"
#include <QRgb>
#include "lib/box.h"
#include "texturing/lumiere.h"

#include "texturing/texture.h"


/** @brief délimite la distance des hauteurs à calculer autour d'un point pour déterminer sa normale*/
#define RAYON_NORMAL 0.1f
#define DEFAULT_COLOR vec3(1,1,1)
#define DEFAULT_T 0.5f      //changement d'état entre IN et OUT dans les surfaces implicites

using namespace glm;

///
/// \author Rudi Welter
/// \brief The Node class
///
class Node
{
public:
    Node();
    virtual ~Node();
    virtual bool inOut(const glm::vec3& p) const = 0;
    virtual float potentiel(const glm::vec3& p) const = 0;
    virtual float distance(const glm::vec3& p) const =0;

    //virtual bool intersect(const Rayon& r, float &distance) const =0;

    virtual glm::vec3 getNormal(const vec3& p, float eps = RAYON_NORMAL) const = 0;

    virtual void setColor(const vec3& color) = 0;
    virtual vec3 getColor(const vec3& p) const;
    virtual Texture getTexture(const vec3& p) const;

    virtual Box getBox() const = 0;

};

#endif // NODE_H
