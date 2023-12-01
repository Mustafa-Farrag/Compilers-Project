#include "headers/InputParser.h"
#include "headers/NFA.h"
#include "headers/State.h"
#include "headers/Transition.h"
#include "headers/NFAHandler.h"
#include "headers/DFAHandler.h"
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
    NFAHandler* nfahandler = new NFAHandler();
    // cout << "iam here" <<endl;
    map<int, map<string, vector<int>>> nfaTransitionTable = nfahandler->getTransitionTable(combinedNFA->getStartState());
    cout << nfaTransitionTable.empty() <<endl;

    map<string, vector<int>> inputTranstionPairs = nfaTransitionTable.at(combinedNFA->getStartState()->getNum());
    cout << inputTranstionPairs.empty() <<endl;


    //new for NFA_to_DFA task
    map<int, State*> idStatesMap = nfahandler->getIdStateMap(combinedNFA->getStartState());

    DFAHandler* dfahandler = new DFAHandler(idStatesMap);

    map<string, map<string, string>> dfaTransitionTable = dfahandler->getDFATransitionTable(nfaTransitionTable, combinedNFA->getStartState());
    cout << dfaTransitionTable.empty() <<endl;

    printf("iam here");

    return 0;
}