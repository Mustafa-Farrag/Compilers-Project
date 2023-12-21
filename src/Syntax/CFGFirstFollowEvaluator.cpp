#include "Syntax/CFGElement.h"
#include "Syntax/CFGFirstFollowEvaluator.h"

CFGFirstFollowEvaluator::CFGFirstFollowEvaluator(CFGElement* start, map<string, CFGElement*> elements){
    this->start = start;
    this->elements = elements;
    computeFirstSet();
}

void CFGFirstFollowEvaluator::computeFirstSet(){

    for (auto element : elements)
        set<string> nonTerminalFirstSet = computeNonTerminalFirstSet(element.second);
    
}

set<string> CFGFirstFollowEvaluator::computeNonTerminalFirstSet(CFGElement* nonTerminal){

    if (firstSet.find(nonTerminal) != firstSet.end())
        return firstSet[nonTerminal];

    set<string> nonTerminalFirstSet;
    
    for (vector<CFGElement*> production : nonTerminal->getProductions()){
        
        for (int i = 0; i < production.size(); i++){
            
            if (production[i]->getIsTerminal() == true){
                
                nonTerminalFirstSet.insert(production[i]->getName());
                
                break;
            
            } else {
                
                set<string> newSet = computeNonTerminalFirstSet(production[i]);
                nonTerminalFirstSet.insert(newSet.begin(), newSet.end());
                
                if(i == production.size()-1 && production[i]->getHasEpsilon() == true) nonTerminalFirstSet.insert("\\L");
                
                if (production[i]->getHasEpsilon() == false)
                    break;
            }

        }
    }

    firstSet[nonTerminal] = nonTerminalFirstSet;

    return nonTerminalFirstSet;
}

void CFGFirstFollowEvaluator::computeFollowSet(){
    
}

map<CFGElement*, set<string>> CFGFirstFollowEvaluator::getFirstSet(){
    return firstSet;
}

map<CFGElement*, set<string>> CFGFirstFollowEvaluator::getFollowSet(){
    return followSet;
}

