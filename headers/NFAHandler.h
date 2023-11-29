#ifndef NFAHANDLER_H
#define NFAHANDLER_H

#include <bits/stdc++.h>
#include "NFA.h"

using namespace std;

class NFAHandler {
public:
    NFAHandler();
    NFA* performUnion(NFA* nfa1, NFA* nfa2);
    NFA* performConcatination(NFA* nfa1, NFA* nfa2);
    NFA* performKleenClosure(NFA* nfa);
    NFA* performPositiveClosure(NFA* nfa);
    NFA* performUnionCombination(vector<NFA*> nfas);
    NFA* performConcatinationCombination(vector<NFA*> nfas);
};

#endif