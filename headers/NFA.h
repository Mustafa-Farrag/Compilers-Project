#include <bits/stdc++.h>
#include "headers/State.h"
using namespace std;

class NFA{
private:
    State* startState;
    State* acceptState;
public:
    NFA(string condition);
    State* getStartState();
    State* getAcceptState();
};