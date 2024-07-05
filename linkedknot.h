#pragma once

namespace Unknotter {
    struct LinkedCross;
    struct LinkedCrossPointer;
    struct LinkedCrossReference;

    struct LinkedCrossPointer final {
        LinkedCross* r;
        bool over;
        __forceinline LinkedCrossPointer() = default;
        __forceinline LinkedCrossPointer(LinkedCross& Ref, bool Over)
            : r(&Ref),
              over(Over) { }
        __forceinline LinkedCrossPointer(LinkedCross* Ref, bool Over)
            : r(Ref),
              over(Over) { }
        __forceinline LinkedCrossPointer(const LinkedCrossReference& Ref)
            : r(&Ref.r),
              over(Ref.over) { }
        __forceinline explicit operator LinkedCrossReference() {
            return LinkedCrossReference(r, over);
        }
    };

    struct LinkedCrossReference final {
        LinkedCross& r;
        bool over;
        __forceinline LinkedCrossReference() = default;
        __forceinline LinkedCrossReference(LinkedCross& Ref, bool Over)
            : r(Ref),
              over(Over) { }
        __forceinline LinkedCrossReference(LinkedCross* Ref, bool Over)
            : r(*Ref),
              over(Over) { }
        __forceinline LinkedCrossReference(LinkedCrossPointer Ref)
            : r(*Ref.r),
              over(Ref.over) { }
        __forceinline operator LinkedCrossPointer() {
            return LinkedCrossPointer(r, over);
        }
    };

    struct LinkedCross final {
        LinkedCrossPointer l_p;
        LinkedCrossPointer l_n;
        LinkedCrossPointer u_p;
        LinkedCrossPointer u_n;
        __forceinline void Remove() {
            if (l_p.over) {
                l_p.r->u_n = l_n;
            }
            else {
                l_p.r->l_n = l_n;
            }
            if (l_n.over) {
                l_n.r->u_p = l_p;
            }
            else {
                l_n.r->l_p = l_p;
            }
            if (u_p.over) {
                u_p.r->u_n = u_n;
            }
            else {
                u_p.r->l_n = u_n;
            }
            if (u_n.over) {
                u_n.r->u_p = u_p;
            }
            else {
                u_n.r->l_p = u_p;
            }
        }
    private:
        static __forceinline void GoToRef(const LinkedCross*& Current, bool& Over, const LinkedCrossPointer& Ref) {
            Current = Ref.r;
            Over = Ref.over;
        }
    public:
        static __forceinline void TravelN(const LinkedCross*& Current, bool& Over) {
            if (Over) {
                GoToRef(Current, Over, Current->u_n);
            }
            else {
                GoToRef(Current, Over, Current->l_n);
            }
        }
        static __forceinline void TravelP(const LinkedCross*& Current, bool& Over) {
            if (Over) {
                GoToRef(Current, Over, Current->u_p);
            }
            else {
                GoToRef(Current, Over, Current->l_p);
            }
        }
        static __forceinline void Add(LinkedCrossReference Upper, LinkedCrossReference Lower, LinkedCross* NewMemory) {
            if (!NewMemory) {
                NewMemory = new LinkedCross;
            }

            LinkedCross& nlc = *NewMemory;

            bool un_u = Upper.over;
            LinkedCross& un = Upper.r;
            bool ln_u = Lower.over;
            LinkedCross& ln = Lower.r;

            LinkedCrossReference upperPre = un_u ? un.u_p : un.l_p;
            LinkedCrossReference lowerPre = ln_u ? ln.u_p : ln.l_p;

            bool up_u = upperPre.over;
            LinkedCross& up = upperPre.r;
            bool lp_u = lowerPre.over;
            LinkedCross& lp = lowerPre.r;

            if (un_u) {
                LinkedCrossPointer& p = un.u_p;
                nlc.u_p = p;
                p.over = true;
                p.r = NewMemory;
            }
            else {
                LinkedCrossPointer& p = un.l_p;
                nlc.u_p = p;
                p.over = true;
                p.r = NewMemory;
            }

            if (up_u) {
                LinkedCrossPointer& p = up.u_n;
                nlc.u_n = p;
                p.over = true;
                p.r = NewMemory;
            }
            else {
                LinkedCrossPointer& p = up.l_n;
                nlc.u_n = p;
                p.over = true;
                p.r = NewMemory;
            }

            if (ln_u) {
                LinkedCrossPointer& p = ln.u_p;
                nlc.l_p = p;
                p.over = false;
                p.r = NewMemory;
            }
            else {
                LinkedCrossPointer& p = ln.l_p;
                nlc.l_p = p;
                p.over = false;
                p.r = NewMemory;
            }

            if (lp_u) {
                LinkedCrossPointer& p = lp.u_n;
                nlc.l_n = p;
                p.over = false;
                p.r = NewMemory;
            }
            else {
                LinkedCrossPointer& p = lp.l_n;
                nlc.l_n = p;
                p.over = false;
                p.r = NewMemory;
            }
        }
    };
}