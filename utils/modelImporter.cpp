#include"modelImporter.hpp"
ModelImporter::ModelImporter() {}

void ModelImporter::parseObjFile(const char* path) {
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
            float x, y, z;
            ss >> x >> y >> z;
            vertVals.push_back(x);
            vertVals.push_back(y);
            vertVals.push_back(z);
        } else if (prefix == "vt") {
            float u, v;
            ss >> u >> v;
            texCoordVals.push_back(u);
            texCoordVals.push_back(v);
        } else if (prefix == "vn") {
            float nx, ny, nz;
            ss >> nx >> ny >> nz;
            normVals.push_back(nx);
            normVals.push_back(ny);
            normVals.push_back(nz);
        } else if (prefix == "f") {
            unsigned int vertIdx[3], texIdx[3], normIdx[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                ss >> vertIdx[i] >> slash >> texIdx[i] >> slash >> normIdx[i];
                // OBJ files are 1-indexed, adjust for 0-indexing in vectors
                triangleVerts.push_back(vertVals[(vertIdx[i] - 1) * 3]);
                triangleVerts.push_back(vertVals[(vertIdx[i] - 1) * 3 + 1]);
                triangleVerts.push_back(vertVals[(vertIdx[i] - 1) * 3 + 2]);

                textureCoords.push_back(texCoordVals[(texIdx[i] - 1) * 2]);
                textureCoords.push_back(texCoordVals[(texIdx[i] - 1) * 2 + 1]);

                normals.push_back(normVals[(normIdx[i] - 1) * 3]);
                normals.push_back(normVals[(normIdx[i] - 1) * 3 + 1]);
                normals.push_back(normVals[(normIdx[i] - 1) * 3 + 2]);
            }
        }
    }

    file.close();
}

int ModelImporter::getNumVertices() {
    return triangleVerts.size() / 3;
}

std::vector<float> ModelImporter::getVertices() {
    return triangleVerts;
}

std::vector<float> ModelImporter::getTexCoords() {
    return textureCoords;
}

std::vector<float> ModelImporter::getNormals() {
    return normals;
}