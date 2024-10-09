#include "../include/public/MeshPyramid.hpp"

MeshPyramid::MeshPyramid(std::vector<float> vertexPositions) : vertexPositions(vertexPositions){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

MeshPyramid::~MeshPyramid() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void MeshPyramid::setupMesh() {

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), &vertexPositions[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3,GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void MeshPyramid::render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size() / 6);
}

