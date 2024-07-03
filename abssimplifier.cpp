#include "abssimplifier.h"

#include "abslengths.h"
#include <algorithm>

struct AbsChord final {
    Unknotter::AbsLength length;
    bool over;
    constexpr __forceinline AbsChord(Unknotter::AbsLength Length, bool Over)
        : length(Length),
          over(Over) { }
};

__forceinline size_t initIndex(const Unknotter::AbsCircle& Circle) {
    return Circle.l1.index2;
}

__forceinline bool incrementIndex(size_t& Index, const Unknotter::AbsCircle& Circle) {
    ++Index;
    if (Index == Circle.l1.index1) {
        return false;
    }
    if (Index == Circle.l2.index1) {
        Index = Circle.l2.index2;
    }
    return true;
}

bool absChordSortPredicate(AbsChord A, AbsChord B) {
    return A.length.index1 < B.length.index1;
}

__forceinline size_t binarySearchAbsChords(const std::vector<AbsChord>& AbsChords, size_t Index) {
    size_t lwr = 0;
    size_t upr = AbsChords.size();
    while (upr - lwr > 1) {
        size_t avg = (upr + lwr) >> 1;
        if (AbsChords[avg].length.index1 >= Index) {
            lwr = avg;
        }
        else {
            upr = avg;
        }
    }
    return lwr;
}

__forceinline bool areAbsChordsDetatched(const Unknotter::AbsKnot& Knot, const std::vector<AbsChord>& AbsChords) {
    for (const AbsChord& chord : AbsChords) {
        for (size_t i = chord.length.index1 + 1; i < chord.length.index2; ++i) {
            size_t j = binarySearchAbsChords(AbsChords, Knot[i].crossingIndex);
            bool meOver = chord.over;
            bool otherOver = AbsChords[j].over;
            bool meOverOther = Knot[i].over;
            if (meOver != meOverOther && meOver != otherOver) {
                return false;
            }
        }
    }
    return true;
}

bool Unknotter::CanBeRemovedImmediately(const AbsKnot& Knot, size_t StartEndIndex) {
    size_t otherIndex = Knot[StartEndIndex].crossingIndex;
    AbsLength range(StartEndIndex, otherIndex);
    std::vector<AbsChord> absChords;
    {
        size_t i = otherIndex;
        bool inCircle = false;
        size_t inCircleSince;
        bool wentOver;
        do {
            const AbsCross& cross = Knot[i];
            size_t j = cross.crossingIndex;
            if (range.InRange(j)) {
                if (inCircle) {
                    if (wentOver != cross.over) {
                        return false;
                    }
                    AbsLength len(inCircleSince, i);
                    AbsChord chord(len, wentOver);
                    inCircle = false;
                    absChords.push_back(chord);
                }
                else {
                    inCircle = true;
                    inCircleSince = i;
                    wentOver = cross.over;
                }
            }
            ++i;
            if (i == Knot.crosses.size()) {
                i = 0;
            }
        } while (i != StartEndIndex);
    }
    std::sort(absChords.data(), absChords.data() + absChords.size(), absChordSortPredicate);
    return areAbsChordsDetatched(Knot, absChords);
}

bool Unknotter::CanBeRemovedImmediately(const AbsKnot& Knot, const AbsCircle& Circle) {
    std::vector<AbsChord> absChords;
    {
        size_t i = initIndex(Circle);
        bool inCircle = false;
        size_t inCircleSince;
        bool wentOver;
        do {
            const AbsCross& cross = Knot[i];
            size_t j = cross.crossingIndex;
            if (Circle.OnCircle(j)) {
                if (inCircle) {
                    if (wentOver != cross.over) {
                        return false;
                    }
                    AbsLength len(inCircleSince, i);
                    AbsChord chord(len, wentOver);
                    inCircle = false;
                    absChords.push_back(chord);
                }
                else {
                    inCircle = true;
                    inCircleSince = i;
                    wentOver = cross.over;
                }
            }
        } while (incrementIndex(i, Circle));
    }
    std::sort(absChords.data(), absChords.data() + absChords.size(), absChordSortPredicate);
    return areAbsChordsDetatched(Knot, absChords);
}

bool Unknotter::TryToRemoveImmediately(AbsKnot& Knot, size_t StartEndIndex) {
    if (CanBeRemovedImmediately(Knot, StartEndIndex)) {
        Knot.RemoveRange(StartEndIndex, Knot[StartEndIndex].crossingIndex);
        return true;
    }
    return false;
}