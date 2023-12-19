#pragma once

#include <bits/stdc++.h>
#include "State.h"
using namespace std;

class NFA{
private:
    State* startState;
    State* acceptState;
    
public:
    NFA(string condition, int* counter);
    NFA(State* start, State* end);
    NFA(NFA* other, int* counter); 
    ~NFA();
    State* getStartState();
    State* getAcceptState();
    void setStartState(State* state);
    void setAcceptState(State* state);
};