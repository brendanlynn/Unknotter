#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include "linkedknot.h"
#include "linkedsimplifier.h"

using std::cout;
using std::cin;
using std::getchar;
using std::mt19937_64;
using std::stringstream;
using std::pair;
using std::vector;
using std::string;
using std::getline;
using std::istringstream;
using std::unordered_set;
using std::numeric_limits;

using Unknotter::LinkedCross;
using Unknotter::TryToRemoveImmediately;
using Unknotter::CanBeRemovedImmediately;
using Unknotter::IterateRemovalAttempts;

using iPair_t = pair<size_t, size_t>;

#define CreateKnotFromPairs LinkedCross::CreateKnotFromPairs
#define KnotAdd LinkedCross::Add
#define KnotRemoveRange LinkedCross::RemoveRange
#define KnotGetVector LinkedCross::CompileAll_Vector
#define KnotGetSet LinkedCross::CompileAll_Set
#define CrossTravelN LinkedCross::TravelN
#define CrossTravelP LinkedCross::TravelP

uint64_t getMills() {
    return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
    cout << "Welcome to \033[33mUnknotter\033[0m!\n";
    cout << "by \033[32mBrendan Lynn\033[0m.\n\n";
    cout << "This program was designed by computer scientist Brendan Lynn as a algorithm to detect whether or not a given mathematical knot is the unknot. Bear in mind that this algorithm is of an evidence-supported but nonetheless unproven hypothesis.\n\n";
    cout << "Knots must be specified in a base-0, absolute, paired version of Dowker-Thistlethwaite notation, with the index crossing over on the left and the index crossing under on the right. To indicate the end of the set, type \"end\", case-sensitive, exactally like that. For instance, the trefoil knot could be notated as such:\n\n";
    cout << "1: 0 3\n";
    cout << "2: 4 1\n";
    cout << "3: 2 5\n";
    cout << "4: end\n\n";
    cout << "I hope this tool is made good use of. Have fun!\n";
ContinueOuter:
    while (true) {
        cout << "--------------------------------\n";
        cout << "Please enter knot notation:\n\n";
        size_t i = 1;
        vector<iPair_t> pairs;
        do {
            cout << i << ": ";

            string dv;
            getline(cin, dv);
            if (dv == "end") break;

            size_t a;
            size_t b;
            istringstream iss(dv);
            if (iss >> a >> b && iss.eof()) {
                pairs.push_back(iPair_t(a, b));
            }
            else {
                cout << "\nInvalid format.\n";
                goto ContinueOuter;
            }
        } while (++i, true);

        size_t doubleSize = pairs.size() << 1;
        unordered_set<size_t> indicies;
        for (const auto& pair : pairs) {
            if (pair.first >= doubleSize) {
                cout << "\nIndex " << pair.first << " is greater than or equal to the quantity of indicies. This is an impossibility. The list is therefore invalid.\n";
                goto ContinueOuter;
            }
            if (pair.second >= doubleSize) {
                cout << "\nIndex " << pair.second << " is greater than or equal to the quantity of indicies. This is an impossibility. The list is therefore invalid.\n";
                goto ContinueOuter;
            }
            if (!indicies.insert(pair.first).second) {
                cout << "\nIndex " << pair.first << " is present more than once in the pairings. This is an impossibility. The list is therefore invalid.\n";
                goto ContinueOuter;
            }
            if (!indicies.insert(pair.second).second) {
                cout << "\nIndex " << pair.second << " is present more than once in the pairings. This is an impossiblity. The list is therefore invalid.\n";
                goto ContinueOuter;
            }
        }

        i = 1;
        cout << "\nThe following notation was recieved:\n\n";
        if (pairs.size()) {
            for (const auto& pair : pairs) {
                cout << i++ << ": (" << pair.first << ", " << pair.second << ")\n";
            }
        }
        else {
            cout << "No data.\n";
        }

        cout << "\nPlease verify this notation before continuing. Computation may take time.\n";
        cout << "Is this notation correct? (y/n): ";
        string correct;
        getline(cin, correct);
        if (correct != "y" && correct != "n") {
            cout << "Invalid format.\n";
            continue;
        }
        if (correct == "n") {
            cout << "Please re-enter notation.\n";
            continue;
        }

        cout << "\nProceeding with computation...";
        bool notUnknot;
        if (pairs.size()) {
            const auto* p_startCrosses = CreateKnotFromPairs(pairs.data(), pairs.size());
            const auto& startCrosses = *p_startCrosses;
            unordered_set<LinkedCross*> allCrosses(startCrosses.begin(), startCrosses.size() ? startCrosses.end() : startCrosses.begin());
            IterateRemovalAttempts(allCrosses);

            notUnknot = (bool)allCrosses.size();

            LinkedCross::DisposeAll(allCrosses);
            delete p_startCrosses;
        }
        else {
            notUnknot = false;
        }
        if (notUnknot) {
            cout << "\n\nTHE RESULT: Is the knot the unknot?: ->\033[31mNO\033[0m<-\n";
        }
        else {
            cout << "\n\nTHE RESULT: Is the knot the unknot?: ->\033[32mYES\033[0m<-\n";
        }
    }
}