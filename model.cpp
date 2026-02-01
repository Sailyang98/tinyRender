#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool Model::LoadModel(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    std::string line;
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") {
            Vec3 v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (prefix == "vt") {
            Vec3 v;
            ss >> v.x >> v.y >> v.z;
            texCoords.push_back(v);
        } else if (prefix == "vn") {
            Vec3 v;
            ss >> v.x >> v.y >> v.z;
            normals.push_back(v);
        } else if (prefix == "f") {
            Face f;
            std::string vertexStr;
            while(ss >> vertexStr) {
                IVec3 vertex;
                if (sscanf(vertexStr.c_str(), "%d/%d/%d", &vertex.x, &vertex.y, &vertex.z) == 3) {
                    f.vertices.push_back(vertex);
                }
            }
            faces.push_back(f);
        }
    }
    return true;
}
