#include "Lexical/ExpressionEvaluator.h"
#include "Lexical/NFA.h"
#include "Lexical/NFAHandler.h"

ExpressionEvaluator :: ExpressionEvaluator(){
    handler = new NFAHandler();
}

string ExpressionEvaluator :: removeRegExpressionDep (string expression) {
        int start = 0;
        string symbols = "()[]{}*+ |";
        
        while(start <= expression.length() - 1){
            
            if (symbols.find(expression[start]) == symbols.npos){
                
                string new_def = "";
                int left = start;

                while (start <= expression.length() - 1 && symbols.find(expression[start]) == symbols.npos)
                    new_def += expression[start++];
                
                if (regularDefs.find(new_def) != regularDefs.end()) {
                    expression.replace(left, start - left, "(" + regularDefs[new_def] + ")" );
                    start = left;
                }
            }

            start++;
        }

        return expression;
}

string ExpressionEvaluator :: infixToPostfix (string infix) {

    vector<char> postfixStack; 
    string postFix = "";
    string operators = "()*+|^";
    bool operand = false;

    map<char, int> precedence = {
        {'*', 3},
        {'+', 3},
        {'^', 2},
        {'|', 1}
    };

    int i = 0;
    string new_infix = "";
    
    while (i < infix.length()){
    if (i != 0 && infix[i] == '(' && infix[i-1] != '|' && infix[i-1] != '(')
        {
            new_infix += "^";
            new_infix += infix[i];
        }
    else if (i != infix.length()-1 && (infix[i] == ')' || infix[i] == '*' || infix[i] == '+') && operators.find(infix[i+1]) == operators.npos)
        {
            new_infix += infix[i];
            new_infix += "^";
        }
    else if(i != infix.length()-1 && operators.find(infix[i]) == operators.npos && operators.find(infix[i+1]) == operators.npos && !(infix[i] == '-' || infix[i] == '\\' || infix[i+1] == '-')){
        new_infix += infix[i];
        new_infix += "^";
    }
    else
        new_infix += infix[i];
    i++;
    }

    infix = new_infix;


    for(int i = 0; i < infix.length(); i++) {
        if (operators.find(infix[i]) == operators.npos || i > 0 && operators.find(infix[i]) != operators.npos && infix[i-1] == '\\') {
            postFix += infix[i];
            operand = true;
        }
        else{
            if (operand == true){
                operand = false;
                postFix += ' ';
            }
            if (infix[i] == '(') {
                postfixStack.push_back('(');
            }
            else if (infix[i] == ')') {
                while(postfixStack.back() != '(') {
                    postFix += postfixStack.back();
                    postfixStack.pop_back();
                }
                postfixStack.pop_back();
            }
            else {
                while (!postfixStack.empty() && precedence[infix[i]] <= precedence[postfixStack.back()]) {
                    postFix += postfixStack.back();
                    postfixStack.pop_back();
                }
                postfixStack.push_back(infix[i]);
            }
        }

    }

    if (operand == true)
        postFix += ' ';

    while (!postfixStack.empty()) {
        postFix += postfixStack.back();
        postfixStack.pop_back();
    }
    return postFix;
}

NFA* ExpressionEvaluator :: evaluatePostfix(string postfix){
    
    vector<NFA*> stack;
    string operators = "*+|^";
    string operand = "";
    int i = 0;
    for (char c : postfix) {
        if (operators.find(c) == operators.npos || operators.find(c) != operators.npos && i > 0 && postfix[i-1] == '\\' ) {
            // Operand encountered, push onto the stack
            if (c == ' '){
                stack.push_back(handler->createNFA(operand));
                operand = "";
            } else {
                if (operators.find(c) != operators.npos){
                    operand = "";
                    operand += c;
                }
                else operand += c; 
            }

        } else {
            if (c == '*'){
                NFA* operandNFA = stack.back();
                stack.pop_back();
                stack.push_back(handler->performKleenClosure(operandNFA));
            }
            else if (c == '+'){
                NFA* operandNFA = stack.back();
                stack.pop_back();
                stack.push_back(handler->performPositiveClosure(operandNFA));
            }
            else if (c == '^'){
                NFA* operandNFA2 = stack.back();
                stack.pop_back();

                NFA* operandNFA1 = stack.back();
                stack.pop_back();

                stack.push_back(handler->performConcatination(operandNFA1, operandNFA2));
            }
            else{
                NFA* operandNFA2 = stack.back();
                stack.pop_back();

                NFA* operandNFA1 = stack.back();
                stack.pop_back();

                stack.push_back(handler->performUnion(operandNFA1, operandNFA2));
            }
        }
        i++;
    }

    return stack.back();
}

NFA* ExpressionEvaluator :: evaluateRegExpression(string reg, string type){
    reg = removeRegExpressionDep(reg);
    reg.erase(remove_if(reg.begin(), reg.end(), ::isspace), reg.end());

    string postfix = infixToPostfix(reg);

    NFA* resultNFA = evaluatePostfix(postfix);
    resultNFA->getAcceptState()->setClassType(type);

    return resultNFA;
}

NFA* ExpressionEvaluator :: evaluateKeyword(string keyword){
    vector<NFA*> nfas;

    for(char ch: keyword){
        nfas.push_back(handler->createNFA(string(1, ch)));
    }

    NFA* resultNFA = handler->performConcatinationCombination(nfas);
    resultNFA->getAcceptState()->setClassType(keyword);

    return resultNFA;
}

NFA* ExpressionEvaluator :: evaluatePunct(string punct){
    
    NFA* resultNFA = handler->createNFA(punct);
    resultNFA->getAcceptState()->setClassType(punct);

    return resultNFA;
}

NFA* ExpressionEvaluator :: computeCombinedNFA(map<string, string> rd, map<string, string> re, set<string> keywords, set<string> puncts){
    regularDefs = rd;
    vector<NFA*> nfas;

    for(auto &pair: re){
        nfas.push_back(evaluateRegExpression(pair.second, pair.first));
    }

    for (set<string>::iterator keyword = keywords.begin(); keyword != keywords.end(); ++keyword) {
        nfas.push_back(evaluateKeyword(*keyword));
    }

    for (set<string>::iterator punct = puncts.begin(); punct != puncts.end(); ++punct) {
        nfas.push_back(evaluatePunct(*punct));
    }

    return handler->performUnionCombination(nfas);
}
