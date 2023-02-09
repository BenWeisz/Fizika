#pragma once

#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Light.hpp"
#include "graphics/Transform.hpp"

class CardboardBox {
   private:
    Texture* mTexture;
    Transform* mTransform;
    Model* mModel;

   public:
    CardboardBox();
    ~CardboardBox();
    inline Transform* GetTransform() const { return mTransform; }
    void Draw() const;
    void Update();
};