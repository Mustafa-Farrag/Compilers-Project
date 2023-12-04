#include <bits/stdc++.h>
#include "DFA.h"

using namespace std;

class Analyzer{
private:
    DFA* dfa;

public:
    Analyzer(DFA* dfa);
    void analyze (string filepath);
};
