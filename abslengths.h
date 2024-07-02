#pragma once

namespace Unknotter {
    //A length of consecutive crossovers.
    struct AbsLength final {
        //The inclusive lower bound of the range. If index1 > index2, the range will wrap around.
        size_t index1;
        //The exclusive upper bound of the range. If index1 > index2, the range will wrap around.
        size_t index2;
        //Initializes an AbsLength instance.
        constexpr inline AbsLength(size_t Index1, size_t Index2)
            : index1(Index1), index2(Index2) { }
        //Returns true if a given index is within the range, false otherwise.
        constexpr inline bool InRange(size_t Index) const {
            if (index1 <= index2) {
                return (Index >= index1 && Index < index2);
            }
            else {
                return (Index >= index1 || Index < index2);
            }
        }
        constexpr inline AbsLength Inverse() const {
            return AbsLength(index2, index1);
        }
    };
}