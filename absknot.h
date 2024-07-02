#pragma once

#include <vector>

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
        void RemoveAt(size_t* Indicies, size_t IndexCount);
        //Removes the crossovers indicated by the indicies. The function assumes that all nodes in the list also have their corresponding node in the list. If this assumption is false, the behavior of the function is undefined, and may corrupt the class.
        void RemoveAtFull(size_t* Indicies, size_t IndexCount);
        //The number of crossovers. The number of nodes is always twice the number of crossovers.
        inline size_t CrossoverCount() const {
            return crosses.size() >> 1;
        }
        //Returns the crossover node at the given index.
        inline AbsCross& operator[](size_t Index) {
            return crosses[Index];
        }
    };
}