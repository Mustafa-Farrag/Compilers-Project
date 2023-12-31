#pragma once

#include <bits/stdc++.h>
#include "State.h"
#include "NFAHandler.h"
using namespace std;

class DFAHandler {
private:
    NFAHandler* nfaHandler;
    map<int, State*> idStatesMap2;
    map<string, set<int>> nStates_To_NStates;
    map<string, map<string, string>> DFATransitionTable;
    map<string, string> acceptanceStateToClassType;
    string startState;
    string getConcatenatedString(set<int> states);
    set<string> getAllInputs(set<int> states);
    set<int> get_ids(vector<State*> nextStates);

public:
    DFAHandler(map<int, State*> idStatesMap);
    map<int, State*> getidStatesMap();
    map<string, set<int>> getnStates_To_NStates();
    map<string, map<string, string>> ConstructDFATransitionTable(map<int, map<string, vector<int>>>, State* startState);
    map<string, map<string, string>> getDFATransitionTable();
    map<string, string> getAcceptanceStateToClassType();
    string getStartState();
    void processDFATableInputs();
    ~DFAHandler();
};