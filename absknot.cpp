#include "absknot.h"

#include <algorithm>
#include <set>
#include <numeric>

void subtractByList(size_t& Index, const size_t* Array, size_t Length) {
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
    Index -= lwr;
}

void removePresorted(Unknotter::AbsKnot& Knot, size_t* Indicies, size_t IndexCount) {
    size_t c = Indicies[0];
    for (size_t i = 1; i < IndexCount; ++i) {
        size_t s = Indicies[i - 1] + 1;
        size_t l = Indicies[i] - s;
        memcpy(Knot.crosses.data() + c, Knot.crosses.data() + s, sizeof(Unknotter::AbsCross) * l);
    }
    for (auto& cross : Knot.crosses) {
        subtractByList(cross.crossingIndex, Indicies, IndexCount);
    }
}

void Unknotter::AbsKnot::RemoveAt(size_t Index) {
    size_t indicies[2];
    indicies[0] = Index;
    indicies[1] = crosses[Index].crossingIndex;
    RemoveAtFull(indicies, 2);
}
void Unknotter::AbsKnot::RemoveAt(const size_t* Indicies, size_t IndexCount) {
    std::vector<size_t> arr(IndexCount);
    memcpy(arr.data(), Indicies, IndexCount * sizeof(size_t));
    std::sort(arr.data(), arr.data() + IndexCount); //ascending
    size_t* ep = arr.data() + IndexCount;
    for (size_t i = 0; i < IndexCount; ++i) {
        if (!std::binary_search(arr.data(), ep, i)) {
            arr.push_back(i);
        }
    }
    RemoveAtFull(arr.data(), arr.size());
}
void Unknotter::AbsKnot::RemoveAtFull(const size_t* Indicies, size_t IndexCount) {
    if (!IndexCount) {
        return;
    }
    if (IndexCount & 1) {
        throw std::exception("It is impossible for all nodes present to have their corresponding node also present when IndexCount is odd.");
    }
    std::vector<size_t> arr(IndexCount);
    memcpy(arr.data(), Indicies, IndexCount * sizeof(size_t));
    std::sort(arr.data(), arr.data() + IndexCount); //ascending
    removePresorted(*this, arr.data(), IndexCount);
}

void Unknotter::AbsKnot::RemoveRange(AbsLength Range) {
    RemoveRange(Range.index1, Range.index2);
}

void Unknotter::AbsKnot::RemoveRange(size_t LowerIndex, size_t UpperIndex) {
    if (LowerIndex > UpperIndex) {
        //optimize
        RemoveRange(LowerIndex, crosses.size());
        RemoveRange(0, UpperIndex);
    }
    //run benchmarks
#if false
    std::set<size_t> otherElements;
    for (size_t i = LowerIndex; i < UpperIndex; ++i) {
        size_t idx = crosses[i].crossingIndex;
        if (idx < LowerIndex || idx >= UpperIndex) {
            otherElements.insert(idx);
        }
    }
    size_t diff = UpperIndex - LowerIndex;
    size_t* arr = new size_t[diff + otherElements.size()];
    size_t* arr2 = arr;
    bool dropped = false;
    for (size_t v : otherElements) {
        if (dropped || v < LowerIndex) {
            *(arr2++) = v;
        }
        else {
            size_t* arr3 = arr2 + diff;
            std::iota(arr2, arr3, LowerIndex);
            arr2 = arr3;
            dropped = true;
        }
    }
    removePresorted(*this, arr, arr2 - arr);
    delete[] arr;
#elif false
    size_t diff = UpperIndex - LowerIndex;
    size_t* arr = new size_t[diff << 1];
    size_t* arr2 = arr + diff;
    std::iota(arr, arr2, LowerIndex);
    for (size_t i = LowerIndex; i < UpperIndex; ++i) {
        size_t idx = crosses[i].crossingIndex;
        if (idx < LowerIndex || idx >= UpperIndex) {
            *(arr2++) = idx;
        }
    }
    std::sort(arr, arr2);
    removePresorted(*this, arr, arr2 - arr);
    delete[] arr;
#else
    size_t diff = UpperIndex - LowerIndex;
    size_t* arr = new size_t[diff << 1];
    size_t* arr2 = arr + diff;
    std::iota(arr, arr2, LowerIndex);
    size_t* arr3 = arr2;
    for (size_t i = LowerIndex; i < UpperIndex; ++i) {
        size_t idx = crosses[i].crossingIndex;
        if (idx < LowerIndex || idx >= UpperIndex) {
            *(arr3++) = idx;
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
    delete[] arr4;
#endif
}