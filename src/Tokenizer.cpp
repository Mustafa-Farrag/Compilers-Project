#include "../headers/Tokenizer.h"
#include <filesystem>

Tokenizer:: Tokenizer (DFA* dfa){
    this->dfa = dfa;
}

void Tokenizer::tokenize(string inputPath) {
    filesystem::path filePath(inputPath);
    filesystem::path parentPath = filePath.parent_path();
    filesystem::path outputPath = parentPath / "tokens.txt";

    ifstream inputFile(inputPath);
    
    if (!inputFile.is_open()) {
        cerr << "Unable to open file!" << endl;
        return;
    }

    char character;
    string inputString = "", outputString = "";

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
        if(inputString[i]=='?'){
            cout << "";
        }
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
            outputString += "unrecognized token: " + string(1, inputString[startIdx]) + "\n";
            i = startIdx;
            continue;
        }

        outputString += acceptanceType + '\n';
        i = acceptanceIndex;
    }
    
    ofstream outputFile(outputPath, ios::trunc);

    if (outputFile.is_open()){
        outputFile << outputString << endl;
    } else {
        cerr << "Unable to write file!" << endl;
    }

    outputFile.close();

}
