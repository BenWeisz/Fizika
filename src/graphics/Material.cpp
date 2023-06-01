#include "../include/graphics/Material.hpp"

Material::Material(const std::string& path) : mRepresentation(Representation::TRIANGLES) {
    // Initialize the material
    InitMaterial(path);
}

Material::Material(const std::string& path, const Representation rep) : mRepresentation(rep) {
    // Initialize the material
    InitMaterial(path);
}

Material::~Material() {
    delete mShader;
    delete mLayout;
}

void Material::InitMaterial(const std::string& path) {
    // Load the material file
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    // Basic error checking
    if (doc.Error()) {
        std::cout << "ERROR: Failed to load material file: " << path << std::endl;
        doc.PrintError();
        return;
    }

    // Set up the root node
    tinyxml2::XMLElement* rootNode = doc.RootElement();

    // Load the shader
    tinyxml2::XMLElement* shaderNode = rootNode->FirstChildElement("Shader");
    tinyxml2::XMLElement* sourcesNode = shaderNode->FirstChildElement("Sources");

    std::string vertexShaderPath = "";
    std::string fragmentShaderPath = "";

    // Read the paths to the shader sources
    tinyxml2::XMLElement* sourceNode = sourcesNode->FirstChildElement("ShaderSource");

    while (sourceNode != 0 && (vertexShaderPath == "" || fragmentShaderPath == "")) {
        std::string type = SafeXMLAttribute("type", sourceNode);

        // If the path isn't set yet then set it
        if (type == "vertex" && vertexShaderPath == "")
            vertexShaderPath = "../res/shaders/" + SafeXMLAttribute("path", sourceNode);
        else if (type == "fragment" && fragmentShaderPath == "")
            fragmentShaderPath = "../res/shaders/" + SafeXMLAttribute("path", sourceNode);

        sourceNode = sourceNode->NextSiblingElement();
    }

    // Set up the shader for the material
    mShader = new Shader(vertexShaderPath, fragmentShaderPath);

    // // Load the layout
    mLayout = new VertexBufferLayout();

    tinyxml2::XMLElement* vertexBufferLayoutNode = rootNode->FirstChildElement("Layout");
    tinyxml2::XMLElement* layoutElementNode = vertexBufferLayoutNode->FirstChildElement("LayoutElement");
    while (layoutElementNode != 0) {
        std::string nameStr = SafeXMLAttribute("name", layoutElementNode);
        if (nameStr.compare("") == 0)
            std::cout << "ERROR: Element has no \"name\" attribute" << std::endl;
        mAttributes.push_back(nameStr);

        std::string attribSizeStr = SafeXMLAttribute("attribSize", layoutElementNode);
        if (attribSizeStr.compare("") == 0)
            std::cout << "ERROR: Element has no \"attribSize\" attribute" << std::endl;

        int attribSize;
        try {
            attribSize = std::stoi(attribSizeStr);
        } catch (std::invalid_argument const& ex) {
            std::cout << "ERROR: Material File: attribSize is not a number" << ex.what() << '\n';
            std::exit(1);
        }

        if (attribSize < 1 || attribSize > 4) {
            std::cout << "ERROR: attribSize must be between 1 and 4" << std::endl;
            std::exit(1);
        }

        mLayout->AddLayoutElement(nameStr, attribSize);

        layoutElementNode = layoutElementNode->NextSiblingElement();
    }
}

void Material::Bind() const {
    mShader->Bind();
}

void Material::Unbind() const {
    mShader->Unbind();
}

Shader* Material::GetShader() const {
    return mShader;
}

VertexBufferLayout* Material::GetVertexBufferLayout() const {
    return mLayout;
}

void Material::SetUniformVec3(const std::string& name, const glm::vec3& value) {
    mShader->Bind();
    mShader->SetUniformVec3(name, value);
    mShader->Unbind();
}

void Material::SetUniformMat4(const std::string& name, const glm::mat4& value, bool shouldTranspose) {
    mShader->Bind();
    mShader->SetUniformMat4(name, value, shouldTranspose);
    mShader->Unbind();
}

void Material::SetRepresentation(const Representation rep) {
    mRepresentation = rep;
}

Material::Representation Material::GetRepresentation() const {
    return mRepresentation;
}

bool Material::HasAttribute(const std::string& attribName) const {
    return std::find(mAttributes.begin(), mAttributes.end(), attribName) != mAttributes.end();
}

void Material::AddTexture(const std::string& name, Texture* texture) {
    mShader->AddTexture(name, texture);
}