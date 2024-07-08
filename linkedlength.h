#pragma once

#include "linkedknot.h"
#include <unordered_set>
#include <vector>

namespace Unknotter {
    struct LinkedLength final {
        //The inclusive lower bound of the range.
        LinkedCrossPointer lower;
        //The exclusive upper bound of the range.
        LinkedCrossPointer upper;

        __forceinline LinkedLength();
        __forceinline LinkedLength(LinkedCrossPointer Lower, LinkedCrossPointer Upper);
        __forceinline LinkedLength(LinkedCross* Lower_Pointer, bool Lower_Over, LinkedCross* Upper_Pointer, bool Upper_Over);

        __forceinline std::vector<LinkedCrossPointer>* CompileLength() const;
        __forceinline std::unordered_set<LinkedCross*>* CompileLength_Set() const;
        __forceinline std::vector<LinkedCross*>* CompileLength_Vec() const;
    };
}

__forceinline Unknotter::LinkedLength::LinkedLength() = default;
__forceinline Unknotter::LinkedLength::LinkedLength(LinkedCrossPointer Lower, LinkedCrossPointer Upper)
    : lower(Lower),
      upper(Upper) { }
__forceinline Unknotter::LinkedLength::LinkedLength(LinkedCross* Lower_Pointer, bool Lower_Over, LinkedCross* Upper_Pointer, bool Upper_Over)
    : lower(Lower_Pointer, Lower_Over),
      upper(Upper_Pointer, Upper_Over) { }
__forceinline std::vector<Unknotter::LinkedCrossPointer>* Unknotter::LinkedLength::CompileLength() const {
    auto* p_vec = new std::vector<Unknotter::LinkedCrossPointer>;
    auto& vec = *p_vec;

    LinkedCrossPointer lcp = lower;
    do vec.push_back(lcp);
    while (LinkedCross::TravelN(*(const LinkedCross**)&lcp.r, lcp.over), lcp != upper);
    return p_vec;
}
__forceinline std::unordered_set<Unknotter::LinkedCross*>* Unknotter::LinkedLength::CompileLength_Set() const {
    auto* p_set = new std::unordered_set<Unknotter::LinkedCross*>;
    auto& set = *p_set;

    LinkedCrossPointer lcp = lower;
    do set.insert(lcp.r);
    while (LinkedCross::TravelN(*(const LinkedCross**)&lcp.r, lcp.over), lcp != upper);
    return p_set;
}
__forceinline std::vector<Unknotter::LinkedCross*>* Unknotter::LinkedLength::CompileLength_Vec() const {
    auto* p_set = CompileLength_Set();
    auto& set = *p_set;
    auto* p_vec = new std::vector<Unknotter::LinkedCross*>(set.size());
    auto& vec = *p_vec;

    size_t i = 0;
    for (auto* ptr : set) {
        vec[i++] = ptr;
    }

    delete p_set;

    return p_vec;
}