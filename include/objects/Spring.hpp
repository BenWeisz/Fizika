#pragma once

#include <cmath>

#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Light.hpp"
#include "graphics/Transform.hpp"

class Spring {
   private:
    Texture* mTexture;
    Transform* mTransform;
    Model* mModel;

   public:
    Spring();
    ~Spring();
    inline Transform* GetTransform() const { return mTransform; }
    void Draw() const;
    void Update(const double dt);
};