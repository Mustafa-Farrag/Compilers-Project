#include "../headers/DFA.h"

DFA::DFA() : startState(nullptr) {}

void DFA::setStartState(DFAState* start){
    startState = start;
}

DFAState* DFA::getStartState() {
    return startState;
}

void DFA::convertMap(const map<string, map<string, string>>& oldMap,
                    string startState, map<string, string> &classType){

    unordered_map<string, DFAState*> stateMap;

    for (const auto& pair : oldMap) {
        string currentStateId = pair.first;

        DFAState* currentState = nullptr;

        auto it = stateMap.find(currentStateId);
        if (it != stateMap.end()) {
            currentState = it->second;
        } else {
            currentState = new DFAState(currentStateId);
            stateMap[currentStateId] = currentState; 
            auto temp = classType.find(currentStateId);
            if(temp != classType.end()){
                currentState->setAccepting(true);
                currentState->setId(currentStateId);
                currentState->setClassType(temp->second);
            }
        }
        const auto& transitions = pair.second;
        for (const auto& transition : transitions) {
            string condition = transition.first;
            string nextStateId = transition.second;

            DFAState* nextState = nullptr;
            it = stateMap.find(nextStateId);
            if (it != stateMap.end()) {
                nextState = it->second;
            } else {
                nextState = new DFAState(nextStateId);
                stateMap[nextStateId] = nextState;
            }
            currentState->setTransition(condition, nextState);
        }

        if(currentStateId == startState){
            this->startState = currentState;
        }
    }
}