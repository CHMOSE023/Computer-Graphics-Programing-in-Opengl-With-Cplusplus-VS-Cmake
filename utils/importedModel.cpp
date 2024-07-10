#include "importedModel.hpp"


ImportedModel::ImportedModel(const char* path) {
    parseObjFile(path);
}

void ImportedModel::parseObjFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        } else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "f") {
            unsigned int vertIdx[3], texIdx[3], normIdx[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                ss >> vertIdx[i] >> slash >> texIdx[i] >> slash >> normIdx[i];
                // OBJ files are 1-indexed, adjust for 0-indexing in vectors
                vertices.push_back(vertices[vertIdx[i] - 1]);
                texCoords.push_back(texCoords[texIdx[i] - 1]);
                normals.push_back(normals[normIdx[i] - 1]);
            }
        }
    }

    file.close();
}

int ImportedModel::getNumVertices() const {
    return vertices.size();
}

std::vector<glm::vec3> ImportedModel::getVertices() const {
    return vertices;
}

std::vector<glm::vec2> ImportedModel::getTexCoords() const {
    return texCoords;
}

std::vector<glm::vec3> ImportedModel::getNormals() const {
    return normals;
}