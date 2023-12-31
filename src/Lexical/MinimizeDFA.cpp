#include "Lexical/MinimizeDFA.h"
#include "Lexical/DFAHandler.h"
#include <iostream>
#include <filesystem>
using namespace std;

/*
 * Separates acceptance states from non-acceptance states as 
 * a first step towards minimization.
 */
map<string, int> MinimizeDFA:: separateNonAcceptAndAcceptStates(
    map<string, set<int>> nStates_To_NStates, 
    map<int, State*> getidStatesMap)
    {
    map<string, int> index;
    map<string, int> acceptTypeIndex;
    int currentIndex = 1;
    for (auto it = nStates_To_NStates.begin(); it != nStates_To_NStates.end(); ++it) {
        string state = it->first;
        if(isAcceptanceState(state)){
            auto existingIndex = acceptTypeIndex.find(oldclassType[state]);
            if(existingIndex != acceptTypeIndex.end()){
                index[state] = existingIndex->second;
                acceptTypeIndex[oldclassType[state]] = existingIndex->second;
            }else{
                index[state] = currentIndex;
                acceptTypeIndex[oldclassType[state]] = currentIndex;
                currentIndex++;
            }
        }else{
            index[state] = 0;
        }
    }
    return index;
}
set<string> formTransitions(map<string, map<string, string>> &DFATransitionTable){
    set<string> transitions;
    for (const auto& outerPair : DFATransitionTable) {
        const auto& innerMap = outerPair.second;
        for (const auto& innermap : innerMap) {
            transitions.insert(innermap.first);
        }
    }
    return transitions;
}

map<string, int> MinimizeDFA::getIndexOfGroupForMinimized(DFAHandler* dfahandler, set<string> transitions){
    map<string, map<string, string>> DFATransitionTable = dfahandler->getDFATransitionTable();
    map<string, int> stateIndex = separateNonAcceptAndAcceptStates
    (
        dfahandler->getnStates_To_NStates(), 
        dfahandler->getidStatesMap()
    );
    
    string phiState = "phi";
    // stateIndex[phiState] = 0;

    int numOfGroups = 2;

    while(true){
        set<int> set;
        map<string, int> newIndex;
        map<string, int> GroupIndex;
        int currentIndex = 0;

         for (auto it = stateIndex.begin(); it != stateIndex.end(); ++it) {
            string state = it->first;
            string group = to_string(it->second);
            group += ",";
            map<string, string> stateTransition = DFATransitionTable[state];
            for (const auto& transition : transitions) {
                auto nextState = stateTransition.find(transition);
                if (nextState != stateTransition.end()) {
                    group += to_string(stateIndex[nextState->second]);
                }else{
                    group += phiState;
                }
                group += ",";
            }
            auto newGroupIndex = GroupIndex.find(group);
            if(newGroupIndex != GroupIndex.end()){
                newIndex[state] = newGroupIndex->second;
            }else{
                int newGroupIndex = currentIndex++;
                set.insert(newGroupIndex);
                GroupIndex[group] = newGroupIndex;
                newIndex[state] = newGroupIndex;
            }
        }
        if(numOfGroups == set.size()) break;
        numOfGroups = set.size();
        stateIndex = newIndex;
    }
    return stateIndex;
}

void MinimizeDFA::appendDataToFile(const map<string, map<string, string>>& data, const set<string>& horizontalHeaders, const string& filename) {

    ofstream outputFile(filename, ios::app);

    if (!outputFile.is_open()) {
        cerr << "Unable to open file!" << endl;
        return;
    }

    const int columnWidth = 35; 

    outputFile << setw(columnWidth / 2) << "" << setw(columnWidth - columnWidth / 2) << "";

    for (const auto& header : horizontalHeaders) {
        outputFile << setw(columnWidth) << left << header;
    }
    outputFile << "\n";

    for (const auto& row : data) {
        outputFile << setw(columnWidth / 2) << left << row.first << setw(columnWidth - columnWidth / 2) << "";

        for (const auto& header : horizontalHeaders) {
            auto iter = row.second.find(header);
            if (iter != row.second.end()) {
                outputFile << setw(columnWidth) << left << iter->second;
            } else {
                outputFile << setw(columnWidth) << left << ""; 
            }
        }
        outputFile << "\n";
    }
    outputFile << "\n";
    outputFile.close();
}

void MinimizeDFA::printMinizedTableToFile(map<string, map<string, string>> minimizedTable, set<string> transitions){
    string filename = "Files/MinimizedDFA.txt";
   
    size_t halfSize = transitions.size() / 2;
    auto it = transitions.begin();

    std::advance(it, halfSize);

    std::set<std::string> firstHalf(transitions.begin(), it);
    std::set<std::string> secondHalf(it, transitions.end());

    if (filesystem::exists(filename)) {
        filesystem::remove(filename);
    }

    appendDataToFile(minimizedTable, firstHalf, filename);
    appendDataToFile(minimizedTable, secondHalf, filename);
}

bool MinimizeDFA:: isStartState(string oldStartState, string state){
   return oldStartState == state;
}

bool MinimizeDFA:: isAcceptanceState(string state){
    if(oldclassType.find(state) != oldclassType.end()){
        return true;
    }
    return false;
}

DFA* MinimizeDFA::constructDFA(){
    DFA* dfa = new DFA();
    dfa->convertMap(minimizedTable, startState, classType, phiState);
    return dfa;
}

DFA* MinimizeDFA::constructMinimizedDFATable(DFAHandler* dfahandler){
    string oldStartState = dfahandler->getStartState();
    oldclassType = dfahandler->getAcceptanceStateToClassType();

    map<string, map<string, string>> DFATransitionTable = dfahandler->getDFATransitionTable();
    set<string> transitions = formTransitions(DFATransitionTable);
    map<string, int> groupIndex = getIndexOfGroupForMinimized(dfahandler, transitions);
    map<string, map<string, string>> table;
    
    
    // phiState = to_string(groupIndex["phi"]);

    for (const auto pair : groupIndex) {
        string index = to_string(pair.second);
        string state = pair.first;
        auto entry = table.find(index);
        
        
        if(entry == table.end()){
            map<string, string> nextStates;
            map<string, string> stateTransition = DFATransitionTable[state];
            for (const auto& transition : transitions) {
                auto nextState = stateTransition.find(transition);
                if (nextState != stateTransition.end()) {
                    string nextstate = nextState->second;
                    nextStates[transition] = to_string(groupIndex[nextstate]);
                }
                // else{
                //     nextStates[transition] = to_string(groupIndex["phi"]);
                // }
            }
            table[index] = nextStates;
        }

        if(state == ""){
            phiState = index;
        }

        if(isStartState(oldStartState, state)){
            startState = index;
        }
        if(isAcceptanceState(state)){
            classType[index] = oldclassType[state];
        }
    }
    minimizedTable = table;
    
    printMinizedTableToFile(minimizedTable, transitions);
    return constructDFA();
}

map<string, map<string, string>> MinimizeDFA:: getMinimizedTable(){
    return minimizedTable;
}

string MinimizeDFA:: getStartState(){
    return startState;
}

map<string, string> MinimizeDFA::getClassType(){
    return classType;
}