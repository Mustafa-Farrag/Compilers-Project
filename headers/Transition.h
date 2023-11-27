#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "State.h"
using namespace std;

class State;

class Transition {
private:
    regex condition;
    State* nextState;

public:
    Transition(string in, State* nState);
    State* applyInput(string in);
};
