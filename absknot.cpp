#include "absknot.h"

#include <algorithm>
#include <set>
#include <numeric>

using Unknotter::AbsCross;
using Unknotter::AbsKnot;

size_t passByOnList(size_t Index, const size_t* Array, size_t Length) {
    size_t lwr = 0;
    size_t upr = Length;
    while (upr - lwr > 1) {
        size_t avg = (upr + lwr) >> 1;
        if (Array[avg] >= Index) {
            lwr = avg;
        }
        else {
            upr = avg;
        }
    }
    return lwr;
}

size_t passByOnList(size_t Index, const std::pair<size_t, AbsCross>* Array, size_t Length) {
    size_t lwr = 0;
    size_t upr = Length;
    while (upr - lwr > 1) {
        size_t avg = (upr + lwr) >> 1;
        if (Array[avg].first >= Index) {
            lwr = avg;
        }
        else {
            upr = avg;
        }
    }
    return lwr;
}

void subtractByList(size_t& Index, const size_t* Array, size_t Length) {
    Index -= passByOnList(Index, Array, Length);
}

void addByList(size_t& Index, const std::pair<size_t, AbsCross>* Array, size_t Length) {
    Index += passByOnList(Index, Array, Length);
}

void removePresorted(AbsKnot& Knot, const size_t* Indicies, size_t IndexCount) {
    size_t c = Indicies[0];
    for (size_t i = 1; i <= IndexCount; ++i) {
        size_t s = Indicies[i - 1] + 1;
        size_t l = ((i == IndexCount) ? Knot.crosses.size() : Indicies[i]) - s;
        memcpy(Knot.crosses.data() + c, Knot.crosses.data() + s, sizeof(AbsCross) * l);
        c += l;
    }
    Knot.crosses.erase(Knot.crosses.end() - IndexCount, Knot.crosses.end());
    for (auto& cross : Knot.crosses) {
        subtractByList(cross.crossingIndex, Indicies, IndexCount);
    }
}

void addPresorted(AbsKnot& Knot, const std::pair<size_t, AbsCross>* Indicies, size_t IndexCount) {
    Knot.crosses.resize(Knot.crosses.size() + IndexCount);
    AbsCross* c = Knot.crosses.data() + Knot.crosses.size();
    for (size_t i = IndexCount; i > 0; --i) {
        size_t j = i - 1;
        auto& element = Indicies[j];
        size_t iV = (i == IndexCount) ? Knot.crosses.size() : Indicies[i].first;
        size_t jV = element.first;
        size_t diff = iV - jV;
        c -= diff;
        memcpy(c, Knot.crosses.data() + jV, diff * sizeof(AbsCross));
        *--c = element.second;
    }
    for (auto& cross : Knot.crosses) {
        addByList(cross.crossingIndex, Indicies, IndexCount);
    }
}

void AbsKnot::RemoveAt(size_t Index, AbsShift* ReferenceShifts) {
    size_t oi = crosses[Index].crossingIndex;
    if (ReferenceShifts) {
        size_t* indicies = new size_t[2];
        if (oi > Index) {
            indicies[0] = Index;
            indicies[1] = oi;
        }
        else {
            indicies[0] = oi;
            indicies[1] = Index;
        }
        removePresorted(*this, indicies, 2);
        *ReferenceShifts = AbsShift(indicies, 2);
    }
    else {
        size_t indicies[2];
        if (oi > Index) {
            indicies[0] = Index;
            indicies[1] = oi;
        }
        else {
            indicies[0] = oi;
            indicies[1] = Index;
        }
        removePresorted(*this, indicies, 2);
    }
}
void AbsKnot::RemoveAt(const size_t* Indicies, size_t IndexCount, AbsShift* ReferenceShifts) {
    size_t* arr = new size_t[IndexCount << 1];
    std::copy(Indicies, Indicies + IndexCount, arr);
    std::sort(arr, arr + IndexCount); //ascending
    size_t* ep = arr + IndexCount;
    size_t* j = ep;
    for (size_t* i = arr; i < ep; ++i) {
        size_t ci = crosses[*i].crossingIndex;
        if (!std::binary_search(arr, ep, ci)) {
            *(j++) = ci;
        }
    }
    std::sort(arr, j); //ascending
    removePresorted(*this, arr, IndexCount);
    if (ReferenceShifts) {
        *ReferenceShifts = AbsShift(arr, j - arr);
    }
    else {
        delete[] arr;
    }
}
void AbsKnot::RemoveAtFull(const size_t* Indicies, size_t IndexCount, AbsShift* ReferenceShifts) {
    if (!IndexCount) {
        return;
    }
    if (IndexCount & 1) {
        throw std::exception("It is impossible for all nodes present to have their corresponding node also present when IndexCount is odd.");
    }
    size_t* arr = new size_t[IndexCount];
    std::copy(Indicies, Indicies + IndexCount, arr);
    std::sort(arr, arr + IndexCount); //ascending
    removePresorted(*this, arr, IndexCount);
    if (ReferenceShifts) {
        *ReferenceShifts = AbsShift(arr, IndexCount);
    }
    else {
        delete[] arr;
    }
}

void AbsKnot::RemoveRange(AbsLength Range, AbsShift* ReferenceShifts) {
    RemoveRange(Range.index1, Range.index2, ReferenceShifts);
}

void AbsKnot::RemoveRange(size_t LowerIndex, size_t UpperIndex, AbsShift* ReferenceShifts) {
    size_t* arr;
    size_t* arr2;
    size_t* arr3;
    if (LowerIndex > UpperIndex) {
        size_t diff = AbsLength(LowerIndex, UpperIndex).RangeSize(crosses.size());
        arr = new size_t[diff << 1];
        size_t* arr1_5 = arr + UpperIndex;
        arr2 = arr + diff;
        std::iota(arr, arr1_5, 0);
        std::iota(arr1_5, arr2, LowerIndex);
        arr3 = arr2;
        for (size_t i = LowerIndex; i < UpperIndex; ++i) {
            size_t idx = crosses[i].crossingIndex;
            if (idx < LowerIndex && idx >= UpperIndex) {
                *(arr3++) = idx;
            }
        }
    }
    else {
        size_t diff = UpperIndex - LowerIndex;
        arr = new size_t[diff << 1];
        arr2 = arr + diff;
        std::iota(arr, arr2, LowerIndex);
        arr3 = arr2;
        for (size_t i = LowerIndex; i < UpperIndex; ++i) {
            size_t idx = crosses[i].crossingIndex;
            if (idx < LowerIndex || idx >= UpperIndex) {
                *(arr3++) = idx;
            }
        }
    }
    std::sort(arr2, arr3);
    size_t finalLength = arr3 - arr;
    size_t* arr4 = new size_t[finalLength];
    size_t* p_v1 = arr;
    size_t* p_v2 = arr2;
    size_t* p_v3 = arr4;
    size_t v1 = *p_v1;
    size_t v2 = *p_v2;
    while (true) {
        if (v2 >= v1) {
            *(p_v3++) = v1;
            v1 = *(p_v1++);
            if (p_v1 >= arr2) {
                memcpy(p_v3, p_v2, (arr3 - p_v2) * sizeof(size_t));
                break;
            }
        }
        else {
            *(p_v3++) = v2;
            v2 = *(p_v2++);
            if (p_v2 >= arr3) {
                memcpy(p_v3, p_v1, (arr2 - p_v1) * sizeof(size_t));
                break;
            }
        }
    }
    removePresorted(*this, arr4, finalLength);
    delete[] arr;
    if (ReferenceShifts) {
        *ReferenceShifts = AbsShift(arr4, finalLength);
    }
    else {
        delete[] arr4;
    }
}

void AbsKnot::AddAt(size_t Index1, size_t Index2, AbsShift* ReferenceShifts) {
    std::pair<size_t, AbsCross> indicies[2];
    if (Index1 < Index2) {
        indicies[0] = std::pair<size_t, AbsCross>(Index1, AbsCross{ Index2, true });
        indicies[1] = std::pair<size_t, AbsCross>(Index2, AbsCross{ Index1, false });
    }
    else {
        indicies[0] = std::pair<size_t, AbsCross>(Index2, AbsCross{ Index1, true });
        indicies[1] = std::pair<size_t, AbsCross>(Index1, AbsCross{ Index2, false });
    }
    addPresorted(*this, indicies, 2);
    if (ReferenceShifts) {
        size_t* shifts = new size_t[2];
        shifts[0] = indicies[0].first;
        shifts[1] = indicies[1].first;
        *ReferenceShifts = AbsShift(shifts, 2);
    }
}

void AbsKnot::AddAt(const std::pair<size_t, size_t>* NewCrosses, size_t CrossCount, AbsShift* ReferenceShifts) {
    size_t l2 = CrossCount << 1;
    auto* newCrossesSorted = new std::pair<size_t, AbsCross>[l2];
    for (size_t i = 0; i < CrossCount; ++i) {
        std::pair<size_t, size_t> c = NewCrosses[i];
        size_t i2 = i << 1;
        newCrossesSorted[i2] = std::pair<size_t, AbsCross>(c.first, { c.second, true });
        newCrossesSorted[i2 | 1] = std::pair<size_t, AbsCross>(c.second, { c.first, false });
    }
    std::sort(newCrossesSorted, newCrossesSorted + l2, [](auto& a, auto& b) {
        return a.first > b.first;
    }); //ascending
    addPresorted(*this, newCrossesSorted, l2);
    if (ReferenceShifts) {
        size_t* shifts = new size_t[l2];
        for (size_t i = 0; i < l2; ++i) {
            shifts[i] = newCrossesSorted[i].first;
        }
        *ReferenceShifts = AbsShift(shifts, l2);
    }
}