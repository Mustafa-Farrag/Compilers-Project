#include <bits/stdc++.h>
#include "DFA.h"

using namespace std;

class Tokenizer{
private:
    DFA* dfa;

public:
    Tokenizer(DFA* dfa);
    void tokenize (string filepath);
};
