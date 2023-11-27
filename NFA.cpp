#include "headers/NFA.h"

NFA::NFA(string condition){
    startState = new State(0, true, false);
    acceptState = new State(1, false, true);
    startState->addTransition(Transition(condition, acceptState));
}

NFA::~NFA() {
    delete startState;
    delete acceptState;
}

State* NFA::getStartState(){
    return startState;
}

State* NFA::getAcceptState(){
    return acceptState;
}
