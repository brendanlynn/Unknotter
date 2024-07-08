#include <iostream>
#include "linkedknot.h"
#include "linkedsimplifier.h"

using std::cout;
using std::getchar;

using Unknotter::LinkedCross;
using Unknotter::TryToRemoveImmediately;
using Unknotter::CanBeRemovedImmediately;
using Unknotter::IterateRandomRemovalAttempts;

#define CreateKnotFromPairs LinkedCross::CreateKnotFromPairs
#define KnotAdd LinkedCross::Add
#define KnotRemoveRange LinkedCross::RemoveRange
#define KnotGetVector LinkedCross::CompileAll_Vector
#define KnotGetSet LinkedCross::CompileAll_Set
#define CrossTravelN LinkedCross::TravelN
#define CrossTravelP LinkedCross::TravelP

int main()
{
    constexpr size_t pairsLength = 3;
    std::pair<size_t, size_t> pairs[pairsLength] = {
        { 3, 0 },
        { 4, 1 },
        { 2, 5 }
    };

    LinkedCross* lc = CreateKnotFromPairs(pairs, pairsLength);

    LinkedCross::DisposeAll(lc);

    getchar();
}