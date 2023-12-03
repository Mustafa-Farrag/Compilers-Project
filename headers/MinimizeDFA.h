#include <bits/stdc++.h>
#include <vector>
#include <set>
#include "DFAHandler.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>


using namespace std;

class MinimizeDFA {
private:
    map<string, int> separateNonAcceptAndAcceptStates(
        map<string, set<int>> nStates_To_NStates, 
        map<int, State*> getidStatesMap);

    map<string, int> getIndexOfGroupForMinimized(DFAHandler* dfahandler, set<string> transitions);
    vector<vector<string>> separateStatesIntoGroups(map<string, int> &groupIndex);
    vector<vector<string>> minimize(DFAHandler* dfahandler);
    void appendDataToFile(const map<string, map<string, string>>& data, const set<string>& horizontalHeaders, const string& filename) ;
    void printMinizedTableToFile(map<string, map<string, string>> minimizedTable, set<string> transitions);

public:
    void constructMinimizedDFATable(DFAHandler* dfahandler);
};