#include "../include/graphics/Material.hpp"

Material::Material(const std::string& path) {
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
    tinyxml2::XMLElement* sourceNode = sourcesNode->FirstChildElement("ShaderSource"); ;
    while (sourceNode != 0 && (vertexShaderPath == "" || fragmentShaderPath == "")) {
        std::string type = Material::SafeXMLAttribute("type", sourceNode);

        // If the path isn't set yet then set it
        if (type == "vertex" && vertexShaderPath == "")
            vertexShaderPath = "../res/" + Material::SafeXMLAttribute("path", sourceNode);
        else if (type == "fragment" && fragmentShaderPath == "")
            fragmentShaderPath = "../res/" + Material::SafeXMLAttribute("path", sourceNode);

        sourceNode = sourceNode->NextSiblingElement();
    }

    // Set up the shader for the material
    mShader = std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath);

    // // Load the layout
    

    // tinyxml2::XMLElement* bufferLayoutNode = rootNode->FirstChildElement("BufferLayout");
    // tinyxml2::XMLElement* layoutElementNode = bufferLayoutNode->FirstChildElement("LayoutElement");
    // while (layoutElementNode != 0) {
    //     const char* nameStr = SafeXMLAttribute("name", layoutElementNode);
    //     const char* attribSizeStr = SafeXMLAttribute("attribSize", layoutElementNode);



    //     layoutElementNode = layoutElementNode->NextSiblingElement();
    // }

    // std::cout << bufferLayoutNode->Name() << std::endl;

}

std::shared_ptr<Shader> Material::GetShader() const {
    return mShader;
}