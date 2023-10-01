#pragma once

#include <glm/glm.hpp>

#include "graphics/Window.hpp"
#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/LightManager.hpp"
#include "graphics/Transform.hpp"

class Box {
   private:
    Transform* mTransform;
    Model* mModel;

   public:
    Box();
    ~Box();
    inline Transform* GetTransform() const { return mTransform; }
    void Draw(const Window::RenderPassType& renderPassType) const;
    void Update();
};