#include "Lexical/Transition.h"
#include "Lexical/State.h"

Transition::Transition(string in, State* nState){
    conditionStr = "[" + in + "]";
    condition = regex(conditionStr);
    nextState = nState;
}

Transition::Transition(Transition* other,  map<State*, State*>* stateGetter, int* counter){
    conditionStr = other->conditionStr;
    condition = regex(other->conditionStr);

    if (stateGetter->find(other->nextState) != stateGetter->end())
        nextState = (*stateGetter)[other->nextState];
    else{
        nextState = new State(other->nextState, stateGetter, counter);
        (*stateGetter)[other->nextState] = nextState; 
    }
    
}

string Transition::getConditionStr(){
    return conditionStr;
}

State* Transition::applyInput(string in){
    bool isContainingRange;

    if(conditionStr.length() != 3){
        isContainingRange = 
        regex_match(in, regex("\\(\\?\\!.+\\)\\"+conditionStr.substr(0, conditionStr.length()-1)+"\\]"));
    }else{
        isContainingRange = false;
    }

    if(conditionStr == "[\\L]" || conditionStr == in || isContainingRange){
        return nextState;
    }
    
    if (regex_match(in.substr(1, in.length()-2), condition)) {
        return nextState;
    } else {
        return nullptr;
    }
}
