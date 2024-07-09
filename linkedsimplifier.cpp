#include "linkedsimplifier.h"

#include <unordered_set>
#include <unordered_map>

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

bool Unknotter::CanBeRemovedImmediately(LinkedCross* PrimaryStart, bool PrimaryUpper, LinkedCross* PrimaryEnd, bool SecondaryForward) {
    if (PrimaryStart == PrimaryEnd) {
        return CanBeRemovedImmediately(PrimaryStart, PrimaryUpper);
    }
    
    //gets crosses in primary band, excluding endpoints

    std::unordered_set<LinkedCross*> set_primary;
    LinkedCross* i_p = PrimaryStart;
    bool i_o = PrimaryUpper;
    while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryEnd) {
        if (set_primary.insert(i_p).second) continue;
        return false;
    }

    if (PrimaryUpper != i_o) {
        return false;
    }

    //gets crosses in secondary band, excluding endpoints
    //puts crosses in a combined set

    std::unordered_set<LinkedCross*> set_combined(set_primary);
    i_o = !i_o;
    if (SecondaryForward) {
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart) {
            if (set_combined.insert(i_p).second) continue;
            return false;
        }
    }
    else {
        while (LinkedCross::TravelP(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart) {
            if (set_combined.insert(i_p).second) continue;
            return false;
        }
    }

    //gets chords of the area

    std::vector<LinkedChord> chords;
    i_p = PrimaryEnd;
    i_o = PrimaryUpper;
    {
        bool inside = false;
        bool lastOver;
        bool lastPrimary;
        LinkedCross* lastContactPoint = 0;
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart && i_p != PrimaryEnd) {
            if (set_combined.contains(i_p)) {
                bool over = i_o;
                bool primary = set_primary.contains(i_p);
                LinkedCross* contactPoint = i_p;
                if (inside) {
                    if (lastOver != over && (lastPrimary == primary || over ^ primary ^ PrimaryUpper)) {
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
    i_p = PrimaryStart;
    i_o = PrimaryUpper;
    {
        bool inside = false;
        bool lastOver;
        bool lastPrimary;
        LinkedCross* lastContactPoint = 0;
        while (LinkedCross::TravelP(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart && i_p != PrimaryEnd) {
            if (set_combined.contains(i_p)) {
                bool over = i_o;
                bool primary = set_primary.contains(i_p);
                LinkedCross* contactPoint = i_p;
                if (inside) {
                    if (lastOver != over && (lastPrimary == primary || over ^ primary ^ PrimaryUpper)) {
                        return false;
                    }
                    LinkedChord lc(contactPoint, lastContactPoint, over, lastOver);
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
    
    //distributes points of chords into sets by type of chord,
    //and returns false if a higher type crosses below a lower type

    std::unordered_set<LinkedCross*> setU;

    std::unordered_set<LinkedCross*> set1_L;
    for (LinkedChord chord : chords) {
        if (chord.over_start && chord.over_end) {
            i_p = chord.contact_start;
            i_o = true;
            while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                if (i_o) {
                    setU.insert(i_p);
                }
                else {
                    set1_L.insert(i_p);
                }
            }
        }
    }
    for (LinkedCross* v : set1_L) {
        if (!setU.contains(v)) {
            return false;
        }
    }

    std::unordered_set<LinkedCross*> set2_L;
    for (LinkedChord chord : chords) {
        if (chord.over_start != chord.over_end) {
            i_p = chord.contact_start;
            i_o = chord.over_start;
            while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                if (i_o) {
                    setU.insert(i_p);
                }
                else {
                    set2_L.insert(i_p);
                }
            }
        }
    }
    for (LinkedCross* v : set2_L) {
        if (!setU.contains(v)) {
            return false;
        }
    }

    return true;
}
bool Unknotter::CanBeRemovedImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartUpper) {
    //gets crosses in primary band, excluding endpoints

    std::unordered_set<LinkedCross*> set_primary;
    LinkedCross* i_p = PrimaryStartEnd;
    bool i_o = PrimaryStartUpper;
    while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStartEnd) {
        if (set_primary.insert(i_p).second) continue;
        return false;
    }

    //gets chords of the area

    std::vector<LinkedChordNoSecond> chords;
    i_p = PrimaryStartEnd;
    i_o = !PrimaryStartUpper;
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

    //distributes points of chords into sets by type of chord,
    //and returns false if a higher type crosses below a lower type

    std::unordered_set<LinkedCross*> setU;
    std::unordered_set<LinkedCross*> set1_L;

    for (LinkedChordNoSecond chord : chords) {
        if (chord.over) {
            i_p = chord.contact_start;
            i_o = true;
            while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != chord.contact_end) {
                if (i_o) {
                    setU.insert(i_p);
                }
                else {
                    set1_L.insert(i_p);
                }
            }
        }
    }
    for (LinkedCross* v : set1_L) {
        if (!setU.contains(v)) {
            return false;
        }
    }

    return true;
}

bool Unknotter::TryToRemoveImmediately(LinkedCross* PrimaryStart, bool PrimaryUpper, LinkedCross* PrimaryEnd, bool SecondaryForward, std::unordered_set<LinkedCross*>* AllCrosses) {
    if (PrimaryStart == PrimaryEnd) {
        return TryToRemoveImmediately(PrimaryStart, PrimaryUpper, AllCrosses);
    }

    if (!CanBeRemovedImmediately(PrimaryStart, PrimaryUpper, PrimaryEnd, SecondaryForward)) {
        return false;
    }

    LinkedCross* i_p;
    bool i_o;

    std::unordered_map<LinkedCross*, bool> escapeDirections;
    i_p = PrimaryStart;
    i_o = PrimaryUpper;
    while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryEnd) {
        escapeDirections.insert(std::pair<LinkedCross*, bool>(i_p, false));
    }


    i_p = PrimaryEnd;
    i_o = PrimaryUpper;
    {
        bool inside = false;
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart && i_p != PrimaryEnd) {
            if (escapeDirections.contains(i_p)) {
                escapeDirections[i_p] = inside;
            }
        }
    }
    i_p = PrimaryStart;
    i_o = PrimaryUpper;
    {
        bool inside = false;
        while (LinkedCross::TravelP(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryStart && i_p != PrimaryEnd) {
            if (escapeDirections.contains(i_p)) {
                escapeDirections[i_p] = !inside;
            }
        }
    }

    i_p = PrimaryStart;
    i_o = PrimaryUpper;
    LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o);
    LinkedCross::RemoveRange(LinkedCrossPointer(i_p, i_o), PrimaryEnd, AllCrosses);

    i_p = PrimaryStart;
    i_o = PrimaryUpper;
    LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o);
    i_o = !i_o;
    LinkedCrossReference startRef(PrimaryStart, !PrimaryUpper);
    if (SecondaryForward) {
        while (LinkedCross::TravelN(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryEnd) {
            bool afterParallel = escapeDirections[i_p];
            LinkedCrossReference parallelRef(i_p, !i_o);
            if (afterParallel) {
                LinkedCross* lc = LinkedCross::Add<true, true>(startRef, parallelRef);
                if (AllCrosses) AllCrosses->insert(lc);
            }
            else {
                LinkedCross* lc = LinkedCross::Add<true, false>(startRef, parallelRef);
                if (AllCrosses) AllCrosses->insert(lc);
            }
        }
    }
    else {
        while (LinkedCross::TravelP(*(const LinkedCross**)&i_p, i_o), i_p != PrimaryEnd) {
            bool afterParallel = escapeDirections[i_p];
            LinkedCrossReference parallelRef(i_p, !i_o);
            if (afterParallel) {
                LinkedCross* lc = LinkedCross::Add<true, true>(startRef, parallelRef);
                if (AllCrosses) AllCrosses->insert(lc);
            }
            else {
                LinkedCross* lc = LinkedCross::Add<true, false>(startRef, parallelRef);
                if (AllCrosses) AllCrosses->insert(lc);
            }
        }
    }

    return true;
}
bool Unknotter::TryToRemoveImmediately(LinkedCross* PrimaryStartEnd, bool PrimaryStartUpper, std::unordered_set<LinkedCross*>* AllCrosses) {
    if (!CanBeRemovedImmediately(PrimaryStartEnd, PrimaryStartUpper)) {
        return false;
    }
    LinkedCross::RemoveRange(LinkedCrossPointer(PrimaryStartEnd, PrimaryStartUpper), PrimaryStartEnd, AllCrosses);
    return true;
}

size_t Unknotter::IterateRandomRemovalAttempts(LinkedCross* Sample, std::mt19937_64& RNG, uint64_t Iterations) {
    std::unordered_set<LinkedCross*>* crosses_set = LinkedCross::CompileAll_Set(Sample);
    std::vector<LinkedCross*> crosses_vec;
    std::uniform_int_distribution<size_t> dis;
    auto refreshInfo = [&]() {
        crosses_vec.clear();
        for (LinkedCross* element : *crosses_set) crosses_vec.push_back(element);
        dis = std::uniform_int_distribution<size_t>(0, crosses_vec.size() - 1);
    };
    auto getCross = [&]() {
        return crosses_vec[dis(RNG)];
    };
    refreshInfo();

    uint64_t r64;
    uint32_t r64Pos = 0;
    auto getBool = [&]() {
        if (!r64Pos) {
            r64 = RNG();
            r64Pos = 1;
            return (bool)(r64Pos & 1);
        }
        else {
            bool b = r64Pos & r64;
            r64Pos <<= 1;
            return b;
        }
    };

    uint64_t totalSuccesses = 0;
    for (uint64_t i = 0; i < Iterations; ++i) {
        if (TryToRemoveImmediately(getCross(), getBool(), getCross(), getBool(), crosses_set)) {
            ++totalSuccesses;
            if (!crosses_set->size()) break;
            refreshInfo();
        }
    }

    delete crosses_set;

    return totalSuccesses;
}

size_t Unknotter::IterateRandomRemovalAttempts(std::unordered_set<LinkedCross*>& AllCrosses, std::mt19937_64& RNG, uint64_t Iterations) {
    std::vector<LinkedCross*> crosses_vec;
    std::uniform_int_distribution<size_t> dis;
    auto refreshInfo = [&]() {
        crosses_vec.clear();
        for (LinkedCross* element : AllCrosses) crosses_vec.push_back(element);
        dis = std::uniform_int_distribution<size_t>(0, crosses_vec.size() - 1);
    };
    auto getCross = [&]() {
        return crosses_vec[dis(RNG)];
    };
    refreshInfo();

    uint64_t r64;
    uint32_t r64Pos = 0;
    auto getBool = [&]() {
        if (!r64Pos) {
            r64 = RNG();
            r64Pos = 1;
            return (bool)(r64Pos & 1);
        }
        else {
            bool b = r64Pos & r64;
            r64Pos <<= 1;
            return b;
        }
    };

    uint64_t totalSuccesses = 0;
    for (uint64_t i = 0; i < Iterations; ++i) {
        if (TryToRemoveImmediately(getCross(), getBool(), getCross(), getBool(), &AllCrosses)) {
            ++totalSuccesses;
            if (!AllCrosses.size()) break;
            refreshInfo();
        }
    }

    return totalSuccesses;
}