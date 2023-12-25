#pragma once

#include <bits/stdc++.h>
#include "CFGElement.h"
#include "../Lexical/Tokenizer.h"


using namespace std;

class Tracer{
private:
    map<string, map<string, vector<string>>> parserTable ;
    map<string, CFGElement*> elements;
    Tokenizer* tokenizer;
    CFGElement* start;
    
public:
    Tracer(CFGElement* start, map<string, map<string, vector<string>>> parserTable , 
    map<string, CFGElement*> elements, Tokenizer* tokenizer);

    void trace();
};