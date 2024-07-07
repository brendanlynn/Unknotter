#include "linkedsimplifier.h"

#include <unordered_set>

struct LinkedChord final {
    Unknotter::LinkedCross* contact_start;
    Unknotter::LinkedCross* contact_end;
    bool over_start;
    bool over_end;

    __forceinline LinkedChord() = default;
    __forceinline LinkedChord(Unknotter::LinkedCross* Contact_Start, Unknotter::LinkedCross* Contact_End, bool Over_Start, bool Over_End)
        : contact_start(Contact_Start),
          contact_end(Contact_End),
          over_start(Over_Start),
          over_end(Over_End) { }
};

struct LinkedChordNoSecond final {
    Unknotter::LinkedCross* contact_start;
    Unknotter::LinkedCross* contact_end;
    bool over;

    __forceinline LinkedChordNoSecond() = default;
    __forceinline LinkedChordNoSecond(Unknotter::LinkedCross* Contact_Start, Unknotter::LinkedCross* Contact_End, bool Over)
        : contact_start(Contact_Start),
          contact_end(Contact_End),
          over(Over) { }
};

bool Unknotter::CanBeRemovedImmediately(LinkedCross* PrimaryStart, bool PrimaryStart_Over, LinkedCross* PrimaryEnd, bool SecondaryForward) {
    //gets crosses in primary band, excluding endpoints

    std::unordered_set<LinkedCross*> set_primary;
    LinkedCross* i_p = PrimaryStart;
    bool i_o = PrimaryStart_Over;
    while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryEnd) {
        set_primary.insert(i_p);
    }

    if (PrimaryStart_Over != i_o) {
        return false;
    }

    //gets crosses in secondary band, excluding endpoints
    //puts crosses in a combined set

    std::unordered_set<LinkedCross*> set_combined(set_primary);
    i_o = !i_o;
    if (SecondaryForward) {
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart) {
            set_combined.insert(i_p);
        }
    }
    else {
        while (LinkedCross::TravelP(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart) {
            set_combined.insert(i_p);
        }
    }

    //gets chords of the area

    std::vector<LinkedChord> chords;
    i_p = PrimaryEnd;
    i_o = PrimaryStart_Over;
    {
        bool inside = false;
        bool lastOver;
        bool lastPrimary;
        LinkedCross* lastContactPoint = 0;
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryEnd) {
            if (set_combined.contains(i_p)) {
                bool over = i_o;
                bool primary = set_primary.contains(i_p);
                LinkedCross* contactPoint = i_p;
                if (inside) {
                    if (lastOver != over && (lastPrimary == primary || over ^ primary ^ PrimaryStart_Over)) {
                        return false;
                    }
                    LinkedChord lc(lastContactPoint, contactPoint, lastOver, over);
                    chords.push_back(lc);
                    inside = false;
                }
                else {
                    lastOver = over;
                    lastPrimary = primary;
                    lastContactPoint = contactPoint;
                    inside = true;
                }
            }
        }
    }
    
    //distributes points of chords into sets by type of chord

    std::unordered_set<LinkedCross*> setUL;
    std::unordered_set<LinkedCross*> setLL;

    for (LinkedChord chord : chords) {
        i_p = chord.contact_start;
        if (chord.over_start) {
            if (chord.over_end) {
                continue;
            }
            i_o = true;
            while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                setUL.insert(i_p);
            }
        }
        else {
            i_o = false;
            if (chord.over_end) {
                while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                    setUL.insert(i_p);
                }
            }
            else {
                while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                    setLL.insert(i_p);
                }
            }
        }
    }

    //detects any chords that would prevent the loop from being pulled out

    for (LinkedChord chord : chords) {
        i_p = chord.contact_start;
        i_o = chord.over_start;
        int_fast32_t t;
        if (chord.over_start) {
            if (chord.over_end) {
                t = 0;
            }
            else {
                t = 1;
            }
        }
        else {
            if (chord.over_end) {
                t = 1;
            }
            else {
                t = 2;
            }
        }
        switch (t) {
        case 0:
            while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                if (i_o) continue;
                if (setUL.contains(i_p) || setLL.contains(i_p)) {
                    return false;
                }
            }
            break;
        case 1:
            while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                if (i_o) continue;
                if (setLL.contains(i_p)) {
                    return false;
                }
            }
        }
    }

    return true;
}
bool Unknotter::CanBeRemovedImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartStartAbove) {
    //gets crosses in primary band, excluding endpoints

    std::unordered_set<LinkedCross*> set_primary;
    LinkedCross* i_p = PrimaryStartEnd;
    bool i_o = PrimaryStartStartAbove;
    while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStartEnd) {
        set_primary.insert(i_p);
    }

    //gets chords of the area

    std::vector<LinkedChordNoSecond> chords;
    i_p = PrimaryStartEnd;
    i_o = !PrimaryStartStartAbove;
    {
        bool inside = false;
        bool lastOver;
        LinkedCross* lastContactPoint = 0;
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStartEnd) {
            if (set_primary.contains(i_p)) {
                bool over = i_o;
                LinkedCross* contactPoint = i_p;
                if (inside) {
                    if (lastOver != over) {
                        return false;
                    }
                    LinkedChordNoSecond lc(lastContactPoint, contactPoint, over);
                    chords.push_back(lc);
                    inside = false;
                }
                else {
                    lastOver = over;
                    lastContactPoint = contactPoint;
                    inside = true;
                }
            }
        }
    }

    //distributes points of chords into sets by type of chord

    std::unordered_set<LinkedCross*> setLL;

    for (LinkedChordNoSecond chord : chords) {
        if (chord.over) {
            continue;
        }
        i_p = chord.contact_start;
        i_o = false;
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
            setLL.insert(i_p);
        }
    }

    //detects any chords that would prevent the loop from being pulled out

    for (LinkedChordNoSecond chord : chords) {
        if (!chord.over) {
            continue;
        }
        i_p = chord.contact_start;
        i_o = true;
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
            if (i_o) continue;
            if (setLL.contains(i_p)) {
                return false;
            }
        }
    }

    return true;
}