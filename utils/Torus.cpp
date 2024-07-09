#include "Torus.hpp"

Torus::Torus(float innerRadius, float outerRadius, int sectors, int stacks){
    this->innerRadius = innerRadius;
    this->outerRadius = outerRadius;
    this->sectors = sectors;
    this->stacks = stacks;

    buildVertices();
    buildIndices();
}

// rings = sectores
// sides = stacks o lados

void Torus::buildVertices() {
    float x, y, z;
    float nx, ny, nz;
    float s, t;

    float sectorSteps = 2 * M_PI / sectors;
    float stackSteps = 2 * M_PI / stacks;
    float stackAngle, sectorAngle;

    for (int i = 0; i <= stacks; ++i) {
        stackAngle = i * stackSteps;

        for (int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorSteps;            

            x = (outerRadius + innerRadius * cosf(sectorAngle)) * cosf(stackAngle);
            y = (outerRadius + innerRadius * cosf(sectorAngle)) * sinf(stackAngle);
            z = innerRadius * sinf(sectorAngle);

            vertices.push_back(glm::vec3(x, y, z));

            nx = cosf(stackAngle) * cosf(sectorAngle);
            ny = sinf(stackAngle) * cosf(sectorAngle);
            nz = sinf(sectorAngle);
            normals.push_back(glm::vec3(nx, ny, nz));

            s = (float)j / sectors;
            t = (float)i / stacks;
            texCoords.push_back(glm::vec2(s, t));
        }
    }
}

void Torus::buildIndices()
{
    int k1, k2;
    for (int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);
        k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }
}

std::vector<unsigned int> Torus::getIndices() {
    return indices;
}

std::vector<glm::vec3> Torus::getVertices() {
    return vertices;
}

std::vector<glm::vec3> Torus::getNormals() {
    return normals;
}

std::vector<glm::vec2> Torus::getTexCoords() {
    return texCoords;
}

int Torus::getNumIndices() {
    return indices.size();
}

int Torus::getNumVertices() {
    return vertices.size();
}