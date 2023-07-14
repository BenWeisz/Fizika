#pragma once

#include "util/types.hpp"

inline u32 Math_CantorsPairing(u32 k1, u32 k2) {
    return (u32)((0.5 * (k1 + k2) * (k1 + k2 + 1)) + k2);
}