#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "State.h"
using namespace std;

class State;

class Transition {
private:
    string conditionStr;
    regex condition;
    State* nextState;

public:
    Transition(string in, State* nState);
    Transition(Transition* other, map<State*, State*>* stateGetter, int* counter);
    string getConditionStr();
    State* applyInput(string in);
};
