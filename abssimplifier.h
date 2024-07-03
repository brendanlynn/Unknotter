#pragma once

#include "abscircle.h"
#include "absknot.h"

namespace Unknotter {
    bool CanBeRemovedImmediately(const AbsKnot& Knot, size_t StartEndIndex);
    bool CanBeRemovedImmediately(const AbsKnot& Knot, const AbsCircle& Area);

    bool TryToRemoveImmediately(AbsKnot& Knot, size_t StartEndIndex);
}