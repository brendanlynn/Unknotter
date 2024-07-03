#include "absknot.h"

#include <algorithm>

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
    size_t c = arr[0];
    for (size_t i = 1; i < IndexCount; ++i) {
        size_t s = arr[i - 1] + 1;
        size_t l = arr[i] - s;
        memcpy(crosses.data() + c, crosses.data() + s, sizeof(AbsCross) * l);
    }
    for (AbsCross& cross : crosses) {
        subtractByList(cross.crossingIndex, arr.data(), IndexCount);
    }
}