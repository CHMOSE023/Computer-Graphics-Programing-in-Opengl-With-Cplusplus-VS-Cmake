#include <Sphere.hpp>

Sphere::Sphere(float radius, int sectorCount, int stackCount) {

    this->radius = radius;
    this->sectorCount = sectorCount;
    this->stackCount = stackCount;

    buildVertices();
    buildIndices();
}

Sphere::Sphere(int radius) {
    this->radius = radius;
    this->sectorCount = 36;
    this->stackCount = 18;

    buildVertices();
    buildIndices();
}

void Sphere::buildVertices(){
    
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(glm::vec3(x,y,z));
            
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(glm::vec3(nx,ny,nz));

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.push_back(glm::vec2(s,t));

        }
    }
}

void Sphere::buildIndices() {
    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(static_cast<unsigned int>(k1));
                indices.push_back(static_cast<unsigned int>(k2));
                indices.push_back(static_cast<unsigned int>(k1 + 1));
            }

            if (i != (stackCount - 1)) {
                indices.push_back(static_cast<unsigned int>(k1 + 1));
                indices.push_back(static_cast<unsigned int>(k2));
                indices.push_back(static_cast<unsigned int>(k2 + 1));
            }
        }
    }
}


float Sphere::toRadians(float degrees) {
    return (degrees * 2.0f * M_PI)/360.0f;
}

int Sphere::getNumVertices()
{
    return vertices.size();
}

int Sphere::getNumIndices()
{
    return indices.size();
}

std::vector<unsigned int> Sphere::getIndices()
{
    return indices;
}

std::vector<glm::vec3> Sphere::getVertices()
{
    return vertices;
}

std::vector<glm::vec2> Sphere::getTexCoords()
{
    return texCoords;
}

std::vector<glm::vec3> Sphere::getNormals()
{
    return normals;
}

