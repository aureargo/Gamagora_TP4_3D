#ifndef VOXEL_H
#define VOXEL_H
#include "vector"
#include "node.h"
class Voxel
{
public:
    Voxel();
    Voxel(int n, Node *noeud);

    int operator()(int i, int j, int k)const{return v.at(i*_n*_n+j*_n+k);}
    int& operator()(int i, int j, int k){return v[i*_n*_n+j*_n+k];}
    inline int get_n()const{return _n;}
    inline float get_resolution()const{return _resolution;}


private:
    std::vector<int>v;
    int _n;
    float _resolution;
    bool createvoxel(Node *noeud);
};

#endif // VOXEL_H
