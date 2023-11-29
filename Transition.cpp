#include "headers/Transition.h"
#include "headers/State.h"

Transition::Transition(string in, State* nState){
    conditionStr = "[" + in + "]";
    condition = regex(in);
    nextState = nState;
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
