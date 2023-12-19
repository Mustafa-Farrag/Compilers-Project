#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "CFGElement.h"

using namespace std;

class CFGInputParser{
private:
    map<string, vector<string>> productions;
    map<string, CFGElement*> cfg;
    CFGElement* startSymbol;
    void parse(string inputFilePath);
    void constructProdsList();
    
public:
    CFGInputParser(string inputFilePath);
    map<string, CFGElement*> getCFGElements();
    CFGElement* getStartSymbol();
};