#include "node.h"

Node::Node()
{
}

Node::~Node(){

}


vec3 Node::getColor(const vec3& p) const
{
    return DEFAULT_COLOR;
    (void) p;
}

Material Node::getMaterial(const vec3& p) const
{
    return Material();
    (void) p;
}
