# pragma once
#include <bits/stdc++.h>
#include "DFA.h"

using namespace std;

class Tokenizer{
private:
    DFA* dfa;
    int currTokenIndx;
    vector<string> tokens;

public:
    Tokenizer(DFA* dfa, string inputPath);
    void tokenize (string filepath);
    string getNextToken();
};
