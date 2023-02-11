#pragma once

#include <string>

#include <glad/glad.h>

#include "Mesh.hpp"
#include "Material.hpp"
#include "Transform.hpp"

class Model {
   public:
    Model() = delete;
    Model(const std::string& meshPath, const std::string& materialPath);
    Model(const std::string& meshPath, const Mesh::AttributeSettings attributeSettings);
    Model(const std::string& meshPath);
    ~Model();
    void Draw() const;
    void Draw(const Transform* transform) const;
    Mesh* GetMesh() const;
    Material* GetMaterial() const;
    Transform* GetBaseTransform() const;

   private:
    Mesh* mMesh;
    Material* mMaterial;
    Transform* mBaseTransform;
    void Bundle();
    void _Draw(const Transform* transform) const;
    const Mesh::AttributeSettings GetAttributeSettingsFromMaterial(const Material* material) const;
};