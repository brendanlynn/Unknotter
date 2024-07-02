#pragma once

namespace Unknotter {
    struct LinkedCross;
    struct LinkedCrossReference;

    struct LinkedCrossReference final {
        LinkedCross* r;
        bool over;
    };

    struct LinkedCross final {
        LinkedCrossReference l_p;
        LinkedCrossReference l_n;
        LinkedCrossReference u_p;
        LinkedCrossReference u_n;
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
        static __forceinline void GoToRef(LinkedCross*& Current, bool& Over, const LinkedCrossReference& Ref) {
            Current = Ref.r;
            Over = Ref.over;
        }
    public:
        static __forceinline void TravelN(LinkedCross*& Current, bool& Over) {
            if (Over) {
                GoToRef(Current, Over, Current->u_n);
            }
            else {
                GoToRef(Current, Over, Current->l_n);
            }
        }
        static __forceinline void TravelP(LinkedCross*& Current, bool& Over) {
            if (Over) {
                GoToRef(Current, Over, Current->u_p);
            }
            else {
                GoToRef(Current, Over, Current->l_p);
            }
        }
    };
}