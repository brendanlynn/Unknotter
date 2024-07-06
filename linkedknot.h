#pragma once

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
        //Removes a node from the rest of the network. The function only disconnects the node; it does not dispose of it. It is the user's responsibility to make the choice whether or not to reuse the memory or delete it.
        void Remove();
    private:
        static __forceinline void GoToRef(const LinkedCross*& Current, bool& Over, const LinkedCrossPointer& Ref);
    public:
        //Travels to the next node, of the direction given by Over. To turn, not Over.
        static __forceinline void TravelN(const LinkedCross*& Current, bool& Over);
        //Travels to the previous node, of the direction given by Over. To turn, not Over.
        static __forceinline void TravelP(const LinkedCross*& Current, bool& Over);
        //Connects the string behind two nodes, Upper crossing over Lower. If NewMemory is null, the function will allocate memory resources on its own.
        static LinkedCross* Add(LinkedCrossReference Upper, LinkedCrossReference Lower);
        //Removes a range of crossovers, between Lower.r and Upper, starting in the direction of Lower.over.
        static void RemoveRange(LinkedCrossPointer Lower, LinkedCross* Upper);
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