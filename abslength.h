#pragma once

namespace Unknotter {
    //A length of consecutive crossovers.
    struct AbsLength final {
        //The inclusive lower bound of the range. If index1 > index2, the range will wrap around. If index1 == index2, the range will encompass no points.
        size_t index1;
        //The exclusive upper bound of the range. If index1 > index2, the range will wrap around. If index1 == index2, the range will encompass no points.
        size_t index2;
        //Initializes an AbsLength instance.
        constexpr __forceinline AbsLength(size_t Index1, size_t Index2);
        //Returns true if a given index is within the range, false otherwise.
        constexpr __forceinline bool InRange(size_t Index) const;
        //Reverses the range, such that points that were considered inside are now considered outside, and vice versa.
        constexpr __forceinline AbsLength Inverse() const;
        //Computes the amount of indicies in the range.
        constexpr __forceinline size_t RangeSize(size_t TotalSize) const;
        //Computes the equivilance or lack of such of two AbsLength instances.
        constexpr __forceinline bool operator==(AbsLength Other) const;
        //Computes the equivilance or lack of such of two AbsLength instances.
        constexpr __forceinline bool operator!=(AbsLength Other) const;
    };
}

constexpr __forceinline Unknotter::AbsLength::AbsLength(size_t Index1, size_t Index2)
    : index1(Index1), index2(Index2) { }
constexpr __forceinline bool Unknotter::AbsLength::InRange(size_t Index) const {
    if (index1 <= index2) {
        return (Index >= index1 && Index < index2);
    }
    else {
        return (Index >= index1 || Index < index2);
    }
}
constexpr __forceinline Unknotter::AbsLength Unknotter::AbsLength::Inverse() const {
    return AbsLength(index2, index1);
}
constexpr __forceinline size_t Unknotter::AbsLength::RangeSize(size_t TotalSize) const {
    if (index1 <= index2) {
        return index2 - index1;
    }
    else {
        return TotalSize - index1 + index2;
    }
}
constexpr __forceinline bool Unknotter::AbsLength::operator==(AbsLength Other) const {
    return index1 == Other.index1 && index2 == Other.index2;
}
constexpr __forceinline bool Unknotter::AbsLength::operator!=(AbsLength Other) const {
    return index1 != Other.index1 || index2 != Other.index2;
}