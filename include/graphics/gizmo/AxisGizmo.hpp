#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "graphics/Window.hpp"
#include "graphics/Camera.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexBufferLayout.hpp"
#include "graphics/Shader.hpp"

class AxisGizmo {
   private:
    double mScreenX;
    double mScreenY;
    VertexBuffer* mVBO;
    IndexBuffer* mIBO;
    VertexArray* mVAO;
    Shader* mShader;

   public:
    AxisGizmo(const double screenX, const double screenY);
    ~AxisGizmo();
    void Draw() const;
};