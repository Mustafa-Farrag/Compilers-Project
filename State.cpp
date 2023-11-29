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

vector<Transition> State::getEpsilonTransitions(){
    return epsilonTransitions;
}

vector<State*> State::getEpsilonStates(){
    vector<State*> states;
    queue<State*> statesQueue;

    for(auto trans: epsilonTransitions){
        State* nextS = trans.applyInput("");
        statesQueue.push(nextS);
    }

    while(!statesQueue.empty()){
        State* state = statesQueue.front();

        for(auto trans: state->epsilonTransitions){
            State* nextS = trans.applyInput("");
            statesQueue.push(nextS);
        }

        states.push_back(state);
        statesQueue.pop();
    }

    return states;
}

void State::addTransition(Transition trans){
    if(trans.getConditionStr() == "[\\L]")
        epsilonTransitions.push_back(trans);
    else
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
