#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Texture.hpp"

class Shader {
   public:
    Shader() = delete;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void Bind() const;
    void Unbind() const;
    GLint GetAttribLocation(const std::string& name) const;
    void SetUniformVec3(const std::string& name, const glm::vec3& value);
    void SetUniformMat4(const std::string& name, const glm::mat4& value, bool shouldTranspose);
    void SetUniformInt(const std::string& name, const int value);
    void AddTexture(const std::string& name, Texture* texture);

   private:
    GLuint mID;
    std::unordered_map<std::string, GLint> mUniformCache;
    std::vector<Texture*> mTextureIDs;
    static std::string ReadSource(const std::string& path) {
        std::ifstream file(path.c_str(), std::ifstream::in);

        if (!file.is_open()) {
            std::cout << "ERROR: Could not open shader file: " << path << std::endl;
            return "";
        }

        // Shader file is ok to read
        std::string file_str;
        while (file.good()) {
            std::string line;
            std::getline(file, line);
            file_str += line + "\n";
        }

        // Free the resources
        file.close();

        return file_str;
    }
    GLint CompileShader(const std::string& source, GLuint type);
    GLint GetUniformLocation(const std::string& name);
};