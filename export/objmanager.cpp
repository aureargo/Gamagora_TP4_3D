#include "objmanager.h"

namespace ObjManager {

void voxelSave(const std::string name, const Voxel &vox){

    std::vector<glm::vec3> vertex;
    std::vector<unsigned int> face;

    const float scale = 1.0/2.0;

    const unsigned int nb = vox.get_n();
    for(unsigned int i=0; i<nb; i++){
        for(unsigned int j=0; j<nb; j++){
            for(unsigned int k=0; k<nb; k++){

                if(vox(i,j,k) != 0){

                    /*
                          (s7)       (s6)
                           x----------x
                          /          /|
                     (s3)/      (s2)/ |
                        x----------x  |
                        |      X   |  |
                        |  x(i,j,k)|  x (s5)
                        | (s4)     | /
                        |          |/
                        x----------x
                      (s0)        (s1)

                    */

                    // ajout faces
                    unsigned int s0 = vertex.size(), s1 = vertex.size()+1, s2 = vertex.size()+2, s3 = vertex.size()+3,
                            s4 = vertex.size()+4, s5 = vertex.size()+5, s6 = vertex.size()+6, s7 = vertex.size()+7;

                    vertex.push_back(glm::vec3(i-scale,j-scale,k-scale));
                    vertex.push_back(glm::vec3(i-scale,j-scale,k+scale));
                    vertex.push_back(glm::vec3(i-scale,j+scale,k+scale));
                    vertex.push_back(glm::vec3(i-scale,j+scale,k-scale));

                    vertex.push_back(glm::vec3(i+scale,j-scale,k-scale));
                    vertex.push_back(glm::vec3(i+scale,j-scale,k+scale));
                    vertex.push_back(glm::vec3(i+scale,j+scale,k+scale));
                    vertex.push_back(glm::vec3(i+scale,j+scale,k-scale));


                    if(i == 0 || vox(i-1,j,k) == 0){

                        //** Face s0,s1,s2,s3 **//
                        face.push_back(s0);
                        face.push_back(s1);
                        face.push_back(s3);

                        face.push_back(s3);
                        face.push_back(s1);
                        face.push_back(s2);
                    }

                    if(k == nb-1 || vox(i,j,k+1) == 0){
                        //** Face s1,s5,s6,s2 **//
                        face.push_back(s1);
                        face.push_back(s5);
                        face.push_back(s2);

                        face.push_back(s2);
                        face.push_back(s5);
                        face.push_back(s6);
                    }

                    if(i == nb-1 || vox(i+1,j,k) == 0){
                        //** Face s5,s4,s7,s6 **//
                        face.push_back(s5);
                        face.push_back(s4);
                        face.push_back(s6);

                        face.push_back(s6);
                        face.push_back(s4);
                        face.push_back(s7);

                    }

                    if(k == 0 || vox(i,j,k-1) == 0){
                        //** Face s4,s0,s3,s7 **//
                        face.push_back(s4);
                        face.push_back(s0);
                        face.push_back(s7);

                        face.push_back(s7);
                        face.push_back(s0);
                        face.push_back(s3);
                    }

                    if(j == 0 || vox(i,j-1,k) == 0){

                        //** Face s5,s4,s0,s1 **//
                        face.push_back(s5);
                        face.push_back(s4);
                        face.push_back(s1);

                        face.push_back(s1);
                        face.push_back(s4);
                        face.push_back(s0);
                    }

                    if(j == nb-1 || vox(i,j+1,k) == 0){

                        //** Face s3,s2,s6,s7 **//
                        face.push_back(s3);
                        face.push_back(s2);
                        face.push_back(s7);

                        face.push_back(s7);
                        face.push_back(s2);
                        face.push_back(s6);
                    }
                }
            }
        }
    }
    writeToObj(name, vertex, face);
}


void writeToObj(const std::string name, const std::vector<glm::vec3> &vertex, const std::vector<unsigned int> &face)
{
    std::ofstream obj;
    obj.open(name.c_str(), std::ios::out);

    obj << "#vertices:\n";

    for(unsigned int i = 0 ; i < vertex.size(); i++) {
        obj << "v " << std::setprecision(4) << vertex[i].x << " " << vertex[i].y << " " << vertex[i].z << "\n";
    }


    obj << "#faces:\n";

    for(unsigned int i = 0 ; i < face.size(); i += 3){
        obj << "f " << face[i] + 1 << " " << face[i+1] + 1 << " "<< face[i+2] + 1 << "\n";

    }

    obj << "\n";

    obj.close();

    std::cout << "wrote " << name << std::endl;
}

}   //end namespace ObjManager
