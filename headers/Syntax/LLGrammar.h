#pragma once
// #include "CFGInputParser.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class LLGrammar{
private:

    map<string, vector<vector<string>>> productions;
    void removeLeftRecursion();
    void eliminateLR(string nonTerminal);
    void replaceProduction(string a1, string a2);

public:
    LLGrammar();

    void parse(string filePath);
};