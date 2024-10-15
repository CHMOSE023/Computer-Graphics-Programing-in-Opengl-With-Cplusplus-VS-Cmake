#pragma once
#include <glad/glad.h>
#include "IMesh.hpp"
#include <vector>
#include <iostream>

class MeshSphere : public IMesh {
public:
    MeshSphere(std::vector<float> vertexPositions, std::vector<unsigned int> indices);
    ~MeshSphere();
    void setupMesh();
    void render() override;
private:
    unsigned int vao, vbo, ebo;
    std::vector<float> vertexPositions;
    std::vector<unsigned int> indices;
};
