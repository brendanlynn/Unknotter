#include "linkedknot.h"

Unknotter::LinkedCross* Unknotter::LinkedCross::Add(LinkedCrossReference Upper, LinkedCrossReference Lower) {
    auto* ptr = new LinkedCross;

    LinkedCross& nlc = *ptr;

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
        p.r = ptr;
    }
    else {
        LinkedCrossPointer& p = un.l_p;
        nlc.u_p = p;
        p.over = true;
        p.r = ptr;
    }

    if (up_u) {
        LinkedCrossPointer& p = up.u_n;
        nlc.u_n = p;
        p.over = true;
        p.r = ptr;
    }
    else {
        LinkedCrossPointer& p = up.l_n;
        nlc.u_n = p;
        p.over = true;
        p.r = ptr;
    }

    if (ln_u) {
        LinkedCrossPointer& p = ln.u_p;
        nlc.l_p = p;
        p.over = false;
        p.r = ptr;
    }
    else {
        LinkedCrossPointer& p = ln.l_p;
        nlc.l_p = p;
        p.over = false;
        p.r = ptr;
    }

    if (lp_u) {
        LinkedCrossPointer& p = lp.u_n;
        nlc.l_n = p;
        p.over = false;
        p.r = ptr;
    }
    else {
        LinkedCrossPointer& p = lp.l_n;
        nlc.l_n = p;
        p.over = false;
        p.r = ptr;
    }
    return ptr;
}

template <bool _AfterUpper, bool _AfterLower>
Unknotter::LinkedCross* Unknotter::LinkedCross::Add(LinkedCrossReference Upper, LinkedCrossReference Lower) {
    LinkedCrossReference* p_upper;
    LinkedCrossReference* p_lower;
    
    if constexpr (_AfterUpper) {
        LinkedCross* u_p = &Upper.r;
        bool u_o = Upper.over;

        TravelN(*(const LinkedCross**)&u_p, u_o);

        p_upper = new LinkedCrossReference(u_p, u_o);
    }
    else {
        p_upper = new LinkedCrossReference(Upper);
    }
    if constexpr (_AfterLower) {
        LinkedCross* l_p = &Upper.r;
        bool l_o = Upper.over;

        TravelN(*(const LinkedCross**)&l_p, l_o);

        p_lower = new LinkedCrossReference(l_p, l_o);
    }
    else {
        p_lower = new LinkedCrossReference(Lower);
    }

    auto* r = Add(*p_upper, *p_lower);

    delete p_upper;
    delete p_lower;

    return r;
}

template Unknotter::LinkedCross* Unknotter::LinkedCross::Add<true, true>(LinkedCrossReference, LinkedCrossReference);
template Unknotter::LinkedCross* Unknotter::LinkedCross::Add<true, false>(LinkedCrossReference, LinkedCrossReference);
template Unknotter::LinkedCross* Unknotter::LinkedCross::Add<false, true>(LinkedCrossReference, LinkedCrossReference);
template Unknotter::LinkedCross* Unknotter::LinkedCross::Add<false, false>(LinkedCrossReference, LinkedCrossReference);

void Unknotter::LinkedCross::Remove() {
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

void Unknotter::LinkedCross::RemoveRange(LinkedCrossPointer Lower, LinkedCross* Upper) {
    LinkedCross*& p = Lower.r;
    bool& over = Lower.over;
    do {
        LinkedCross* cp = p;
        TravelN(*(const LinkedCross**)&p, over);
        cp->Remove();
        delete cp;
    } while (p != Upper);
}

std::unordered_set<Unknotter::LinkedCross*>* Unknotter::LinkedCross::CompileAll_Set(LinkedCross* Sample) {
    auto* p_set = new std::unordered_set<LinkedCross*>;
    auto& set = *p_set;
    LinkedCross* p = Sample;
    bool b = false;
    while (TravelN(*(const LinkedCross**)&p, b), p != Sample) {
        set.insert(p);
    }
    b = true;
    while (TravelN(*(const LinkedCross**)&p, b), p != Sample) {
        set.insert(p);
    }
    return p_set;
}

std::vector<Unknotter::LinkedCross*>* Unknotter::LinkedCross::CompileAll_Vector(LinkedCross* Sample) {
    auto* p_set = new std::vector<LinkedCross*>;
    auto& set = *p_set;
    LinkedCross* p = Sample;
    bool b = false;
    while (TravelN(*(const LinkedCross**)&p, b), p != Sample) {
        set.push_back(p);
    }
    b = true;
    while (TravelN(*(const LinkedCross**)&p, b), p != Sample) {
        set.push_back(p);
    }
    return p_set;
}