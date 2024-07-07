#include "abssimplifier.h"

#include "abslength.h"
#include <algorithm>

using Unknotter::AbsCircle;
using Unknotter::AbsCross;
using Unknotter::AbsKnot;
using Unknotter::AbsLength;

struct AbsChord final {
    AbsLength length;
    bool over;
    constexpr __forceinline AbsChord(AbsLength Length, bool Over)
        : length(Length),
          over(Over) { }
};

__forceinline size_t initIndex(const AbsCircle& Circle) {
    return Circle.l1.index2;
}

__forceinline bool incrementIndex(size_t& Index, const AbsCircle& Circle) {
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

__forceinline bool areAbsChordsDetatched(const AbsKnot& Knot, const std::vector<AbsChord>& AbsChords) {
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

bool getChordsConditionally(const AbsKnot& Knot, size_t StartEndIndex, std::vector<AbsChord>*& AbsChords) {
    size_t otherIndex = Knot[StartEndIndex].crossingIndex;
    AbsLength range(StartEndIndex, otherIndex);
    auto* p_absChords = new std::vector<AbsChord>;
    std::vector<AbsChord>& absChords = *p_absChords;
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
    AbsChords = p_absChords;
}

bool getChordsConditionally(const AbsKnot& Knot, const AbsCircle& Circle, std::vector<AbsChord>*& AbsChords) {
    bool upperPrimary = Knot[Circle.l1.index1].over;
    if (upperPrimary != Knot[Circle.l1.index2].over) {
        return false;
    }
    auto* p_absChords = new std::vector<AbsChord>;
    std::vector<AbsChord>& absChords = *p_absChords;
    {
        size_t i = initIndex(Circle);
        bool inCircle = false;
        size_t inCircleSince;
        bool lastOver;
        bool lastPrimary;
        do {
            const AbsCross& cross = Knot[i];
            size_t j = cross.crossingIndex;

            bool primary;
            if (Circle.l1.InRange(j)) primary = true;
            else if (Circle.l2.InRange(j)) primary = false;
            else continue;

            bool over = cross.over;

            if (inCircle) {
                if (lastOver != over) {
                    if (lastPrimary) {
                        if (primary) {
                            return false;
                        }
                        else {
                            if (upperPrimary != over) {
                                return false;
                            }
                        }
                    }
                    else {
                        if (primary) {
                            if (upperPrimary == over) {
                                return false;
                            }
                        }
                        else {
                            return false;
                        }
                    }
                }
                AbsLength len(inCircleSince, i);
                AbsChord chord(len, lastOver);
                inCircle = false;
                absChords.push_back(chord);
            }
            else {
                inCircle = true;
                inCircleSince = i;
                lastOver = over;
                lastPrimary = primary;
            }
        } while (incrementIndex(i, Circle));
    }
    std::sort(absChords.data(), absChords.data() + absChords.size(), absChordSortPredicate);
    AbsChords = p_absChords;
}

bool Unknotter::CanBeRemovedImmediately(const AbsKnot& Knot, size_t StartEndIndex) {
    std::vector<AbsChord>* p_absChords;
    getChordsConditionally(Knot, StartEndIndex, p_absChords);
    bool r = areAbsChordsDetatched(Knot, *p_absChords);
    delete p_absChords;
    return r;
}

bool Unknotter::CanBeRemovedImmediately(const AbsKnot& Knot, const AbsCircle& Circle) {
    std::vector<AbsChord>* p_absChords;
    getChordsConditionally(Knot, Circle, p_absChords);
    bool r = areAbsChordsDetatched(Knot, *p_absChords);
    delete p_absChords;
    return r;
}

bool Unknotter::TryToRemoveImmediately(AbsKnot& Knot, size_t StartEndIndex) {
    if (CanBeRemovedImmediately(Knot, StartEndIndex)) {
        Knot.RemoveRange(StartEndIndex, Knot[StartEndIndex].crossingIndex, 0);
        return true;
    }
    return false;
}