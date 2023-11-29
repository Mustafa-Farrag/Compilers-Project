#include "headers/Transition.h"
#include "headers/State.h"

Transition::Transition(string in, State* nState){
    conditionStr = "[" + in + "]";
    condition = regex(conditionStr);
    nextState = nState;
}

Transition::Transition(Transition* other,  map<State*, State*>* stateGetter){
    conditionStr = other->conditionStr;
    condition = regex(other->conditionStr);

    if (stateGetter->find(other->nextState) != stateGetter->end())
        nextState = (*stateGetter)[other->nextState];
    else{
        nextState = new State(other->nextState, stateGetter);
        (*stateGetter)[other->nextState] = nextState; 
    }
    
}

string Transition::getConditionStr(){
    return conditionStr;
}

State* Transition::applyInput(string in){
    if(conditionStr == "[\\L]"){
        return nextState;
    }
    
    if (regex_match(in, condition)) {
        return nextState;
    } else {
        return nullptr;
    }
}
