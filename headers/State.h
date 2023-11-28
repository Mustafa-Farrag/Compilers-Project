#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "Transition.h"
using namespace std;

class Transition;

class State {
private:
    int num;
    bool isStart;
    bool isAccept;
    vector<Transition> transitions;
    vector<Transition> epsilonTransitions;

public:
    State(int n);
    State(int n, bool start, bool accept);
    bool getIsStart();
    bool getIsAccept();
    vector<Transition> getTransitions();
    vector<Transition> getEpsilonTransitions();
    vector<State*> getEpsilonStates();
    void addTransition(Transition trans);
    vector<State*> applyInput(string in);
};
