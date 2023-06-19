#pragma once

#include <vector>

#include "util/types.hpp"

/*
 *   State variables enums and structs for the DataBuild system
 */

namespace DataBuild {

enum GeometryMode {
    STATIC,
    DYNAMIC
};

enum PrimitiveType {
    TRIANGLES,
    LINES
};

enum ShaderType {
    VERTEX,
    FRAGMENT
};

enum PipelineOperatorType {
    SCALE,
    TRANSLATE,
    ROTATE
};

struct PipelineOperator {
    PipelineOperatorType mPipelineOperatorType;
    f32 v0;
    f32 v1;
    f32 v2;
};

struct PipelineInputData {
    f32* mPositions;
    f32* mUVs;

    u32* mPositionIndices;
    u32* mUVIndices;
    u32 mVertexArity;
};

};  // namespace DataBuild