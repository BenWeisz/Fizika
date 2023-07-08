#pragma once

#include <vector>

#include "util/types.hpp"

/*
 *   State variables enums and structs for the FDataBuild system
 */

namespace FDataBuild {

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

enum ModifierType {
    SCALE,
    TRANSLATE,
    ROTATE
};

struct Modifier {
    ModifierType mModifierType;
    f32 v0;
    f32 v1;
    f32 v2;
};

};  // namespace FDataBuild