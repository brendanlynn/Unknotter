#include "absknot.h"

#include <algorithm>
#include <set>
#include <numeric>

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

size_t passByOnList(size_t Index, const std::pair<size_t, size_t>* Array, size_t Length) {
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

void addByList(size_t& Index, const std::pair<size_t, size_t>* Array, size_t Length) {
    Index += passByOnList(Index, Array, Length);
}

void removePresorted(Unknotter::AbsKnot& Knot, size_t* Indicies, size_t IndexCount) {
    size_t c = Indicies[0];
    for (size_t i = 1; i <= IndexCount; ++i) {
        size_t s = Indicies[i - 1] + 1;
        size_t l = ((i == IndexCount) ? Knot.crosses.size() : Indicies[i]) - s;
        memcpy(Knot.crosses.data() + c, Knot.crosses.data() + s, sizeof(Unknotter::AbsCross) * l);
        c += l;
    }
    Knot.crosses.erase(Knot.crosses.end() - IndexCount, Knot.crosses.end());
    for (auto& cross : Knot.crosses) {
        subtractByList(cross.crossingIndex, Indicies, IndexCount);
    }
}

void addPresorted(Unknotter::AbsKnot& Knot, std::pair<size_t, size_t>* Indicies, size_t IndexCount) {
    Knot.crosses.resize(Knot.crosses.size() + IndexCount);
    Unknotter::AbsCross* c = Knot.crosses.data() + Knot.crosses.size();
    for (size_t i = IndexCount; i > 0; --i) {
        size_t j = i - 1;
        size_t iV = (i == IndexCount) ? Knot.crosses.size() : Indicies[i].first;
        size_t jV = Indicies[j].first;
        size_t diff = iV - jV;
        c -= diff;
        memcpy(c, Knot.crosses.data() + jV, diff * sizeof(Unknotter::AbsCross));
    }
    for (auto& cross : Knot.crosses) {
        addByList(cross.crossingIndex, Indicies, IndexCount);
    }
}

void Unknotter::AbsKnot::RemoveAt(size_t Index) {
    size_t oi = crosses[Index].crossingIndex;
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
    std::sort(arr.data(), arr.data() + IndexCount); //ascending
    removePresorted(*this, arr.data(), IndexCount);
}
void Unknotter::AbsKnot::RemoveAtFull(const size_t* Indicies, size_t IndexCount) {
    if (!IndexCount) {
        return;
    }
    if (IndexCount & 1) {
        throw std::exception("It is impossible for all nodes present to have their corresponding node also present when IndexCount is odd.");
    }
    std::vector<size_t> arr(Indicies, Indicies + IndexCount);
    std::sort(arr.data(), arr.data() + IndexCount); //ascending
    removePresorted(*this, arr.data(), IndexCount);
}

void Unknotter::AbsKnot::RemoveRange(AbsLength Range) {
    RemoveRange(Range.index1, Range.index2);
}

void Unknotter::AbsKnot::RemoveRange(size_t LowerIndex, size_t UpperIndex) {
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
    delete[] arr4;
}

void Unknotter::AbsKnot::AddAt(size_t Index1, size_t Index2) {
    std::pair<size_t, size_t> indicies[2];
    if (Index1 < Index2) {
        indicies[0] = std::pair<size_t, size_t>(Index1, Index2);
        indicies[1] = std::pair<size_t, size_t>(Index2, Index1);
    }
    else {
        indicies[0] = std::pair<size_t, size_t>(Index2, Index1);
        indicies[1] = std::pair<size_t, size_t>(Index1, Index2);
    }
    addPresorted(*this, indicies, 2);
}

void Unknotter::AbsKnot::AddAt(const std::pair<size_t, size_t>* NewCrosses, size_t CrossCount) {
    const std::pair<size_t, size_t>* newCrossesUpper = NewCrosses + CrossCount;
    std::set<std::pair<size_t, size_t>> crossSet(NewCrosses, newCrossesUpper);
    std::vector<std::pair<size_t, size_t>> newCrossesResizable(NewCrosses, newCrossesUpper);
    for (const std::pair<size_t, size_t>* p_i = NewCrosses; p_i < newCrossesUpper; ++p_i) {
        const std::pair<size_t, size_t>& i = *p_i;
        const std::pair<size_t, size_t> ri(i.second, i.first);
        if (!crossSet.contains(ri)) {
            newCrossesResizable.push_back(ri);
        }
    }
    std::sort(newCrossesResizable.data(), newCrossesResizable.data() + CrossCount, [](auto& a, auto& b) {
        return a.first > b.first;
    }); //ascending
    addPresorted(*this, newCrossesResizable.data(), newCrossesResizable.size());
}

void Unknotter::AbsKnot::AddAtFull(const std::pair<size_t, size_t>* NewCrosses, size_t CrossCount) {
    if (!CrossCount) {
        return;
    }
    if (CrossCount & 1) {
        throw std::exception("It is impossible for all nodes present to have their corresponding node also present when IndexCount is odd.");
    }
    std::vector<std::pair<size_t, size_t>> arr(NewCrosses, NewCrosses + CrossCount);
    std::sort(arr.data(), arr.data() + CrossCount, [](auto& a, auto& b) {
        return a.first > b.first;
    }); //ascending
    addPresorted(*this, arr.data(), CrossCount);
}