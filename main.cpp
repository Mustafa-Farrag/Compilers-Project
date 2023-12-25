#include "Lexical/Lexical.h"
#include "Syntax/Syntax.h"

#include <bits/stdc++.h>

using namespace std;

int main(){
    // parse input file
    InputParser ip("Files/tokens_rules.txt");
    map<string, string> rd = ip.getRD();
    map<string, string> re = ip.getRE();
    set<string> keywords = ip.getKeywords();
    set<string> puncts = ip.getPuncts();

    // construct combined NFA
    ExpressionEvaluator* ee = new ExpressionEvaluator();
    NFA* combinedNFA = ee->computeCombinedNFA(rd, re, keywords, puncts);

    // construct NFA maps
    NFAHandler* nfahandler = new NFAHandler();
    map<int, map<string, vector<int>>> nfaTransitionTable = 
                                    nfahandler->getTransitionTable(combinedNFA->getStartState());
    cout << "NFA transition Table size: " << nfaTransitionTable.size()  <<endl;
    map<string, vector<int>> inputTranstionPairs = nfaTransitionTable.at(combinedNFA->getStartState()->getNum());
    map<int, State*> idStatesMap = nfahandler->getIdStateMap(combinedNFA->getStartState());

    // convert NFA to DFA
    DFAHandler* dfahandler = new DFAHandler(idStatesMap);

    map<string, map<string, string>> dfaTransitionTable = dfahandler->ConstructDFATransitionTable(nfaTransitionTable, combinedNFA->getStartState());
    cout << "DFA transition Table size: " << dfaTransitionTable.size() <<endl;

    // minimize DFA
    MinimizeDFA* minimizeDFA = new MinimizeDFA();
    DFA* dfa = minimizeDFA->constructMinimizedDFATable(dfahandler);

    cout << "minimized DFA transition Table size: " << minimizeDFA->getMinimizedTable().size()<< "\n";

    // Tokenization -> used in Tracing
    Tokenizer* tokenizer = new Tokenizer(dfa, "Files/syntax_test.txt");

    // parse cfg rules
    CFGInputParser cfgParser("Files/cfg_rules.txt");
    map<string, CFGElement*> elements = cfgParser.getCFGElements();
    CFGElement* start = cfgParser.getStartSymbol();
    CFGFirstFollowEvaluator* cfgffe = new CFGFirstFollowEvaluator(start, elements);
    map<CFGElement*, set<string>> firstSet = cfgffe->getFirstSet();
    map<CFGElement*, set<string>> followSet = cfgffe->getFollowSet();

    //parser table test
    CFGParserHandler parserHandler(cfgffe);
    map<string, map<string, vector<string>>> parserTable = parserHandler.getParserTable();

    cout << "Hi";

    Tracer* tracer = new Tracer(start, parserTable, elements, tokenizer);
    tracer->trace();

    return 0;
}