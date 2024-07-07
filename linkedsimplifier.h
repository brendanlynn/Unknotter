#pragma once

#include "linkedknot.h"

namespace Unknotter {
    bool CanBeRemovedImmediately(LinkedCross* PrimaryStart, bool PrimaryStart_Over, LinkedCross* PrimaryEnd, bool SecondaryForward);
    bool CanBeRemovedImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartStartAbove);
}