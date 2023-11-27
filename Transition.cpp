#include "headers/Transition.h"
#include "headers/State.h"

Transition::Transition(string in, State* nState){
    condition = regex(in);
    nextState = nState;
}

State* Transition::applyInput(string in){
    if (regex_match(in, condition)) {
        return nextState;
    } else {
        return nullptr;
    }
}
