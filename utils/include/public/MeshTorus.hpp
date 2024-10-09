#pragma once
#include <glad/glad.h>
#include "IMesh.hpp"
#include <vector>
#include <iostream>

class MeshTorus : public IMesh {
public:
    MeshTorus(std::vector<float> vertexPositions, std::vector<unsigned int> indices);
    ~MeshTorus();
    void setupMesh();
    void render() override;
private:
    unsigned int vao, vbo, ebo;
    std::vector<float> vertexPositions;
    std::vector<unsigned int> indices;
};
