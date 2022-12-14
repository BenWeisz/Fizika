#pragma once

#include <string>

#include <glad/glad.h>

#include "Mesh.hpp"
#include "Material.hpp"

class Model {
   public:
    Model() = delete;
    Model(const std::string& meshPath, const std::string& materialPath);
    Model(const Geometry geometry, const std::string& materialPath);
    Model(const std::string& meshPath, const Mesh::LoadOptions loadOptions);
    Model(const Geometry geometry);
    Model(const std::string& meshPath);
    ~Model();
    void Draw() const;
    Mesh* GetMesh() const;
    Material* GetMaterial() const;

   private:
    Mesh* mMesh;
    Material* mMaterial;
    void Bundle();
    const Mesh::LoadOptions GetLoadOptionsFromMaterial(const Material* material) const;
};