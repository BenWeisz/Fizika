#include "fdatabuild/modifiers/GeomerySegmentModifier.hpp"

namespace FDataBuild {

GeomerySegmentModifier::GeomerySegmentModifier(const Modifier* modifiers, const u32 numModifiers) : mModifiers(modifiers), mNumModifiers(numModifiers) {
    // Compute the transformation for this geometry segment
    GenerateTransformMatrix();
}

// Naive Eigen Implementation
void GeomerySegmentModifier::Run(f32* positions, const u32 numPositions) {
    // Build the Eigen Matrix
    Eigen::Matrix4f transform;
    transform << mTransform[0][0], mTransform[1][0], mTransform[2][0], mTransform[3][0],
        mTransform[0][1], mTransform[1][1], mTransform[2][1], mTransform[3][1],
        mTransform[0][2], mTransform[1][2], mTransform[2][2], mTransform[3][2],
        mTransform[0][3], mTransform[1][3], mTransform[2][3], mTransform[3][3];

    // Copy the data over
    Eigen::MatrixXf positionData(4, numPositions);
    u32 positionOffset;
    for (u32 positionIndex = 0; positionIndex < numPositions; positionIndex++) {
        positionOffset = positionIndex * 3;
        positionData(0, positionIndex) = positions[positionOffset];
        positionData(1, positionIndex) = positions[positionOffset + 1];
        positionData(2, positionIndex) = positions[positionOffset + 2];
        positionData(3, positionIndex) = 1.0;
    }

    // Transform the data
    positionData = transform * positionData;

    // Copy the data back
    for (u32 positionIndex = 0; positionIndex < numPositions; positionIndex++) {
        positionOffset = positionIndex * 3;
        positions[positionOffset] = positionData(0, positionIndex);
        positions[positionOffset + 1] = positionData(1, positionIndex);
        positions[positionOffset + 2] = positionData(2, positionIndex);
    }
}

void GeomerySegmentModifier::GenerateTransformMatrix() {
    mTransform = glm::mat4(1.0f);
    for (u32 i = 0; i < mNumModifiers; i++) {
        const Modifier* modifier = &(mModifiers[mNumModifiers - i - 1]);
        const float v0 = modifier->v0;
        const float v1 = modifier->v1;
        const float v2 = modifier->v2;

        if (modifier->mModifierType == ModifierType::SCALE)
            mTransform = glm::scale(mTransform, glm::vec3(v0, v1, v2));
        else if (modifier->mModifierType == ModifierType::TRANSLATE)
            mTransform = glm::translate(mTransform, glm::vec3(v0, v1, v2));
        else if (modifier->mModifierType == ModifierType::ROTATE) {
            mTransform = glm::rotate(mTransform, glm::radians(v2), glm::vec3(1.0f, 0.0f, 0.0f));
            mTransform = glm::rotate(mTransform, glm::radians(v1), glm::vec3(0.0f, 0.0f, 1.0f));
            mTransform = glm::rotate(mTransform, glm::radians(v0), glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
}

};  // namespace FDataBuild