#include "DFA.h"
#include <bits/stdc++.h>
#include <vector>
#include <set>
#include "DFAHandler.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>


using namespace std;

class MinimizeDFA {
private:
    
    string startState;
    map<string, string> oldclassType;
    map<string, string> classType;
    map<string, map<string, string>> minimizedTable;

    DFA* constructDFA();
    map<string, int> separateNonAcceptAndAcceptStates(
        map<string, set<int>> nStates_To_NStates, 
        map<int, State*> getidStatesMap);

    map<string, int> getIndexOfGroupForMinimized(DFAHandler* dfahandler, set<string> transitions);
    vector<vector<string>> separateStatesIntoGroups(map<string, int> &groupIndex);
    vector<vector<string>> minimize(DFAHandler* dfahandler);
    void appendDataToFile(const map<string, map<string, string>>& data, const set<string>& horizontalHeaders, const string& filename) ;
    void printMinizedTableToFile(map<string, map<string, string>> minimizedTable, set<string> transitions);

public:
    DFA* constructMinimizedDFATable(DFAHandler* dfahandler);
    map<string, map<string, string>> getMinimizedTable();
    string getStartState();
    map<string, string> getClassType();
    bool isStartState(string old, string state);
    bool isAcceptanceState(string state);
};