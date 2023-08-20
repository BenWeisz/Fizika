#include "graphics/Model.hpp"

Model::Model(const std::string& meshPath, const std::string& materialPath) {
    mMaterial = new Material(materialPath);

    Mesh::AttributeSettings meshAttributeSettings = GetAttributeSettingsFromMaterial(mMaterial);
    mMesh = new Mesh(meshPath, meshAttributeSettings);

    // Bundle the mesh and the material
    Bundle();
}

Model::Model(const std::string& meshPath, const Mesh::AttributeSettings attribSettings) {
    mMaterial = new Material("../res/materials/base.mat");
    mMesh = new Mesh(meshPath, attribSettings);

    // Bundle the mesh and the material
    Bundle();
}

Model::Model(const std::string& meshPath) {
    mMaterial = new Material("../res/materials/base.mat");

    Mesh::AttributeSettings meshAttributeSettings = GetAttributeSettingsFromMaterial(mMaterial);
    mMesh = new Mesh(meshPath, meshAttributeSettings);

    // Bundle the mesh and the material
    Bundle();
}

Model::~Model() {
    delete mMesh;
    delete mMaterial;
}

void Model::Draw(const Transform* transform) const {
    glm::mat4 model(1.0f);

    model = model * transform->GetTranslationMatrix() * transform->GetRotationMatrix() * transform->GetScaleMatrix();

    mMaterial->SetUniformMat4("uModel", model, false);

    Shader* shader = mMaterial->GetShader();

    // Bind the shader and vertex array object
    shader->Bind();

    VertexArray* vao = mMesh->GetVertexArray();
    vao->Bind();

    IndexBuffer* ibo = mMesh->GetIndexBuffer();

    GLenum polygonMode = Material::GetRepresentationPolygonEnum(mMaterial->GetRepresentation());
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Draw the model
    Mesh::PrimitiveType primitiveType = mMesh->GetPrimitiveType();
    glDrawElements(primitiveType, ibo->GetCount(), GL_UNSIGNED_INT, (void*)0);

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

const Mesh::AttributeSettings Model::GetAttributeSettingsFromMaterial(const Material* material) const {
    /* Determine the proper load options for the mesh based on
       the attributes provided in the material file. */
    Mesh::AttributeSettings meshAttributeSettings = (Mesh::AttributeSettings)(0);

    // Get the load options based on the attributes present in the material
    if (mMaterial->HasAttribute("iPosition"))
        meshAttributeSettings = (Mesh::AttributeSettings)(meshAttributeSettings | Mesh::AttributeSettings::LOAD_POSITIONS);
    if (mMaterial->HasAttribute("iNormal"))
        meshAttributeSettings = (Mesh::AttributeSettings)(meshAttributeSettings | Mesh::AttributeSettings::LOAD_NORMALS);
    if (mMaterial->HasAttribute("iTextureUV"))
        meshAttributeSettings = (Mesh::AttributeSettings)(meshAttributeSettings | Mesh::AttributeSettings::LOAD_TEXTURES);

    return meshAttributeSettings;
}
