#pragma once

#include <glm/glm.hpp>

class Light {
   private:
    static glm::vec3 LightColour;
    static glm::vec3 LightPosition;

   public:
    Light() = delete;
    static void InitLight(const glm::vec3& colour, const glm::vec3& position) {
        LightColour = colour;
        LightPosition = position;
    }
    static glm::vec3 GetLightColour() {
        return LightColour;
    }
    static glm::vec3 GetLightPosition() {
        return LightPosition;
    }
};