#pragma once
#include "stlHeaders.hpp"
#include <glm/glm.hpp>

class ImportedModel {
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

public:
    ImportedModel(const char* path);
    void parseObjFile(const char* path);
    int getNumVertices() const;
    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec2> getTexCoords() const;
    std::vector<glm::vec3> getNormals() const;
};

