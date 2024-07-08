#pragma once

#include "linkedknot.h"

namespace Unknotter {
    bool CanBeRemovedImmediately(LinkedCross* PrimaryStart, bool PrimaryUpper, LinkedCross* PrimaryEnd, bool SecondaryForward);
    bool CanBeRemovedImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartUpper);

    bool TryToRemoveImmediately(LinkedCross* PrimaryStart, bool PrimaryUpper, LinkedCross* PrimaryEnd, bool SecondaryForward);
    bool TryToRemoveImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartUpper);
}