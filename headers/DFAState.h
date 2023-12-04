#pragma once

#include <bits/stdc++.h>
using namespace std;

// Class representing a state in the DFA
class DFAState {

private:
    string id;  
    bool isAccepting;
    string classType;
    unordered_map<string, DFAState*> transitions;

public:
    DFAState(string id, bool isAccepting, string classType);
    DFAState(string id);
    void setTransition(string i, DFAState* nextState);

    unordered_map<string, DFAState*> getTransitions();
    DFAState* getNextState(string input);
    string getId();
    bool getIsAccepting();
    string getClassType();
    void setId(string id);
    void setAccepting(bool isAccepting);
    void setClassType(string classType);
};