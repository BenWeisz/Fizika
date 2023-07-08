#pragma once

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Eigen/Core>

#include "util/types.hpp"
#include "fdatabuild/FDataBuildState.hpp"

namespace FDataBuild {

class GeomerySegmentModifier {
   private:
    const Modifier* mModifiers;
    const u32 mNumModifiers;
    glm::mat4 mTransform;

   public:
    GeomerySegmentModifier(const Modifier* operators, const u32 numOperators);
    void Run(f32* positions, const u32 numPositions);

   private:
    void GenerateTransformMatrix();
};

};  // namespace FDataBuild