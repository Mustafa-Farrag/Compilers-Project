#include "Syntax/Tracer.h"

Tracer::Tracer(CFGElement* start, map<string, map<string, vector<string>>> parserTable , 
    map<string, CFGElement*> elements, Tokenizer* tokenizer){
    this->start = start;
    this->parserTable = parserTable;
    this->elements = elements;
    this->tokenizer = tokenizer;
}

void printStack(stack<CFGElement*> s, ofstream& outputFile ) {
    string st = "";
    while (!s.empty()) {
        st += s.top()->getName();
        st += " ";
        s.pop();
    }
    st += "\n";
    outputFile << st;
}


void Tracer::trace(){
    cout << "----------- Parsing Actions -----------" << endl;
    ofstream outputFile("Files/Tracer Productions.txt");  

    stack<CFGElement*> stack;

    stack.push(new CFGElement("$", true));
    stack.push(start);

    printStack(stack, outputFile);

    string currtoken = tokenizer->getNextToken();
    while(!stack.empty()){
        if(currtoken == "$"){
            cout << "";
        }
        CFGElement* element = stack.top();
        stack.pop();
        if(element->getIsTerminal()){
            if(element->getName() == "\\L"){
                printStack(stack, outputFile);
                continue;
            }
            if(element->getName() == currtoken){
                cout << "Matched token " << element->getName() << "\n";
            }
            else{
                cout << "Error: missing " << element->getName() << ", inserted \n";
            }
            printStack(stack, outputFile);
            currtoken = tokenizer->getNextToken();
            continue;
        }
        map<string, vector<string>> transitions = parserTable[element->getName()];
        vector<string> production = transitions[currtoken];
        if(production.empty()){
            // Error
            cout << "Error: Discard "<< currtoken << "\n";
            stack.push(element);
            currtoken = tokenizer->getNextToken();

        }else if(production[0] == "sync"){
            cout << "sync\n";
            printStack(stack, outputFile);
            continue;
        }else{
            for (auto it = production.rbegin(); it != production.rend(); ++it) {
                stack.push(elements[*it]);
            }
            printStack(stack, outputFile);
        }
    }
    if(stack.empty()){
        cout << "Successful Matching\n";
    }
    cout << "Traced";
}