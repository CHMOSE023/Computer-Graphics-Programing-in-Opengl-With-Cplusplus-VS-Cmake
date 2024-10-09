#include "../include/public/MeshFactory.hpp"

void MeshFactory::registerMeshType(const std::string& typeName, std::function<std::unique_ptr<IMesh>()> creator) {
    meshFactoryMap[typeName] = creator;
}

std::unique_ptr<IMesh> MeshFactory::createMesh(const std::string& typeName) {
    auto it = meshFactoryMap.find(typeName);
    if (it != meshFactoryMap.end()) {
        return it->second();
    } else {
        std::cerr << "Error: Mesh type " << typeName << " not registered" << std::endl;
        return nullptr;
    }
}