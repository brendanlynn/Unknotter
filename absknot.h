#pragma once

#include <vector>
#include "abslengths.h"

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
        void RemoveAt(size_t Index);
        //Removes the crossovers indicated by the indicies. If a node is present without it's corresponding node, the corresponding node will also be removed.
        void RemoveAt(const size_t* Indicies, size_t IndexCount);
        //Removes the crossovers indicated by the indicies. The function assumes that all nodes in the list also have their corresponding node in the list. If this assumption is false, the behavior of the function is undefined, and may corrupt the class.
        void RemoveAtFull(const size_t* Indicies, size_t IndexCount);
        //Removes the crossovers in the given range. Both the nodes in the range and the nodes corresponding to nodes in the range will be removed. The lower bound of the range is inclusive; the upper bound of the range is not.
        void RemoveRange(AbsLength Range);
        //Removes the crossovers in the given range. Both the nodes in the range and the nodes corresponding to nodes in the range will be removed. The lower bound of the range is inclusive; the upper bound of the range is not.
        void RemoveRange(size_t LowerIndex, size_t UpperIndex);
        //The number of crossovers. The number of nodes is always twice the number of crossovers.
        __forceinline size_t CrossoverCount() const {
            return crosses.size() >> 1;
        }
        //Returns the crossover node at the given index.
        __forceinline AbsCross& operator[](size_t Index) {
            return crosses[Index];
        }
        //Returns the crossover node at the given index.
        __forceinline const AbsCross& operator[](size_t Index) const {
            return crosses[Index];
        }
    };
}