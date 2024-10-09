#pragma once
#include <glad/glad.h>
#include "IMesh.hpp"
#include <vector>

class MeshPyramid : public IMesh {
public:
    MeshPyramid(std::vector<float> vertexPositions);
    ~MeshPyramid();
    void setupMesh();
    void render() override;
private:
    unsigned int vao, vbo;
    std::vector<float> vertexPositions;
};
