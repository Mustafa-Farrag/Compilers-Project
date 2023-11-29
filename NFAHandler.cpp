#include "headers/NFAHandler.h"
#include "headers/NFA.h"
#include "headers/State.h"
#include "headers/Transition.h"

NFAHandler :: NFAHandler(){}

NFA* NFAHandler :: performUnion(NFA* nfa1, NFA* nfa2) {
    State* newStartState = new State(0, true, false);
    State* newAcceptanceState = new State(0, false, true);

    nfa1->getStartState()->setIsStart(false);
    nfa2->getStartState()->setIsStart(false);

    nfa1->getAcceptState()->setIsAccept(false);
    nfa2->getAcceptState()->setIsAccept(false);

    Transition t1("\\L", nfa1->getStartState());
    Transition t2("\\L", nfa2->getStartState());

    Transition t3("\\L", newAcceptanceState);
    Transition t4("\\L", newAcceptanceState);

    newStartState->addTransition(t1);
    newStartState->addTransition(t2);

    nfa1->getAcceptState()->addTransition(t3);
    nfa2->getAcceptState()->addTransition(t4);

    NFA* newNFA = new NFA("\\L");
    newNFA->setStartState(newStartState);
    newNFA->setAcceptState(newAcceptanceState);

    return newNFA;

}

NFA* NFAHandler :: performConcatination(NFA* nfa1, NFA* nfa2) {
    
    nfa2->getStartState()->setIsStart(false);
    nfa1->getAcceptState()->setIsAccept(false);

    vector<Transition> transitions = nfa2->getStartState()->getTransitions();
    vector<Transition> epsTransitions = nfa2->getStartState()->getEpsilonTransitions();
    
    for (int i = 0; i < transitions.size(); i++)
        nfa1->getAcceptState()->addTransition(transitions[i]);
    
    for (int i = 0; i < epsTransitions.size(); i++)
        nfa1->getAcceptState()->addTransition(epsTransitions[i]);

    NFA* newNFA = new NFA("\\L");
    newNFA->setStartState(nfa1->getStartState());
    newNFA->setAcceptState(nfa2->getAcceptState());

    return newNFA;
}

NFA* NFAHandler :: performKleenClosure(NFA* nfa1) {
    State* newStartState = new State(0, true, false);
    State* newAcceptanceState = new State(0, false, true);

    nfa1->getStartState()->setIsStart(false);
    nfa1->getAcceptState()->setIsAccept(false);

    Transition t1("\\L", nfa1->getStartState());
    Transition t2("\\L", nfa1->getStartState());
    Transition t3("\\L", newAcceptanceState);
    Transition t4("\\L", newAcceptanceState);

    nfa1->getAcceptState()->addTransition(t1);
    nfa1->getAcceptState()->addTransition(t3);

    newStartState->addTransition(t2);
    newStartState->addTransition(t4);

    NFA* newNFA = new NFA("\\L");
    newNFA->setStartState(newStartState);
    newNFA->setAcceptState(newAcceptanceState);

    return newNFA;
}

NFA* NFAHandler :: performKleenClosurePlus(NFA* nfa1) {
    // NFA newNFA = performKleenClosure(nfa1);
    
    // State newStartState(0, true, false);

    // newNFA.getStartState().start = false;

    // Transition t1("\\L", nfa1.getStartState());
    // Transition t2("\\L", nfa1.getStartState());
    // Transition t3("\\L", newAcceptanceState);
    // Transition t4("\\L", newAcceptanceState);

    // nfa1.getAcceptState()->addTransition(t1);
    // nfa1.getAcceptState()->addTransition(t3);

    // newStartState.addTransition(t2);
    // newStartState.addTransition(t4);

    NFA newNFA("\\L");
    // newNFA.startState = newStartState;
    // newNFA.acceptState = newAcceptanceState;
    return &newNFA;

}

NFA* NFAHandler :: performConcatinationCombination(vector<NFA*> nfas){
    NFA* newNFA = nfas[0];
    
    for (int i = 1; i < nfas.size(); i++)
        newNFA = performConcatination(newNFA, nfas[i]);
 
    return newNFA;
}

NFA* NFAHandler :: performUnionCombination(vector<NFA*> nfas){
    NFA* newNFA = new NFA("\\L");
    newNFA->setStartState(new State(0, true, false));
    newNFA->setAcceptState(nullptr);

    for (int i = 0; i < nfas.size(); i++) {
            newNFA->getStartState()->addTransition(Transition("\\L", nfas[i]->getStartState()));
            nfas[i]->getStartState()->setIsStart(false);
    }

    return newNFA;
}

int main(){
    NFA* nfa1 = new NFA("0-9");
    NFA* nfa2 = new NFA("a-z");
    NFA* nfa3 = new NFA("A-Z");
    vector<NFA*> nfs;
    nfs.push_back(nfa1);
    nfs.push_back(nfa2);
    nfs.push_back(nfa3);

    NFAHandler handler = NFAHandler();

    NFA* newNFA = handler.performConcatinationCombination(nfs);

}