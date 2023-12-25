#include "Lexical/Tokenizer.h"
#include <filesystem>

Tokenizer:: Tokenizer (DFA* dfa, string inputPath){
    this->dfa = dfa;
    ended = false;
    inFile = ifstream(inputPath);
}

string Tokenizer::tokenize() {
    
    if (!inFile.is_open()) {
        return "$";
    }

    char c;
    
    while (true) {
        if(ended) {
            inFile.close();
            return "$";
        }

        if(inQueue.empty()){
            inFile.get(c);
        }else{
            c = inQueue.front();
            inQueue.pop();
        }

        DFAState* start = dfa->getStartState();
        DFAState* currentState = start;
        
        string acceptanceType = "";
        
        if(c==' ' || c=='\n') continue;

        currentState = currentState->getNextState(string(1, c));
        inQueue.push(c);
        while(!currentState->getIsPhi() && !ended){
            if(currentState->getIsAccepting()){
                acceptanceType = currentState->getClassType();
                while(!inQueue.empty()){
                    inQueue.pop();
                }
            }

            ended = !inFile.get(c);
            inQueue.push(c);
            currentState = currentState->getNextState(string(1, c));
        }

        if(acceptanceType == ""){
            cerr << "unrecognized token: " << c << endl;
            continue;
        }

        return acceptanceType;
    }
}

string Tokenizer::getNextToken(){
    return tokenize();
}
