#include <bits/stdc++.h>
#include "State.h"
using namespace std;

class NFA{
private:
    State* startState;
    State* acceptState;
public:
    NFA(string condition);
    ~NFA();
    State* getStartState();
    State* getAcceptState();
};