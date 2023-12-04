#include "../headers/Analyzer.h"

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
    stack<char> charStack;
    DFAState* start = dfa->getStartState();
    DFAState* currentState = start;
    bool read = true;
    string type = "";
    while (true) {
        if(read){
            inputFile.get(character); 
            charStack.push(character);
        }
        if (inputFile.eof()) {
            break;  
        } else if (inputFile.fail()) {
            std::cerr << "Failed to read from the file." << std::endl;
            break; 
        }

        string input(1, character);
        if(isspace(character)){
            string result = "";
            while (!charStack.empty()) {
                result = charStack.top() + result;  
                charStack.pop();  
            }
            cout << result << "  " << type << "\n";
            currentState = start;
            read = true;
            continue;
        }
        // cout << character << "  " <<currentState->getid() << " \n";
        if(currentState->getIsAccepting()){
            type = currentState->getClassType();
        }
        // else if(currentState->getNextState(input) == nullptr){
        //     currentState = start;
        //     read = true;
        // }
        currentState = currentState->getNextState(input);
        
        if(currentState == nullptr){
            
            currentState = start;
            // cout << " start " << start->getid() << "\n";
            read = false;
        }else{
            // cout << currentState->getid() << "\n";
            read = true;
        }
    }

    inputFile.close();
 
    return;
}
