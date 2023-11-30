#pragma once

#include <bits/stdc++.h>
#include <regex>
#include "NFA.h"
#include "NFAHandler.h"
using namespace std;

class ExpressionEvaluator {
public:
    ExpressionEvaluator();
    NFA* computeCombinedNFA(map<string, string> rd, map<string, string> re, set<string> keywords, set<string> puncts);

private:
    NFAHandler* handler;
    map<string, string> regularDefs;
    string removeRegExpressionDep (string expression);
    string infixToPostfix(string infix);
    NFA* evaluateRegExpression(string reg, string type);
    NFA* evaluatePostfix(string postfix);
    NFA* evaluateKeyword(string keyword);
    NFA* evaluatePuncts(set<string> puncts);
};
