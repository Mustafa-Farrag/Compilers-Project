#include "Syntax/CFGElement.h"
#include "Syntax/CFGFirstFollowEvaluator.h"

CFGFirstFollowEvaluator::CFGFirstFollowEvaluator(CFGElement* start, map<string, CFGElement*> elements){
    this->start = start;
    this->elements = elements;
    computeFirstSet();
    computeFollowSet();
}

void CFGFirstFollowEvaluator::computeFirstSet(){

    for (auto element : elements)
        if (!element.second->getIsTerminal())
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
    set<string> nonTerminalFollowSet = computeNonTerminalFollowSet(start);
    for (auto element : elements)
        if (!element.second->getIsTerminal())
            set<string> nonTerminalFollowSet = computeNonTerminalFollowSet(element.second);
    
}

set<string> CFGFirstFollowEvaluator::computeNonTerminalFollowSet(CFGElement* nonTerminal){

    if (followSet.find(nonTerminal) != followSet.end())
        return followSet[nonTerminal];

    set<string> nonTerminalFollowSet;

    if (nonTerminal == start)
        nonTerminalFollowSet.insert("$");

    for (auto element : elements){

        for (vector<CFGElement*> production : element.second->getProductions()){

                bool nonTerminalFound = false;
        
                for (int i = 0; i < production.size(); i++){

                    if (production[i] == nonTerminal){
                        nonTerminalFound = true;
                        if (i < production.size()-1)
                             continue;
                        else if  (element.second != nonTerminal)
                            {
                                set<string> followSetNonTerminal = computeNonTerminalFollowSet(element.second);
                                nonTerminalFollowSet.insert(followSetNonTerminal.begin(), followSetNonTerminal.end());
                            }

                         break;
                    } 
                    else if (!nonTerminalFound)
                        continue;

                    if (production[i]->getIsTerminal()) {
                            nonTerminalFollowSet.insert(production[i]->getName());
                            break;
                    } else {
                            set<string> firstSetNonTerminal = firstSet[production[i]];
                            firstSetNonTerminal.erase("\\L");
                            nonTerminalFollowSet.insert(firstSetNonTerminal.begin(), firstSetNonTerminal.end());
                            if (!production[i]->getHasEpsilon()) break;
                    }
                    
                    if (i == production.size()-1 && element.second != nonTerminal)
                    {
                         set<string> followSetNonTerminal = computeNonTerminalFollowSet(element.second);
                         nonTerminalFollowSet.insert(followSetNonTerminal.begin(), followSetNonTerminal.end());
                    }

                }
        }

    }

    followSet[nonTerminal] = nonTerminalFollowSet;

    return nonTerminalFollowSet;
}

map<CFGElement*, set<string>> CFGFirstFollowEvaluator::getFirstSet(){
    return firstSet;
}

map<CFGElement*, set<string>> CFGFirstFollowEvaluator::getFollowSet(){
    return followSet;
}

map<string, CFGElement*> CFGFirstFollowEvaluator::getElements(){
    return elements;
}

