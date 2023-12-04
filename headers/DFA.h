#pragma once

#include <bits/stdc++.h>
#include "DFAState.h"
using namespace std;

class DFA {
private:
    DFAState* startState; 

public:
    DFA();
    void setStartState(DFAState* start);
    DFAState* getStartState();
    void convertMap(const map<string, map<string, string>>& oldMap,
                    string startState, map<string, string> &classType, string phistate);
};