#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "glad/glad.h"
#include "tinyxml2.h"

class Material {
   public:
    Material(const std::string& path);
    ~Material();
    void Bind() const;
    void Unbind() const;
    Shader* GetShader() const;
    VertexBufferLayout* GetVertexBufferLayout() const;
    void SetUniformVec3(const std::string& name, const glm::vec3& value);
    void SetUniformMat4(const std::string& name, const glm::mat4& value);

   private:
    Shader* mShader;
    VertexBufferLayout* mLayout;
    static std::string SafeXMLAttribute(const std::string& name, tinyxml2::XMLElement* element) {
        const char* attribStr = element->Attribute(name.c_str());
        if (attribStr == 0) {
            std::cout << "ERROR: Layout element has no \"" << name << "\" attribute" << std::endl;
            return std::string("");
        }

        return std::string(attribStr);
    }
};