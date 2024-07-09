#pragma once

#include "linkedknot.h"
#include <random>

namespace Unknotter {
    bool CanBeRemovedImmediately(LinkedCross* PrimaryStart, bool PrimaryUpper, LinkedCross* PrimaryEnd, bool SecondaryForward);
    bool CanBeRemovedImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartUpper);

    bool TryToRemoveImmediately(LinkedCross* PrimaryStart, bool PrimaryUpper, LinkedCross* PrimaryEnd, bool SecondaryForward, std::unordered_set<LinkedCross*>* AllCrosses = 0);
    bool TryToRemoveImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartUpper, std::unordered_set<LinkedCross*>* AllCrosses = 0);

    size_t IterateRandomRemovalAttempts(LinkedCross* Sample, std::mt19937_64& RNG, uint64_t Iterations);
}