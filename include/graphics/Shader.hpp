#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "glad/glad.h"

class Shader {
    public:
        Shader() = default;
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();
        void Bind() const;
        void Unbind() const;
        GLint GetAttribLocation(const std::string& name) const;
    private:
        GLuint mID;
        std::unordered_map<std::string, GLint> mUniformCache;
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