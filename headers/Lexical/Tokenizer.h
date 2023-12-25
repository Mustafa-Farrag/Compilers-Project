# pragma once
#include <bits/stdc++.h>
#include "DFA.h"

using namespace std;

class Tokenizer{
private:
    DFA* dfa;
    ifstream inFile;
    bool ended;
    queue<char> inQueue;

public:
    Tokenizer(DFA* dfa, string inputPath);
    string tokenize();
    string getNextToken();
};
