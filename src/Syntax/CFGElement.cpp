#include "Syntax/CFGElement.h"

CFGElement::CFGElement(string name, bool isTerminal){
    this->name = name;
    this->isTerminal = isTerminal;
    hasEpsilon = false;
    vector<CFGElement*> newProduction;
    produtions.push_back(newProduction);
}

string CFGElement::getName(){
    return name;
}

bool CFGElement::getIsTerminal(){
    return isTerminal;
}

vector<vector<CFGElement*>> CFGElement::getProductions(){
    return produtions;
}

void CFGElement::pushElement(CFGElement* element){
    produtions.back().push_back(element);
}

void CFGElement::pushProduction(){
    vector<CFGElement*> newProduction;
    produtions.push_back(newProduction);
}

bool CFGElement::getHasEpsilon(){
    return hasEpsilon;
}

void CFGElement::setHasEpsilon(bool hasEpsilon){
    this->hasEpsilon = hasEpsilon;
}