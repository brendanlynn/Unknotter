#include "linkedknot.h"

void Unknotter::LinkedCross::Add(LinkedCrossReference Upper, LinkedCrossReference Lower, LinkedCross* NewMemory) {
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