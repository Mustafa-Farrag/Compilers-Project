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
    string classType;
    vector<Transition*> transitions;
    vector<Transition*> epsilonTransitions;

public:
    State(int n);
    State(int n, bool start, bool accept);
    State(State* other, map<State*, State*>* stateGetter, int* counter);
    bool getIsStart();
    bool getIsAccept();
    int getNum();
    void setIsStart(bool state);
    void setIsAccept(bool state);
    void setClassType(string type);
    string getClassType();
    vector<Transition*> getTransitions();
    vector<Transition*> getEpsilonTransitions();
    vector<State*> getEpsilonStates();
    // vector<State*> State::getNonEpsilonStates();
    void addTransition(Transition* trans);
    vector<State*> applyInput(string in);
};
