#pragma once

#include <bits/stdc++.h>
#include <regex>
using namespace std;

class CFGElement{
private:
    string name;
    bool isTerminal;
    bool hasEpsilon;
    vector<vector<CFGElement*>> produtions;
    
public:
    CFGElement(string name, bool isTerminal);
    string getName();
    bool getIsTerminal();
    vector<vector<CFGElement*>> getProductions();
    void pushElement(CFGElement* element);
    void pushProduction();
    bool getHasEpsilon();
    void setHasEpsilon(bool hasEpsilon);
};