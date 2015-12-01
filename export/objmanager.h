#ifndef OBJMANAGER_H
#define OBJMANAGER_H


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include <glm/vec3.hpp>

#include "lib/voxel.h"


namespace ObjManager
{
    void voxelSave(const std::string name, const Voxel &vox);


    void writeToObj(const std::string name, const std::vector<glm::vec3>& vertex, const std::vector<unsigned int>& face);
}

#endif // OBJMANAGER_H
