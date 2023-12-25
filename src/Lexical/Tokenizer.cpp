#include "Lexical/Tokenizer.h"
#include <filesystem>

Tokenizer:: Tokenizer (DFA* dfa, string inputPath){
    this->dfa = dfa;
    this->currTokenIndx = 0;
    tokenize(inputPath);
}

void Tokenizer::tokenize(string inputPath) {
    ifstream inputFile(inputPath);
    
    if (!inputFile.is_open()) {
        cerr << "Unable to open file!" << endl;
        return;
    }

    char character;
    string inputString = "";

    while (inputFile.get(character)){
        inputString += character;
    }

    inputFile.close();

    for (int i = 0; i < inputString.length(); i++){
        DFAState* start = dfa->getStartState();
        DFAState* currentState = start;
        
        string acceptanceType = "";
        int acceptanceIndex = -1;
        int startIdx = i;
        
        if(inputString[i]==' ' || inputString[i]=='\n') continue;

        currentState = currentState->getNextState(string(1, inputString[i]));
        while(!currentState->getIsPhi() && i<inputString.length()){
            if(currentState->getIsAccepting()){
                acceptanceType = currentState->getClassType();
                acceptanceIndex = i;
            }

            i++;
            currentState = currentState->getNextState(string(1, inputString[i]));
        }

        if(acceptanceIndex == -1){
            cerr << "unrecognized token: " << string(1, inputString[startIdx]) << endl;
            i = startIdx;
            continue;
        }

        tokens.push_back(acceptanceType);
        i = acceptanceIndex;
    }
}

 string Tokenizer::getNextToken(){
    if(currTokenIndx >= tokens.size()) return "$";
    return tokens[currTokenIndx++];
}
