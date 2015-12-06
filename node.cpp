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

Texture Node::getTexture(const vec3& p) const
{
    return Texture();
    (void) p;
}
