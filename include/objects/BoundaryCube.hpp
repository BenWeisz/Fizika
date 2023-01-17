#pragma once

#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Transform.hpp"

class BoundaryCube {
   private:
    Transform* mTransform;
    Model* mModel;

   public:
    BoundaryCube();
    ~BoundaryCube();
    inline Transform* GetTransform() const { return mTransform; }
    void Draw() const;
    void Update();
};