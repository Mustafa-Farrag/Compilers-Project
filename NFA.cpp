#include "headers/NFA.h"

NFA::NFA(string condition, int counter){
    startState = new State(counter, true, false);
    acceptState = new State(counter+1, false, true);
    startState->addTransition(Transition(condition, acceptState));
}

NFA::~NFA(){
    delete startState;
    delete acceptState;
}

State* NFA::getStartState(){
    return startState;
}

State* NFA::getAcceptState(){
    return acceptState;
}
