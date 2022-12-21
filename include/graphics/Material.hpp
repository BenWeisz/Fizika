#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "glad/glad.h"
#include "tinyxml2.h"

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"

class Material {
    public:
        Material(const std::string& path);
        std::shared_ptr<Shader> GetShader() const;
    private:
        std::shared_ptr<Shader> mShader;
        // VertexBufferLayout mLayout;
        static std::string SafeXMLAttribute(const std::string& name, tinyxml2::XMLElement* element) {
            const char* attribStr = element->Attribute(name.c_str());
            if (attribStr == 0) {
                std::cout << "ERROR: Layout element has no \"" << name << "\" attribute" << std::endl;
                return std::string("");
            }

            return std::string(attribStr);
        }
};