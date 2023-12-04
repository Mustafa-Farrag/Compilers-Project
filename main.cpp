#include "headers/InputParser.h"
#include "headers/NFA.h"
#include "headers/State.h"
#include "headers/Transition.h"
#include "headers/NFAHandler.h"
#include "headers/DFAHandler.h"
#include "headers/ExpressionEvaluator.h"
#include "headers/MinimizeDFA.h"
#include "headers/Analyzer.h"
#include <bits/stdc++.h>

using namespace std;
set<string>  set2;

void printClassTypeHelper(DFAState* state, set<string> &set) {
    if(set2.find(state->getId()) != set2.end()) return;
    set.insert(state->getClassType());
    set2.insert(state->getId());
    // Recursively iterate over transitions
    for (const auto& transition : state->getTransitions()) {
        printClassTypeHelper(transition.second, set);
    }
    return ;
}

int main(){
    // parse input file
    InputParser ip("input.txt");
    map<string, string> rd = ip.getRD();
    map<string, string> re = ip.getRE();
    set<string> keywords = ip.getKeywords();
    set<string> puncts = ip.getPuncts();

    // construct combined NFA
    ExpressionEvaluator* ee = new ExpressionEvaluator();
    NFA* combinedNFA = ee->computeCombinedNFA(rd, re, keywords, puncts);

    // construct NFA maps
    NFAHandler* nfahandler = new NFAHandler();
    map<int, map<string, vector<int>>> nfaTransitionTable = 
                                    nfahandler->getTransitionTable(combinedNFA->getStartState());
    cout << "NFA transition Table size: " << nfaTransitionTable.size()  <<endl;
    map<string, vector<int>> inputTranstionPairs = nfaTransitionTable.at(combinedNFA->getStartState()->getNum());
    map<int, State*> idStatesMap = nfahandler->getIdStateMap(combinedNFA->getStartState());

    // convert NFA to DFA
    DFAHandler* dfahandler = new DFAHandler(idStatesMap);

    map<string, map<string, string>> dfaTransitionTable = dfahandler->ConstructDFATransitionTable(nfaTransitionTable, combinedNFA->getStartState());
    cout << "DFA transition Table size: " << dfaTransitionTable.size() <<endl;

    // minimize DFA
    // MinimizeDFA* minimizeDFA = new MinimizeDFA();
    
    // DFA* dfa = minimizeDFA->constructMinimizedDFATable(dfahandler);

    // set<string> set1;

    // printClassTypeHelper(dfa->getStartState(), set1);

    // Analyzer* analyzer = new Analyzer(dfa);

    // analyzer->analyze("test.txt");
    
    return 0;
}