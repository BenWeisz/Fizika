#include "graphics/Model.hpp"

Model::Model(const std::string& meshPath, const std::string& materialPath) {
    mMaterial = new Material(materialPath);

    Mesh::LoadOptions meshLoadOptions = GetLoadOptionsFromMaterial(mMaterial);
    mMesh = new Mesh(meshPath, meshLoadOptions);

    // Bundle the mesh and the material
    Bundle();
}

Model::Model(const Geometry geometry, const std::string& materialPath) {
    // Set up the material from the material file
    mMaterial = new Material(materialPath);

    std::string meshPath = Mesh::GetMeshPrimitivePath(geometry);
    Mesh::LoadOptions meshLoadOptions = GetLoadOptionsFromMaterial(mMaterial);

    mMesh = new Mesh(meshPath, meshLoadOptions);

    // Bundle the mesh and the material
    Bundle();
}

Model::Model(const std::string& meshPath, const Mesh::LoadOptions loadOptions) {
    mMaterial = new Material("../res/base.mat");
    mMesh = new Mesh(meshPath, loadOptions);

    // Bundle the mesh and the material
    Bundle();
}

Model::Model(const Geometry geometry) {
    mMaterial = new Material("../res/base.mat");

    std::string meshPath = Mesh::GetMeshPrimitivePath(geometry);
    Mesh::LoadOptions meshLoadOptions = GetLoadOptionsFromMaterial(mMaterial);

    mMesh = new Mesh(meshPath, meshLoadOptions);

    // Bundle the mesh and the material
    Bundle();
}

Model::Model(const std::string& meshPath) {
    mMaterial = new Material("../res/base.mat");

    Mesh::LoadOptions meshLoadOptions = GetLoadOptionsFromMaterial(mMaterial);

    mMesh = new Mesh(meshPath, meshLoadOptions);

    // Bundle the mesh and the material
    Bundle();
}

Model::~Model() {
    delete mMesh;
    delete mMaterial;
}

void Model::Draw() const {
    Shader* shader = mMaterial->GetShader();

    // Bind the shader and vertex array object
    shader->Bind();

    VertexArray* vao = mMesh->GetVertexArray();
    vao->Bind();

    IndexBuffer* ibo = mMesh->GetIndexBuffer();

    GLenum polygonMode = Material::GetRepresentationPolygonEnum(mMaterial->GetRepresentation());
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Draw the model
    GLenum drawMode = Mesh::GetDrawModeEnum(mMesh->GetGeometry());
    glDrawElements(drawMode, ibo->GetCount(), GL_UNSIGNED_INT, (void*)0);

    vao->Unbind();
    shader->Unbind();
}

Mesh* Model::GetMesh() const {
    return mMesh;
}

Material* Model::GetMaterial() const {
    return mMaterial;
}

void Model::Bundle() {
    Shader* shader = mMaterial->GetShader();
    VertexBufferLayout* layout = mMaterial->GetVertexBufferLayout();

    VertexBuffer* vbo = mMesh->GetVertexBuffer();
    IndexBuffer* ibo = mMesh->GetIndexBuffer();
    VertexArray* vao = mMesh->GetVertexArray();
    vao->Bundle(vbo, ibo, layout, shader);
}

const Mesh::LoadOptions Model::GetLoadOptionsFromMaterial(const Material* material) const {
    /* Determine the proper load options for the mesh based on
       the attributes provided in the material file. */
    Mesh::LoadOptions loadOptions = (Mesh::LoadOptions)(0);

    // Get the load options based on the attributes present in the material
    if (mMaterial->HasAttribute("iPosition"))
        loadOptions = (Mesh::LoadOptions)(loadOptions | Mesh::LoadOptions::POSITIONS);
    if (mMaterial->HasAttribute("iNormal"))
        loadOptions = (Mesh::LoadOptions)(loadOptions | Mesh::LoadOptions::NORMALS);
    if (mMaterial->HasAttribute("iTextureUV"))
        loadOptions = (Mesh::LoadOptions)(loadOptions | Mesh::LoadOptions::TEXTURES);

    return loadOptions;
}