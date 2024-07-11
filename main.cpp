#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "linkedknot.h"
#include "linkedsimplifier.h"

using std::cout;
using std::getchar;
using std::mt19937_64;
using std::stringstream;

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
        auto* p_startCrosses = CreateKnotFromPairs(pairs, pairsLength);
        auto& startCrosses = *p_startCrosses;
        auto* p_allCrosses = new std::unordered_set<LinkedCross*>(startCrosses.data(), startCrosses.data() + startCrosses.size());
        auto& allCrosses = *p_allCrosses;

        size_t sSize = allCrosses.size();
        auto t1 = std::chrono::high_resolution_clock::now();
        IterateRemovalAttempts(allCrosses);
        auto t2 = std::chrono::high_resolution_clock::now();
        size_t eSize = allCrosses.size();
        std::stringstream ss;
        ss << "Starting crossover count: " << sSize << "; time taken:" << std::setw(9) << (t2 - t1) << "; ending crossover count: " << eSize << ".\n";
        cout << ss.str();

        LinkedCross::DisposeAll(allCrosses);
        delete p_startCrosses;
        delete p_allCrosses;

        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(250));
    }
}