/*
this class is not the same as the one in the book,
I made some corrections in the functions to calculate
the vertices and indices and i modified the variables's names
*/

#pragma once

#include <glm/glm.hpp>
#include "../private/stlHeaders.hpp"

class Sphere {
private:

    float radius;
    int sectorCount;
    int stackCount;
   
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    void buildVertices();
    void buildIndices();
    float toRadians(float degrees);

public:
    Sphere(float radius, int sectorCount, int stackCount);
    Sphere(int radius);

    int getNumVertices();
    int getNumIndices();
    
    std::vector<unsigned int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec3> getNormals();
    std::vector<glm::vec2> getTexCoords();
};
