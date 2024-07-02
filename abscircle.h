#pragma once

#include "abslengths.h"
#include "absknot.h"

namespace Unknotter {
    struct AbsCircle final {
        AbsLength l1;
        AbsLength l2;
        constexpr inline AbsCircle(size_t Index11, size_t Index12, size_t Index21, size_t Index22)
            : l1(Index11, Index12),
              l2(Index21, Index22) { }
        constexpr inline AbsCircle(size_t Index1, size_t Index2, AbsKnot& Knot)
            : l1(Index1, Index2),
              l2(Knot.crosses[Index1].crossingIndex, Knot.crosses[Index2].crossingIndex) { }
        constexpr inline AbsCircle(AbsLength Length, AbsKnot& Knot)
            : AbsCircle(Length.index1, Length.index2, Knot) { }
        constexpr inline bool OnCircle(size_t Index) {
            return l1.InRange(Index) || l2.InRange(Index);
        }
    };
}