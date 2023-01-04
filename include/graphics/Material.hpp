#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "glad/glad.h"
#include "tinyxml2.h"

class Material {
   public:
    enum Representation {
        POINTS,
        LINES,
        TRIANGLES
    };

    Material(const std::string& path);
    Material(const std::string& path, const Representation rep);
    ~Material();
    void Bind() const;
    void Unbind() const;
    Shader* GetShader() const;
    VertexBufferLayout* GetVertexBufferLayout() const;
    void SetUniformVec3(const std::string& name, const glm::vec3& value);
    void SetUniformMat4(const std::string& name, const glm::mat4& value);
    void SetRepresentation(const Representation rep);
    Representation GetRepresentation() const;
    bool HasAttribute(const std::string& attribName) const;

   private:
    Shader* mShader;
    VertexBufferLayout* mLayout;
    Representation mRepresentation;
    std::vector<std::string> mAttributes;
    void InitMaterial(const std::string& path);
    static std::string SafeXMLAttribute(const std::string& name, tinyxml2::XMLElement* element) {
        const char* attribStr = element->Attribute(name.c_str());
        if (attribStr == 0) {
            std::cout << "ERROR: Layout element has no \"" << name << "\" attribute" << std::endl;
            return std::string("");
        }

        return std::string(attribStr);
    }
};