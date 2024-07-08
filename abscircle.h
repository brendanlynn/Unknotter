#pragma once

#include "abslength.h"
#include "absknot.h"

namespace Unknotter {
    struct AbsCircle final {
        AbsLength l1;
        AbsLength l2;
        constexpr __forceinline AbsCircle(size_t Index11, size_t Index12, size_t Index21, size_t Index22);
        constexpr __forceinline AbsCircle(size_t Index1, size_t Index2, AbsKnot& Knot);
        constexpr __forceinline AbsCircle(AbsLength Length, AbsKnot& Knot);
        constexpr __forceinline bool OnCircle(size_t Index) const;
    };
}

constexpr __forceinline Unknotter::AbsCircle::AbsCircle(size_t Index11, size_t Index12, size_t Index21, size_t Index22)
    : l1(Index11, Index12),
    l2(Index21, Index22) { }
constexpr __forceinline Unknotter::AbsCircle::AbsCircle(size_t Index1, size_t Index2, AbsKnot& Knot)
    : l1(Index1, Index2),
    l2(Knot[Index1].crossingIndex, Knot[Index2].crossingIndex) { }
constexpr __forceinline Unknotter::AbsCircle::AbsCircle(AbsLength Length, AbsKnot& Knot)
    : AbsCircle(Length.index1, Length.index2, Knot) { }
constexpr __forceinline bool Unknotter::AbsCircle::OnCircle(size_t Index) const {
    return l1.InRange(Index) || l2.InRange(Index);
}