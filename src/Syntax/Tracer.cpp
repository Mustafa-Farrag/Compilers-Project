#include "Syntax/Tracer.h"

Tracer::Tracer(CFGElement* start, map<string, map<string, vector<string>>> parserTable , 
    map<string, CFGElement*> elements, Tokenizer* tokenizer){
    this->start = start;
    this->parserTable = parserTable;
    this->elements = elements;
    this->tokenizer = tokenizer;
}


void Tracer::trace(){

    string nextToken;
    while((nextToken = tokenizer->getNextToken()) != ""){

    }
}