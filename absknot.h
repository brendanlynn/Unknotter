#pragma once

#include <vector>
#include "abslength.h"
#include "absshift.h"

namespace Unknotter {
    //A node of AbsCross.
    struct AbsCross final {
        //The index from the other segment's perspective.
        size_t crossingIndex;
        //Whether this segment is going over or under the other segment whose index is crossingIndex.
        bool over;
    };
    //A representation of a mathematical knot, with all crossovers in an array with each node storing the index of the corresponding node.
    struct AbsKnot final {
        std::vector<AbsCross> crosses;
        //Removes the crossover indicated by the index. Two nodes will be removed in total, the one referenced, and the one corresponding.
        void RemoveAt(size_t Index, AbsShift* ReferenceShifts);
        //Removes the crossovers indicated by the indicies. If a node is present without it's corresponding node, the corresponding node will also be removed.
        void RemoveAt(const size_t* Indicies, size_t IndexCount, AbsShift* ReferenceShifts);
        //Removes the crossovers indicated by the indicies. The function assumes that all nodes in the list also have their corresponding node in the list. If this assumption is false, the behavior of the function is undefined, and may corrupt the struct.
        void RemoveAtFull(const size_t* Indicies, size_t IndexCount, AbsShift* ReferenceShifts);
        //Removes the crossovers in the given range. Both the nodes in the range and the nodes corresponding to nodes in the range will be removed. The lower bound of the range is inclusive; the upper bound of the range is not.
        void RemoveRange(AbsLength Range, AbsShift* ReferenceShifts);
        //Removes the crossovers in the given range. Both the nodes in the range and the nodes corresponding to nodes in the range will be removed. The lower bound of the range is inclusive; the upper bound of the range is not.
        void RemoveRange(size_t LowerIndex, size_t UpperIndex, AbsShift* ReferenceShifts);
        //Adds a crossover at the position specified. Behavior is undefined if the indicies are equal. Each index should be in the range [0, crosses.size()].
        void AddAt(size_t Index1, size_t Index2, AbsShift* ReferenceShifts);
        //Adds crossovers at the positions specified. Behavior is undefined if a pair of indicies are equal to each other. Each index should be in the range [0, crosses.size()].
        void AddAt(const std::pair<size_t, size_t>* NewCrosses, size_t CrossCount, AbsShift* ReferenceShifts);
        //The number of crossovers. The number of nodes is always twice the number of crossovers.
        __forceinline size_t CrossoverCount() const;
        //Returns the crossover node at the given index.
        __forceinline AbsCross& operator[](size_t Index);
        //Returns the crossover node at the given index.
        __forceinline const AbsCross& operator[](size_t Index) const;
    };
}

__forceinline size_t Unknotter::AbsKnot::CrossoverCount() const {
    return crosses.size() >> 1;
}
__forceinline Unknotter::AbsCross& Unknotter::AbsKnot::operator[](size_t Index) {
    return crosses[Index];
}
__forceinline const Unknotter::AbsCross& Unknotter::AbsKnot::operator[](size_t Index) const {
    return crosses[Index];
}