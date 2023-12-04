#include "headers/MinimizeDFA.h"
#include <iostream>

/*
 * Separates acceptance states from non-acceptance states as 
 * a first step towards minimization.
 */
map<string, int> MinimizeDFA:: separateNonAcceptAndAcceptStates(
    map<string, set<int>> nStates_To_NStates, 
    map<int, State*> getidStatesMap)
    {
    map<string, int> index;
    for (auto it = nStates_To_NStates.begin(); it != nStates_To_NStates.end(); ++it) {
        string state = it->first;
        if(isAcceptanceState(state)){
            index[state] = 1;
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
    stateIndex[phiState] = 0;

    int numOfGroups = 2;

    while(true){
        set<int> set;
        map<string, int> newIndex;
        map<string, int> GroupIndex;
        int currentIndex = 0;

         for (auto it = stateIndex.begin(); it != stateIndex.end(); ++it) {
            string state = it->first;
            string group = "";
            map<string, string> stateTransition = DFATransitionTable[state];
            for (const auto& transition : transitions) {
                auto nextState = stateTransition.find(transition);
                if (nextState != stateTransition.end()) {
                    group += stateIndex[nextState->second];
                }else{
                    group += stateIndex[phiState];
                }
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

// vector<vector<string>> MinimizeDFA::separateStatesIntoGroups(map<string, int> &groupIndex ){
//      int maxIndex = 0;
//     for (const auto& pair : groupIndex) {
//         maxIndex = max(maxIndex, pair.second);
//     }

//     vector<vector<string>> groups(maxIndex);

//     for (const auto& pair : groupIndex) {
//         int index = pair.second;  
//         string str = pair.first;
//         groups[index].push_back(str);
//     }

//     return groups;
// }

void MinimizeDFA::appendDataToFile(const map<string, map<string, string>>& data, const set<string>& horizontalHeaders, const string& filename) {
    ofstream outputFile(filename, ios::app);

    if (!outputFile.is_open()) {
        cerr << "Unable to open file!" << endl;
        return;
    }

    const int columnWidth = 6; 

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
    string filename = "MinimizedDFA.txt";
    size_t halfSize = transitions.size() / 2;
    auto it = transitions.begin();

    std::advance(it, halfSize);

    std::set<std::string> firstHalf(transitions.begin(), it);
    std::set<std::string> secondHalf(it, transitions.end());

    appendDataToFile(minimizedTable, firstHalf, filename);
    appendDataToFile(minimizedTable, secondHalf, filename);
}

bool MinimizeDFA:: isStartState(string state){
    // map<int, State*> idStatesMap = dfahandler->getidStatesMap();
    // map<string, set<int>> nStates_To_NStates = dfahandler->getnStates_To_NStates();
    // set<int> nstates = nStates_To_NStates[state];
    // for(auto s : nstates){
    //     if(idStatesMap[s]->getIsStart()){
    //         return true;
    //     }
    // }
    // return false;
   return startState == state;
}

bool MinimizeDFA:: isAcceptanceState(string state){
    // map<int, State*> idStatesMap = dfahandler->getidStatesMap();
    // map<string, set<int>> nStates_To_NStates = dfahandler->getnStates_To_NStates();
    // set<int> nstates = nStates_To_NStates[state];
    // for(auto s : nstates){
    //     if(idStatesMap[s]->getIsAccept()){
    //         return idStatesMap[s]->getClassType();
    //     }
    // }
    // return "";
    if(classType.find(state) != classType.end()){
        return true;
    }
    return false;
}

DFA* MinimizeDFA::constructDFA(){
    DFA* dfa = new DFA();
    dfa->convertMap(minimizedTable, startState, classType);
    return dfa;
}

DFA* MinimizeDFA::constructMinimizedDFATable(DFAHandler* dfahandler){
    map<string, map<string, string>> DFATransitionTable = dfahandler->getDFATransitionTable();
    set<string> transitions = formTransitions(DFATransitionTable);
    map<string, int> groupIndex = getIndexOfGroupForMinimized(dfahandler, transitions);
    map<string, map<string, string>> table;
    
    startState = dfahandler->getStartState();
    classType = dfahandler->getAcceptanceStateToClassType();

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
                }else{
                    nextStates[transition] = to_string(groupIndex["phi"]);
                }
            }
            table[index] = nextStates;
        }
        // if(isStartState(dfahandler, state)){
        //     startState = index;
        // }
        // string type = CheckAcceptanceState(dfahandler, state);
        // if(type != ""){
        //     classType[index] = type;
        // }
        // oldNameToNewName[state] = index;
    }
    this->minimizedTable = table;
    // this->oldNameToNewName = oldNameToNewName;
    // this->transitions = transitions;
    printMinizedTableToFile(minimizedTable, transitions);
    return constructDFA();
}

// set<string> MinimizeDFA::getTransitions(){
//     return transitions;
// }

// map<string, string> MinimizeDFA:: getOldNameToNewName(){
//     return oldNameToNewName;
// }

map<string, map<string, string>> MinimizeDFA:: getMinimizedTable(){
    return minimizedTable;
}

string MinimizeDFA:: getStartState(){
    return startState;
}

map<string, string> MinimizeDFA::getClassType(){
    return classType;
}