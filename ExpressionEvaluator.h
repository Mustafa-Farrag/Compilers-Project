#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H

#include <bits/stdc++.h>
#include <regex>

using namespace std;

class ExpressionEvaluator {
public:
    ExpressionEvaluator(map<string, string> regularDefs);
    string evaluateRegExpression(string reg);  

private:
    map<string, string> regularDefs;  
    string removeRegExpressionDep (string expression);
    string infixToPostfix(string infix);
    string evaluatePostfix(string postfix);
};

#endif