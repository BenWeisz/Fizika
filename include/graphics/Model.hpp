#pragma once

#include <string>

#include <glad/glad.h>

#include "Window.hpp"
#include "LightManager.hpp"
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
    void Draw(const Transform* transform, const Window::RenderPassType& renderPassType) const;
    Mesh* GetMesh() const;
    Material* GetMaterial() const;

   private:
    Mesh* mMesh;
    Material* mMaterial;
    void Bundle();
    const Mesh::AttributeSettings GetAttributeSettingsFromMaterial(const Material* material) const;
};