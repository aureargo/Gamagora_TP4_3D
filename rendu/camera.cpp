#include "camera.h"

Camera::Camera()
{

}

/**
@pOr origine de la caméra
@pAt vecteur direction de la caméra (comprenant distance)
*/

Camera::Camera(const vec3 &pOr, const vec3 &pAt, const float &distance, int l, int h) :
    _origine(pOr), _lu(l), _lv(h), _lw(distance)
{
    glm::vec3 tmp(pAt - pOr);
    _w = glm::normalize(tmp);

    if(_w == vec3(0.f,0.f,1.f) || _w == vec3(0.f,0.f,-1.f))
    {
        _u = vec3(_w.z,0.f,0.f);
        _v = vec3(0.f,_w.z,0.f);
    }
    else
    {
        _u = - ( glm::cross(_w,vec3(0.f,0.f,1.f)) );
        _u = glm::normalize(_u);

        _v = cross(_w,_u);
        _v = glm::normalize(_v);
    }
}

vec3 Camera::vecScreen(int i, int j) const
{
    if(i >= _lu || j >= _lv){
        std::cerr << "i or j is incorrect" << std::endl;
        exit(-1);
    }

    float ti = ((float)i) / (_lu - 1);
    float tj = ((float)j) / (_lv - 1);


    return _w*_lw + ( (1.0f - ti)*(-_lu/2)+(ti*_lu/2) )*_u + ( (1.0f-tj)*(_lv/2)+(tj*(-_lv/2)) )*_v;
}

vec3 Camera::pointScreen(int i, int j) const
{
    return _origine + vecScreen(i, j);
}






