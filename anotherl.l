%{
#include "datastruct.h"
#include "smallc.tab.h"

#include <stdio.h>
#include <string.h>


int linecount = 1;
Node* createNode(char*, char*);
%}

int [0-9]+|0x[0-9A-F]+
id  [_a-zA-Z][_a-zA-Z0-9]*

%%
"." {
    printf("DOT\n");
    yylval.node = createNode("DOT",yytext);
    return DOT;
}


"!=" {
    printf("NOTEQ\n");
    yylval.node = createNode("NOTEQ",yytext);
    return NOTEQ;
}

"==" {
    printf("EQU\n");
    yylval.node = createNode("EQU",yytext);
    return EQU;
}

"<<=" {
    printf("SHLEFTAN\n");
    yylval.node = createNode("SHLEFTAN",yytext);
    return SHLEFTAN;
}

">>=" {
    printf("SHRIGHTAN\n");
    yylval.node = createNode("SHRIGHTAN",yytext);
    return SHRIGHTAN;
}

"<<" {
    printf("SHLEFT\n");
    yylval.node = createNode("SHLEFT",yytext);
    return SHLEFT;
}

">>" {
    printf("SHRIGHT\n");
    yylval.node = createNode("SHRIGHT",yytext);
    return SHRIGHT;
}

"++" {
    printf("INCR\n");
    yylval.node = createNode("INCR",yytext);
    return INCR;
}

"--" {
    printf("DECR\n");
    yylval.node = createNode("DECR",yytext);
    return DECR;
}

"&=" {
    printf("BANDAN\n");
    yylval.node = createNode("BANDAN",yytext);
    return BANDAN;
}

"^=" {
    printf("BXORAN\n");
    yylval.node = createNode("BXORAN",yytext);
    return BXORAN;
}

"|=" {
    printf("BORAN\n");
    yylval.node = createNode("BORAN",yytext);
    return BORAN;
}


"+=" {
    printf("PLUSAN\n");
    yylval.node = createNode("PLUSAN",yytext);
    return PLUSAN;
}


"-=" {
    printf("MINUSAN\n");
    yylval.node = createNode("MINUSAN",yytext);
    return MINUSAN;
}

"*=" {
    printf("TIMESAN\n");
    yylval.node = createNode("TIMESAN",yytext);
    return TIMESAN;
}

"/=" {
    printf("DIVAN\n");
    yylval.node = createNode("DIVAN",yytext);
    return DIVAN;
}

"<=" {
    printf("LEQ\n");
    yylval.node = createNode("LEQ",yytext);
    return LEQ;
}

">=" {
    printf("GEQ\n");
    yylval.node = createNode("GEQ",yytext);
    return GEQ;
}

"&&" {
    printf("LOGAND\n");
    yylval.node = createNode("LOGAND",yytext);
    return LOGAND;
}

"||" {
    printf("LOGOR\n");
    yylval.node = createNode("LOGOR",yytext);
    return LOGOR;
}

"!" {
    printf("LOGNOT\n");
    yylval.node = createNode("LOGNOT",yytext);
    return LOGNOT;
}

"+" {
    printf("PLUS\n");
    yylval.node = createNode("PLUS",yytext);
    return PLUS;
}

"-" {
    printf("MINUS\n");
    yylval.node = createNode("MINUS",yytext);
    return MINUS;
}

"*" {
    printf("TIMES\n");
    yylval.node = createNode("TIMES",yytext);
    return TIMES;
}

"/" {
    printf("DIV\n");
    yylval.node = createNode("DIV",yytext);
    return DIV;
}

"%" {
    printf("MOD\n");
    yylval.node = createNode("MOD",yytext);
    return MOD;
}

"&" {
    printf("BITAND\n");
    yylval.node = createNode("BITAND",yytext);
    return BITAND;
}

"^" {
    printf("BITXOR\n");
    yylval.node = createNode("BITXOR",yytext);
    return BITXOR;
}

"|" {
    printf("BITOR\n");
    yylval.node = createNode("BITOR",yytext);
    return BITOR;
}

"~" {
    printf("BITNOT\n");
    yylval.node = createNode("BITNOT",yytext);
    return BITNOT;
}

"<" {
    printf("LT\n");
    yylval.node = createNode("LT",yytext);
    return LT;
}

">" {
    printf("GT\n");
    yylval.node = createNode("GT",yytext);
    return GT;
}

";" {
    printf("SEMI\n");
    yylval.node = createNode("SEMI",yytext);
    return SEMI;
}

"," {
    printf("COMMA\n");
    yylval.node = createNode("COMMA",yytext);
    return COMMA;
}

"=" {
    printf("ASSIGNOP\n");
    yylval.node = createNode("ASSIGNOP",yytext);
    return ASSIGNOP;
}

"int" {
    printf("TYPE\n");
    yylval.node = createNode("TYPE",yytext);
    return TYPE;
}

"(" {
    printf("LP\n");
    yylval.node = createNode("LP",yytext);
    return LP;
}

")" {
    printf("RP\n");
    yylval.node = createNode("RP",yytext);
    return RP;
}

"[" {
    printf("LB\n");
    yylval.node = createNode("LB",yytext);
    return LB;
}

"]" {
    printf("RB\n");
    yylval.node = createNode("RB",yytext);
    return RB;
}

"{" {
    printf("LC\n");
    yylval.node = createNode("LC",yytext);
    return LC;
}

"}" {
    printf("RC\n");
    yylval.node = createNode("RC",yytext);
    return RC;
}

"struct" {
    printf("STRUCT\n");
    yylval.node = createNode("STRUCT",yytext);
    return STRUCT;
}

"return" {
    printf("RETURN\n");
    yylval.node = createNode("RETURN",yytext);
    return RETURN;
}

"if" {
    printf("IF\n");
    yylval.node = createNode("IF",yytext);
    return IF;
}

"else" {
    printf("ELSE\n");
    yylval.node = createNode("ELSE",yytext);
    return ELSE;
}

"break" {
    printf("BREAK\n");
    yylval.node = createNode("BREAK",yytext);
    return BREAK;
}

"continue" {
    printf("CONT\n");
    yylval.node = createNode("CONT",yytext);
    return CONT;
}


"for" {
    printf("FOR\n");
    yylval.node = createNode("FOR",yytext);
    return FOR;
}

{int} {
    printf("INT: %s\n", yytext);
    yylval.node = createNode("INT",yytext);
    return INT;
}

{id} {
    printf("ID: %s\n",yytext);
    yylval.node = createNode("ID",yytext);
    return ID;
}


["\n"] {
    linecount++;
}

%%

int yywrap(void)
{
    printf("yywrap\n");
    return 1;
}

Node* createNode(char* token, char* content)
{
    Node* res;
    res = malloc(sizeof(Node));
    res->content = strdup(content);
    res->token = strdup(token);
    res->next = NULL;
    res->child = NULL;
    return res;
}
