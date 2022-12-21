#include "graphics/Shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = Shader::ReadSource(vertexPath);
    std::string fragmentSource = Shader::ReadSource(fragmentPath);

    if (vertexSource == "" || fragmentSource == "") {
        std::cout << "ERROR: Failed to acquire both sources for shader" << std::endl;
        return;
    }    

    // Compile the separate shader sources
    GLint vertexShaderID = CompileShader(vertexSource, GL_VERTEX_SHADER);
    GLint fragmentShaderID = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

    // Check for errors with compilation
    if (vertexShaderID == -1 || fragmentShaderID == -1)
        return;

    // Set up the shader program
    mID = glCreateProgram();

    glAttachShader(mID, (GLuint)vertexShaderID);
    glAttachShader(mID, (GLuint)fragmentShaderID);

    // Link the shader sources
    glLinkProgram(mID);

    // Error check the linking
    GLint linkingStatus;
    glGetProgramiv(mID, GL_LINK_STATUS, &linkingStatus);
    if (linkingStatus != GL_TRUE) {
        GLchar message[1024];
        glGetProgramInfoLog(mID, 1024, NULL, message);
        std::cout << "Error: Failed to link shader" << std::endl;
    }

    glDeleteShader((GLuint)vertexShaderID);    
    glDeleteShader((GLuint)fragmentShaderID);
}

Shader::~Shader() {
    glDeleteProgram(mID);
}

void Shader::Bind() const {
    glUseProgram(mID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

GLint Shader::GetAttribLocation(const std::string& name) const {
    const char* cName = name.c_str();
    return glGetAttribLocation(mID, cName);
}

GLint Shader::GetUniformLocation(const std::string& name) {
    auto locationPair = mUniformCache.find(name);
    if (locationPair == mUniformCache.end()) {
        const char* cName = name.c_str();
        GLint location = glGetUniformLocation(mID, cName);
        mUniformCache.emplace(name, location);

        return location;
    }
    
    return locationPair->second;
}

GLint Shader::CompileShader(const std::string& source, GLuint type) {
    // Create a resource handle
    GLuint shaderID = glCreateShader(type);

    // Type string
    const std::string typeStr = type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader";

    // Attach the source to the shader resource
    const char* cSource = source.c_str();
    glShaderSource(shaderID, 1, &cSource, NULL);

    // Compile the shader
    glCompileShader(shaderID);

    // Error checking
    GLint compilationStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus);
    if (!compilationStatus) {
        GLchar message[1024];
        glGetShaderInfoLog(shaderID, 1024, NULL, message);
        std::cout << "ERROR: Failed to compile glsl source for " << typeStr << ":\n" << message << std::endl;
        return -1;
    }

    return (GLint)shaderID;
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value) {
    GLint location = GetUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}