#pragma once

#include <unordered_set>

namespace Unknotter {
    struct LinkedCross;
    struct LinkedCrossPointer;
    struct LinkedCrossReference;

    struct LinkedCrossPointer final {
        LinkedCross* r;
        bool over;
        __forceinline LinkedCrossPointer();
        __forceinline LinkedCrossPointer(LinkedCross& Ref, bool Over);
        __forceinline LinkedCrossPointer(LinkedCross* Ref, bool Over);
        __forceinline LinkedCrossPointer(const LinkedCrossReference& Ref);
        __forceinline explicit operator LinkedCrossReference();
    };

    struct LinkedCrossReference final {
        LinkedCross& r;
        bool over;
        __forceinline LinkedCrossReference(LinkedCross& Ref, bool Over);
        __forceinline LinkedCrossReference(LinkedCross* Ref, bool Over);
        __forceinline LinkedCrossReference(LinkedCrossPointer Ref);
        __forceinline operator LinkedCrossPointer();
    };

    struct LinkedCross final {
        LinkedCrossPointer l_p;
        LinkedCrossPointer l_n;
        LinkedCrossPointer u_p;
        LinkedCrossPointer u_n;
        //Removes a node from the rest of the network. The function only disconnects the node from the rest of the network; it does not dispose of the memory the node takes up.
        void Remove();
    private:
        static __forceinline void GoToRef(const LinkedCross*& Current, bool& Over, const LinkedCrossPointer& Ref);
    public:
        //Travels to the next node, aligned to the direction given by Over. To turn, not Over.
        static __forceinline void TravelN(const LinkedCross*& Current, bool& Over);
        //Travels to the previous node, aligned to the direction given by Over. To turn, not Over.
        static __forceinline void TravelP(const LinkedCross*& Current, bool& Over);
        //Connects the string behind two nodes, Upper crossing over Lower.
        static LinkedCross* Add(LinkedCrossReference Upper, LinkedCrossReference Lower);
        //Connects the string after/behind two nodes, Upper crossing over Lower.
        template <bool _AfterUpper, bool _AfterLower>
        static LinkedCross* Add(LinkedCrossReference Upper, LinkedCrossReference Lower);
        //Removes a range of crossovers, between Lower.r and Upper, starting in the direction of Lower.over.
        static void RemoveRange(LinkedCrossPointer Lower, LinkedCross* Upper);
        //Removes a range of crossovers, between Lower.r and Upper, starting in the direction of Lower.over.
        static void RemoveRange(LinkedCrossPointer Lower, LinkedCrossPointer Upper);
        //Compiles a unordered set of all the pointers in the knot.
        static std::unordered_set<LinkedCross*>* CompileAll_Set(LinkedCross* Sample);
        //Compiles a vector of all the pointers in the knot.
        static std::vector<LinkedCross*>* CompileAll_Vector(LinkedCross* Sample);
        //Disposes of all memory taken up by a knot.
        static void DisposeAll(LinkedCross* Sample);
        //Creates a knot from a number of index-index pairs, with the first value of each pair being the index crossing over, and the second being the index crossing under. The function returns a pointer to one of the crosses, indescriminately.
        static LinkedCross* CreateKnotFromPairs(std::pair<size_t, size_t>* Pairs, size_t Pairs_Length);
    };
}

__forceinline Unknotter::LinkedCrossPointer::LinkedCrossPointer()
    : r(0),
      over(false) { }
__forceinline Unknotter::LinkedCrossPointer::LinkedCrossPointer(LinkedCross& Ref, bool Over)
    : r(&Ref),
      over(Over) { }
__forceinline Unknotter::LinkedCrossPointer::LinkedCrossPointer(LinkedCross* Ref, bool Over)
    : r(Ref),
      over(Over) { }
__forceinline Unknotter::LinkedCrossPointer::LinkedCrossPointer(const LinkedCrossReference& Ref)
    : r(&Ref.r),
      over(Ref.over) { }
__forceinline Unknotter::LinkedCrossPointer::operator Unknotter::LinkedCrossReference() {
    return LinkedCrossReference(r, over);
}

__forceinline Unknotter::LinkedCrossReference::LinkedCrossReference(LinkedCross& Ref, bool Over)
    : r(Ref),
      over(Over) { }
__forceinline Unknotter::LinkedCrossReference::LinkedCrossReference(LinkedCross* Ref, bool Over)
    : r(*Ref),
      over(Over) { }
__forceinline Unknotter::LinkedCrossReference::LinkedCrossReference(LinkedCrossPointer Ref)
    : r(*Ref.r),
      over(Ref.over) { }
__forceinline Unknotter::LinkedCrossReference::operator Unknotter::LinkedCrossPointer() {
    return LinkedCrossPointer(r, over);
}

__forceinline void Unknotter::LinkedCross::GoToRef(const LinkedCross*& Current, bool& Over, const LinkedCrossPointer& Ref) {
    Current = Ref.r;
    Over = Ref.over;
}
__forceinline void Unknotter::LinkedCross::TravelN(const LinkedCross*& Current, bool& Over) {
    if (Over) {
        GoToRef(Current, Over, Current->u_n);
    }
    else {
        GoToRef(Current, Over, Current->l_n);
    }
}
__forceinline void Unknotter::LinkedCross::TravelP(const LinkedCross*& Current, bool& Over) {
    if (Over) {
        GoToRef(Current, Over, Current->u_p);
    }
    else {
        GoToRef(Current, Over, Current->l_p);
    }
}