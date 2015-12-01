#include "voxel.h"
#include <QDebug>

Voxel::Voxel():_n(0){

}

Voxel::Voxel(int n, Node *noeud):_n(n){
    v.resize(n*n*n);
    createvoxel(noeud);
}


bool Voxel::createvoxel(Node *noeud){
    float resx, resy, resz;

    Box box = noeud->getBox();
    vec3 bornemin = box.getMin();
    vec3 bornemax = box.getMax();

    resx = (bornemax.x - bornemin.x)/(_n-1);
    resy = (bornemax.y - bornemin.y)/(_n-1);
    resz = (bornemax.z - bornemin.z)/(_n-1);

    _resolution = resx;
    _resolution = (resy > _resolution) ? resy : _resolution;
    _resolution = (resz > _resolution) ? resz : _resolution;

    //qDebug()<<_resolution;
    for(float i=0; i<_n; i++){
        for(float j=0; j<_n; j++){
            for(float k=0; k<_n; k++){
                if(noeud->inOut(glm::vec3(bornemin.x+(i*_resolution),bornemin.y+(j*_resolution),bornemin.z+(k*_resolution))))
                {
                    (*this)(i,j,k)=1;
                }
                else (*this)(i,j,k)=0;
            }
        }
    }
    return true;
}
