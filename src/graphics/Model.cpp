#include "graphics/Model.hpp"

Model::Model(const std::string& meshPath, const std::string& materialPath) {
}

Model::Model(const Geometry geometry, const std::string& materialPath) {
    std::string meshPath = Mesh::GetMeshPrimitivePath(geometry);

    // Load options for mesh to be determined based on some "built-in" attributes provided in the material file
    // InitModel(meshPath, loadOptions, materialPath);
}

Model::Model(const std::string& meshPath, const Mesh::LoadOptions loadOptions) {
    // InitModel(meshPath, loadOptions, "../res/");
}

Model::Model(const Geometry geometry) {
}

Model::~Model() {
    delete mMesh;
    delete mMaterial;
}

void Model::InitModel(const std::string& meshPath, const Mesh::LoadOptions meshOptions, const std::string& materialPath) {
    mMesh = new Mesh(meshPath, meshOptions);
    mMaterial = new Material(materialPath);
}

void Model::Draw() const {
}

Mesh* Model::GetMesh() const {
    return mMesh;
}

Material* Model::GetMaterial() const {
    return mMaterial;
}
