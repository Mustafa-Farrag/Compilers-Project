#include "Lexical/DFAState.h"

DFAState::DFAState(string id, bool isAccepting, string classType){
    this->id = id;
    this->isAccepting = isAccepting;
    this->classType = classType;
}

DFAState::DFAState(string id){
    this->id = id;
    this->isAccepting = false;
    this->classType = "";
}

void DFAState::setTransition(string i, DFAState* nextState) {
    transitions[i] = nextState;
}

void DFAState:: setIsPhi(bool isPhi){
    this->isPhi = isPhi;
}

bool DFAState:: getIsPhi(){
    return this->isPhi;
}

unordered_map<string, DFAState*> DFAState::getTransitions(){
    return transitions;
}

DFAState* DFAState::getNextState(string input) {

    for (const auto& transition : transitions) {
        regex reg (transition.first);

        if (regex_match(input, reg)){
            return transition.second;
        }
    }
    
    DFAState* phi = new DFAState("phi");
    phi->setIsPhi(true);
    return phi;
}

bool DFAState::getIsAccepting(){
    return isAccepting;
}

string DFAState::getId(){
    return id;
}

void DFAState::setId(string id){
    this->id = id;
}

void DFAState::DFAState::setAccepting(bool isAccepting){
    this->isAccepting = isAccepting;
}

void DFAState::setClassType(string classType){
    this->classType = classType;
}

string DFAState::getClassType() {
    return classType;
}
