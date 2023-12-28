#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "CFGElement.h"
#include "CFGFirstFollowEvaluator.h"
#include <iostream>
#include <fstream>

using namespace std;

class CFGParserHandler{
private:
    map<string, map<string, vector<string>>> parserTable;
    CFGFirstFollowEvaluator* FFEvaluator;
    map<string, CFGElement*> cfgElements;
    void constructParseTable();
    vector<string> getSuitableTransition(string row, string col, map<string, CFGElement*> elements, map<CFGElement*, set<string>> firstSets);
    
public:
    CFGParserHandler(CFGFirstFollowEvaluator* _FFEvaluator, map<string, CFGElement*> cfgElements);
    map<string, map<string, vector<string>>> getParserTable();
    void outputParserTable(string outputfilePath);
};