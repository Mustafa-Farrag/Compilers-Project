%{
#include <stdio.h>
%}

DIGIT    [0-9]
LETTER   [a-zA-Z]
ID   ({LETTER}({LETTER}|{DIGIT})*)



%%
"int"       { printf("int\n"); }
"float"     { printf("float\n"); }
"boolean"     { printf("float\n"); }
"if"        { printf("if\n"); }
"else"      { printf("else\n"); }
"while"     { printf("while\n"); }

"="         { printf("assign\n"); }

"=="|"!="|">"|"<"|">="|"<="      { printf("relop\n"); }

";"         { printf(";\n"); }
"{"         { printf("{\n"); }
"}"         { printf("}\n"); }

"("         { printf("(\n"); }
")"         { printf(")\n"); }
","         { printf(",\n"); }
"+"|"-"         { printf("addop\n"); }
"*"|"/"         { printf("mulop\n"); }

{ID}  { printf("id\n"); }

{DIGIT}+|{DIGIT}+"."{DIGIT}+("E"[-+]?{DIGIT}+)?   { printf("num\n"); }

[ \t\n]                 ;  

.           { printf("unrecognized token\n"); }

%%



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open input file.\n");
        return 1;
    }

    yyin = file;  

    yylex();

    fclose(file); 
    return 0;
}




