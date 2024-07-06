#pragma once

#include <algorithm>

namespace Unknotter {
    class AbsShift final {
    private:
        const size_t* data;
        size_t length;
    public:
        __forceinline const size_t* Data() const;
        __forceinline size_t Length() const;

        __forceinline AbsShift(const size_t* Data, size_t Length);

        static __forceinline AbsShift FromCopy(const size_t* Data, size_t Length);
        static __forceinline AbsShift FromSort(size_t* Data, size_t Length);
        static __forceinline AbsShift FromReverse(size_t* Data, size_t Length);
        static __forceinline AbsShift FromCopySort(const size_t* Data, size_t Length);
        static __forceinline AbsShift FromCopyReverse(const size_t* Data, size_t Length);

        __forceinline size_t GetShiftDelta(size_t Index) const;
        __forceinline size_t ShiftPVal(size_t Index) const;
        __forceinline size_t ShiftNVal(size_t Index) const;
        __forceinline void ShiftP(size_t& Index) const;
        __forceinline void ShiftN(size_t& Index) const;
    };
}

__forceinline const size_t* Unknotter::AbsShift::Data() const {
    return data;
}
__forceinline size_t Unknotter::AbsShift::Length() const {
    return length;
}
__forceinline Unknotter::AbsShift::AbsShift(const size_t* Data, size_t Length)
    : data(Data),
      length(Length) { }
__forceinline Unknotter::AbsShift Unknotter::AbsShift::FromCopy(const size_t* Data, size_t Length) {
    size_t* data = new size_t[Length];
    std::copy(Data, Data + Length, data);
    return AbsShift(data, Length);
}
__forceinline Unknotter::AbsShift Unknotter::AbsShift::FromSort(size_t* Data, size_t Length) {
    std::sort(Data, Data + Length);
    return AbsShift(Data, Length);
}
__forceinline Unknotter::AbsShift Unknotter::AbsShift::FromReverse(size_t* Data, size_t Length) {
    std::reverse(Data, Data + Length);
    return AbsShift(Data, Length);
}
__forceinline Unknotter::AbsShift Unknotter::AbsShift::FromCopySort(const size_t* Data, size_t Length) {
    size_t* data = new size_t[Length];
    std::copy(Data, Data + Length, data);
    std::sort(data, data + Length);
    return AbsShift(data, Length);
}
__forceinline Unknotter::AbsShift Unknotter::AbsShift::FromCopyReverse(const size_t* Data, size_t Length) {
    size_t* data = new size_t[Length];
    std::reverse_copy(Data, Data + Length, data);
    return AbsShift(data, Length);
}
__forceinline size_t Unknotter::AbsShift::GetShiftDelta(size_t Index) const {
    size_t lwr = 0;
    size_t upr = length;
    while (upr - lwr > 1) {
        size_t avg = (upr + lwr) >> 1;
        if (data[avg] >= Index) {
            lwr = avg;
        }
        else {
            upr = avg;
        }
    }
    return lwr;
}
__forceinline size_t Unknotter::AbsShift::ShiftPVal(size_t Index) const {
    return Index + GetShiftDelta(Index);
}
__forceinline size_t Unknotter::AbsShift::ShiftNVal(size_t Index) const {
    return Index - GetShiftDelta(Index);
}
__forceinline void Unknotter::AbsShift::ShiftP(size_t& Index) const {
    Index += GetShiftDelta(Index);
}
__forceinline void Unknotter::AbsShift::ShiftN(size_t& Index) const {
    Index -= GetShiftDelta(Index);
}