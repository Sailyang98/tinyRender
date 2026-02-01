#pragma once
#include <vector>
#include <string>

struct Vec3 {
    float x;
    float y;
    float z;
};

struct IVec3 {
    int x;
    int y;
    int z;
};

struct Face {
    std::vector<IVec3> vertices;
};

class Model {
public:
    bool LoadModel(const std::string& filename);
    std::vector<Vec3> vertices; // {x, y, z}
    std::vector<Vec3> normals; // {x, y, z}
    std::vector<Vec3> texCoords; // {x, y, z}
    std::vector<Face> faces; // { { v1Idx, vt1Idx, vn1Idx}, { v2Idx, vt2Idx, vn2Idx}, { v3Idx, vt3Idx, vn3Idx} }
private:
};
