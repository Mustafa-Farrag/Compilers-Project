#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "headers/Transition.h"
using namespace std;

class State {
private:
    int num;
    bool isStart;
    bool isAccept;
    vector<Transition> transitions;

public:
    State(int n);
    State(int n, bool start, bool accept);
    bool getIsStart();
    bool getIsAccept();
    vector<Transition> getTransitions();
    void addTransition(Transition trans);
    vector<State*> applyInput(string in);
};
