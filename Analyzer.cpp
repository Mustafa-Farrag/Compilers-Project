#include <iostream>
#include <fstream>
#include "headers/Analyzer.h"

Analyzer:: Analyzer (DFA* dfa){
    this->dfa = dfa;
}


void Analyzer::analyze(string filepath) {
    ifstream inputFile(filepath);  
    if (!inputFile.is_open()) {
        cerr << "Unable to open file!" << endl;
        return;
    }

    char character;
    DFAState* currentState = dfa->getStartState();
    while (inputFile.get(character)) {
        cout << character;
        if(currentState->getIsAccepting()){
            cout << "  accepted "<< currentState->getClassType()<<"\n";
        }
        currentState = currentState->getNextState(character + "");
    }

    inputFile.close();
 
    return;
}
