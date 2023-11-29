#ifndef NFAHANDLER_H
#define NFAHANDLER_H

#include <bits/stdc++.h>
#include "NFA.h"

using namespace std;

class NFAHandler {
private:
    int counter;
public:
    NFAHandler();
    NFA* createNFA(string condition);
    NFA* performUnion(NFA* nfa1, NFA* nfa2);
    NFA* performConcatination(NFA* nfa1, NFA* nfa2);
    NFA* performKleenClosure(NFA* nfa);
    NFA* performPositiveClosure(NFA* nfa);
    NFA* performUnionCombination(vector<NFA*> nfas, vector<string> types);
    NFA* performConcatinationCombination(vector<NFA*> nfas);
};

#endif