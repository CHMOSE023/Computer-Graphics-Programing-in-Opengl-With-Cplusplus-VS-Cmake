#pragma once
#include "IMesh.hpp"
#include <memory>
#include <map>
#include <functional>
#include <iostream>

class MeshFactory {
public:
    void registerMeshType(const std::string& typeName, std::function<std::unique_ptr<IMesh>()> creator);
    
    std::unique_ptr<IMesh> createMesh(const std::string& typeName);

private:
    std::map<std::string, std::function<std::unique_ptr<IMesh>()>> meshFactoryMap;
};
