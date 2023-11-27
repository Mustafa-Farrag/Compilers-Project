#include "headers/NFA.h"

NFA::NFA(string condition){
    NFA::startState = &State(0, true, false);
    NFA::startState = &State(1, false, true);
}

State* NFA::getStartState(){
    return NFA::startState;
}

State* NFA::getAcceptState(){
    return NFA::acceptState;
}