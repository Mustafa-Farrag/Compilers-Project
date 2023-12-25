#pragma once
// #include "CFGInputParser.h"
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>

using namespace std;

class LLGrammar{
private:

    map<string, vector<vector<string>>> productions;
    void removeLeftRecursion();
    void eliminateLR(string nonTerminal);
    void replaceProduction(string a1, string a2);
    void LeftFactoring();
    
public:
    LLGrammar();

    void parse(string filePath);
    void writeToFile(string filePath);
};