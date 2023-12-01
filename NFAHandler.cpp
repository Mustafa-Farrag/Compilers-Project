#include "headers/NFAHandler.h"
#include "headers/NFA.h"
#include "headers/State.h"
#include "headers/Transition.h"
#include <map>
#include <stack>

NFAHandler :: NFAHandler(){ counter = 0; }

NFA* NFAHandler :: performUnion(NFA* nfa1, NFA* nfa2){
    State* newStartState = new State(counter++, true, false);
    State* newAcceptanceState = new State(counter++, false, true);

    nfa1->getStartState()->setIsStart(false);
    nfa2->getStartState()->setIsStart(false);

    nfa1->getAcceptState()->setIsAccept(false);
    nfa2->getAcceptState()->setIsAccept(false);

    Transition* t1 = new Transition("\\L", nfa1->getStartState());
    Transition* t2 = new Transition("\\L", nfa2->getStartState());

    Transition* t3 = new Transition("\\L", newAcceptanceState);
    Transition* t4 = new Transition("\\L", newAcceptanceState);

    newStartState->addTransition(t1);
    newStartState->addTransition(t2);

    nfa1->getAcceptState()->addTransition(t3);
    nfa2->getAcceptState()->addTransition(t4);

    NFA* newNFA = new NFA(newStartState, newAcceptanceState);

    return newNFA;
}

NFA* NFAHandler :: performConcatination(NFA* nfa1, NFA* nfa2){
    nfa2->getStartState()->setIsStart(false);
    nfa1->getAcceptState()->setIsAccept(false);

    vector<Transition*> transitions = nfa2->getStartState()->getTransitions();
    vector<Transition*> epsTransitions = nfa2->getStartState()->getEpsilonTransitions();

    for (int i = 0; i < transitions.size(); i++)
        nfa1->getAcceptState()->addTransition(transitions[i]);
    
    for (int i = 0; i < epsTransitions.size(); i++)
        nfa1->getAcceptState()->addTransition(epsTransitions[i]);

    NFA* newNFA = new NFA(nfa1->getStartState(), nfa2->getAcceptState());

    return newNFA;
}

NFA* NFAHandler :: performKleenClosure(NFA* nfa1) {
    State* newStartState = new State(counter++, true, false);
    State* newAcceptanceState = new State(counter++, false, true);

    nfa1->getStartState()->setIsStart(false);
    nfa1->getAcceptState()->setIsAccept(false);

    Transition* t1 = new Transition("\\L", nfa1->getStartState());
    Transition* t2 = new Transition("\\L", nfa1->getStartState());
    Transition* t3 = new Transition("\\L", newAcceptanceState);
    Transition* t4 = new Transition("\\L", newAcceptanceState);

    nfa1->getAcceptState()->addTransition(t1);
    nfa1->getAcceptState()->addTransition(t3);

    newStartState->addTransition(t2);
    newStartState->addTransition(t4);

    NFA* newNFA = new NFA(newStartState, newAcceptanceState);

    return newNFA;
}

NFA* NFAHandler :: performPositiveClosure(NFA* nfa1){
    NFA* clonedNFA = new NFA(nfa1, &counter);
    NFA* newNFA = performKleenClosure(nfa1);
    
    return performConcatination(clonedNFA, newNFA);
}

NFA* NFAHandler :: performConcatinationCombination(vector<NFA*> nfas){
    NFA* newNFA = nfas[0];
    
    for (int i = 1; i < nfas.size(); i++)
        newNFA = performConcatination(newNFA, nfas[i]);

    return newNFA;
}

NFA* NFAHandler :: performUnionCombination(vector<NFA*> nfas){
    State* newStartState = new State(counter++, true, false);
    NFA* newNFA = new NFA(newStartState, nullptr);

    for (int i = 0; i < nfas.size(); i++) {
            newNFA->getStartState()->addTransition(new Transition("\\L", nfas[i]->getStartState()));
            nfas[i]->getStartState()->setIsStart(false);
    }

    return newNFA;
}

NFA* NFAHandler :: performUnionCombinationOneAccept(vector<NFA*> nfas){
    State* newStartState = new State(counter++, true, false);
    State* newAcceptanceState = new State(counter++, false, true);
    NFA* newNFA = new NFA(newStartState, newAcceptanceState);

    for (int i = 0; i < nfas.size(); i++) {
            newNFA->getStartState()->addTransition(new Transition("\\L", nfas[i]->getStartState()));
            nfas[i]->getStartState()->setIsStart(false);
            nfas[i]->getAcceptState()->setIsAccept(false);
            nfas[i]->getAcceptState()->addTransition(new Transition("\\L", newAcceptanceState));
    }

    return newNFA;
}

//new for NFA_to_DFA task
map<int, map<string, vector<int>>> NFAHandler :: getTransitionTable(State* startState){
    map<int, map<string, vector<int>>> transitionTable;
    stack<State*> remainingStates;
    set<int> doneStates;

    remainingStates.push(startState);

    while(!remainingStates.empty()){
        State* curr = remainingStates.top();
        remainingStates.pop();

        //has been done before
        if(doneStates.find(curr->getNum()) != doneStates.end()){
            // cout<<"done before"<<endl;
            continue;
        }

        doneStates.insert(curr->getNum());
        
        //get epsilon transitions
        vector<State*> currEpsStates = curr->getEpsilonStates();
        vector<int> currEpsStatesNums;
        for(auto s: currEpsStates){
            remainingStates.push(s);
            currEpsStatesNums.push_back(s->getNum());
        }
        map<string, vector<int>> inputTransPairs;
        inputTransPairs.insert(pair<string, vector<int>>("\\L", currEpsStatesNums));

        //get non epsilon transitions
        vector<Transition*> Trans = curr->getTransitions();
        for(auto t: Trans){
            vector<State*> nStates = curr->applyInput(t->getConditionStr());
            vector<int> currNStatesNums;
            for (auto ns: nStates){
                remainingStates.push(ns);
                currNStatesNums.push_back(ns->getNum());
            }
            inputTransPairs.insert(pair<string, vector<int>>(t->getConditionStr(), currNStatesNums));
        }

        //update transition map
        transitionTable.insert(pair<int, map<string, vector<int>>>(curr->getNum(), inputTransPairs));
    }

    return transitionTable;
}

//new for NFA_to_DFA task
map<int, State*> NFAHandler :: getIdStateMap(State* startState){
    map<int, State*> idStateMap;
    stack<State*> remainingStates;
    set<int> doneStates;

    remainingStates.push(startState);

    while(!remainingStates.empty()){
        State* curr = remainingStates.top();
        remainingStates.pop();

        //has been done before
        if(doneStates.find(curr->getNum()) != doneStates.end()){
            // cout<<"done before"<<endl;
            continue;
        }

        doneStates.insert(curr->getNum());
        idStateMap.insert(pair<int, State*>(curr->getNum(), curr));
        
        //get epsilon transitions
        vector<State*> currEpsStates = curr->getEpsilonStates();
        vector<int> currEpsStatesNums;
        for(auto s: currEpsStates){
            remainingStates.push(s);
        }

        //get non epsilon transitions
        vector<Transition*> Trans = curr->getTransitions();
        for(auto t: Trans){
            vector<State*> nStates = curr->applyInput(t->getConditionStr());
            for (auto ns: nStates){
                remainingStates.push(ns);
            }
        }
    }

    return idStateMap;
}

 NFA* NFAHandler::createNFA(string condition){
    return new NFA(condition, &counter);
 }
 