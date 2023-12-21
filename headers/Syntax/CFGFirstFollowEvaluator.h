#pragma once

#include <bits/stdc++.h>
#include "CFGElement.h"
#include <regex>
using namespace std;

class CFGFirstFollowEvaluator{
private:
    CFGElement* start;
    map<string, CFGElement*> elements;
    map<CFGElement*, set<string>> firstSet;
    map<CFGElement*, set<string>> followSet;
    void computeFirstSet();
    set<string> computeNonTerminalFirstSet(CFGElement* nonTerminal);
    void computeFollowSet();
    set<string> computeNonTerminalFollowSet(CFGElement* nonTerminal);

public:
    CFGFirstFollowEvaluator(CFGElement* start, map<string, CFGElement*> elements);
    map<CFGElement*, set<string>> getFirstSet();
    map<CFGElement*, set<string>> getFollowSet();
};