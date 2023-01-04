#pragma once

#include <string>

#include "Mesh.hpp"
#include "Material.hpp"

class Model {
   public:
    Model() = delete;
    Model(const std::string& meshPath, const std::string& materialPath);
    Model(const Geometry geometry, const std::string& materialPath);
    Model(const std::string& meshPath, const Mesh::LoadOptions loadOptions);
    Model(const Geometry geometry);
    ~Model();
    void Draw() const;
    Mesh* GetMesh() const;
    Material* GetMaterial() const;

   private:
    Mesh* mMesh;
    Material* mMaterial;
    void InitModel(const std::string& meshPath, const Mesh::LoadOptions meshOptions, const std::string& materialPath);
    void Bundle();
};