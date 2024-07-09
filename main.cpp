#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "linkedknot.h"
#include "linkedsimplifier.h"

using std::cout;
using std::getchar;
using std::mt19937_64;

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

uint64_t getMills() {
    return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
    mt19937_64 mt(getMills());

    constexpr size_t pairsLength = 3;
    std::pair<size_t, size_t> pairs[pairsLength] = {
        { 3, 0 },
        { 4, 1 },
        { 2, 5 }
    };

    while (true) {
        auto* p_allCrosses = KnotGetSet(CreateKnotFromPairs(pairs, pairsLength));
        auto& allCrosses = *p_allCrosses;

        cout << IterateRandomRemovalAttempts(allCrosses, mt, 1000) << "\n";

        LinkedCross::DisposeAll(allCrosses);
        delete p_allCrosses;

        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(250));
    }

    getchar();
}