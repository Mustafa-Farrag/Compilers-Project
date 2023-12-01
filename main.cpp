#include "headers/InputParser.h"
#include "headers/NFA.h"
#include "headers/State.h"
#include "headers/Transition.h"
#include "headers/NFAHandler.h"
#include "headers/ExpressionEvaluator.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    InputParser ip("input.txt");
    map<string, string> rd = ip.getRD();
    map<string, string> re = ip.getRE();
    set<string> keywords = ip.getKeywords();
    set<string> puncts = ip.getPuncts();

    ExpressionEvaluator* ee = new ExpressionEvaluator();
    NFA* combinedNFA = ee->computeCombinedNFA(rd, re, keywords, puncts);

    //new for NFA_to_DFA task
    NFAHandler* handler = new NFAHandler();
    map<int, map<string, vector<int>>> nfaTransitionTable = handler->getTransitionTable(combinedNFA->getStartState());
    cout << nfaTransitionTable.empty() <<endl;

    map<string, vector<int>> inputTranstionPairs = nfaTransitionTable.at(combinedNFA->getStartState()->getNum());
    cout << inputTranstionPairs.empty() <<endl;

    

    printf("iam here");

    return 0;
}