#include "Syntax/Tracer.h"

Tracer::Tracer(CFGElement* start, map<string, map<string, vector<string>>> parserTable , 
    map<string, CFGElement*> elements, Tokenizer* tokenizer){
    this->start = start;
    this->parserTable = parserTable;
    this->elements = elements;
    this->tokenizer = tokenizer;
}

void printStack(stack<CFGElement*> s) {
    while (!s.empty()) {
        cout << s.top()->getName() << " ";
        s.pop();
    }
    cout << "\n";
}


void Tracer::trace(){
    stack<CFGElement*> stack;

    stack.push(new CFGElement("$", true));
    stack.push(start);

    // printStack(stack);

    string currtoken = tokenizer->getNextToken();
    while(!stack.empty()){
        CFGElement* element = stack.top();
        stack.pop();
        if(element->getIsTerminal()){
            if(element->getName() == "\\L"){
                continue;
            }
            if(element->getName() == currtoken){
                cout << "Matched token " << element->getName() << "\n";
            }
            else{
                cout << "Error: missing " << currtoken << ", inserted \n";
            }
            // printStack(stack);
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
            // printStack(stack);
            continue;
        }else{
            for (auto it = production.rbegin(); it != production.rend(); ++it) {
                stack.push(elements[*it]);
            }
            // printStack(stack);
        }
    }
    if(stack.empty()){
        cout << "Successful Matching\n";
    }
    cout << "Traced";
}