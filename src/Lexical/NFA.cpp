#include "Lexical/NFA.h"

NFA::NFA(string condition, int* counter){
    startState = new State((*counter)++, true, false);
    acceptState = new State((*counter)++, false, true);
    startState->addTransition(new Transition(condition, acceptState));
}

NFA::NFA(State* start, State* end){
    startState = start;
    acceptState = end;
}

NFA::NFA(NFA* other, int* counter){
    map<State*, State*> stateGetter;
    startState = new State(other->startState, &stateGetter, counter);

    if (stateGetter.find(other->acceptState) != stateGetter.end())
        acceptState = stateGetter[other->acceptState];
    else
        acceptState = new State(other->acceptState, &stateGetter, counter);
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

void NFA::setStartState(State* state){
    startState = state;
}

void NFA::setAcceptState(State* state){
    acceptState = state;
}


