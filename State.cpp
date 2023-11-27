#include "headers/State.h"
#include "headers/Transition.h"

State::State(int n){
    num = n;
    isStart = false;
    isAccept = false;
}
State::State(int n, bool start, bool accept){
    num = n;
    isStart = start;
    isAccept = accept; 
}

bool State::getIsStart(){
    return isStart;
}

bool State::getIsAccept(){
    return isAccept;
}

vector<Transition> State::getTransitions(){
    return transitions;
}

void State::addTransition(Transition trans){
    transitions.push_back(trans);
}

vector<State*> State::applyInput(string in){
    vector<State*> nextStates;
    for(auto transition:transitions){
        State* nextSt = transition.applyInput(in);
        if(nextSt != nullptr) nextStates.push_back(nextSt);
    }
    return nextStates;
}
