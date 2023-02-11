#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include "tinyxml2.h"
#include "glad/glad.h"

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"

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
    void SetUniformMat4(const std::string& name, const glm::mat4& value, bool shouldTranspose);
    void SetRepresentation(const Representation rep);
    Representation GetRepresentation() const;
    bool HasAttribute(const std::string& attribName) const;
    void AddTexture(const std::string& name, Texture* texture);

    static GLenum GetRepresentationPolygonEnum(const Representation rep) {
        switch (rep) {
            case Representation::POINTS:
                return GL_POINT;
            case Representation::LINES:
                return GL_LINE;
            case Representation::TRIANGLES:
                return GL_FILL;
            default:
                return GL_FILL;
        }
    }

   private:
    Shader* mShader;
    VertexBufferLayout* mLayout;
    Representation mRepresentation;
    std::vector<std::string> mAttributes;

    void InitMaterial(const std::string& path);
    static std::string SafeXMLAttribute(const std::string& name, tinyxml2::XMLElement* element) {
        const char* attribStr = element->Attribute(name.c_str());
        if (attribStr == 0)
            return std::string("");

        return std::string(attribStr);
    }
};