#pragma once

#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include "graphics/Light.hpp"
#include "graphics/Transform.hpp"

class Bunny {
   private:
    Transform* mTransform;
    Model* mModel;

   public:
    Bunny();
    ~Bunny();
    void Draw() const;
    void Update();
};