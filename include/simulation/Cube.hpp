#pragma once

#include <glm/glm.hpp>

#include "graphics/Camera.hpp"
#include "graphics/Model.hpp"

class Cube {
   private:
    Model* mModel;
    glm::vec3 mPosition;

   public:
    Cube(const glm::vec3& position);
    ~Cube();
    void Draw() const;
};