#pragma once

#include <vector>

#include "graphics/Light.hpp"

class LightManager {
   private:
    static std::vector<PointLight> PointLights;
    static std::vector<DirectionalLight> DirectionalLights;

   public:
    static void AddPointLight(const glm::vec3& color, const glm::vec3& position) {
        PointLights.push_back({color, position});
    }
    static void AddDirectionalLight(const glm::vec3& color, const glm::vec3& direction, const glm::vec3& position) {
        DirectionalLights.push_back({color, direction, position});
    }

    static PointLight* GetPointLight(const unsigned int index) {
        if (index < PointLights.size())
            return &(PointLights[index]);

        return nullptr;
    }

    static DirectionalLight* GetDirectionalLight(const unsigned int index) {
        if (index < DirectionalLights.size())
            return &(DirectionalLights[index]);

        return nullptr;
    }
};