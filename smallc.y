%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "datastruct.h"
#define YYDEBUG 1

Node* head;
Node* Record(char*, ...);
void walkThrough(Node* n, int layer);
Node* parse(char* filename);
int yyerror(char*);

FILE *fout;

extern int yydebug;
extern int linecount;
%}

%union {
    Node *node;
}

%token <node> ASSIGNOP TYPE
%token <node> LC RC
%token <node> STRUCT RETURN
%token <node> IF ELSE BREAK CONT FOR
%token <node> INT ID SEMI COMMA

%token <node> LP RP LB RB DOT
%token <node> LOGNOT INCR DECR BITNOT
%token <node> MOD TIMES DIV
%token <node> PLUS MINUS
%token <node> SHLEFT SHRIGHT
%token <node> GT GEQ LT LEQ
%token <node> EQU NOTEQ
%token <node> BITAND
%token <node> BITXOR
%token <node> BITOR
%token <node> LOGAND
%token <node> LOGOR
%token <node> PLUSAN MINUSAN TIMESAN DIVAN BANDAN BXORAN BORAN SHLEFTAN SHRIGHTAN

%type <node> PROGRAM EXTDEFS EXTDEF EXTVARS SPEC STSPEC OPTTAG
%type <node> VAR FUNC PARAS PARA STMTBLOCK STMTS STMT ESTMT
%type <node> DEFS DEF DECS DEC INIT EXP ARRS ARGS
%start PROGRAM

%right PLUSAN MINUSAN TIMESAN DIVAN BANDAN BORAN BXORAN SHLEFTAN SHRIGHTAN ASSIGNOP
%left LOGOR
%left LOGAND
%left BITOR
%left BITXOR
%left BITAND
%left EQU NOTEQ
%left GT GEQ LT LEQ
%left SHLEFT SHRIGHT
%left PLUS MINUS
%left MOD TIMES DIV
%right LOGNOT INCR DECR BITNOT
%left LP RP LB RB DOT

%%
PROGRAM:
	EXTDEFS{
		$$ = Record("PROGRAM", $1, NULL);
		//walkThrough($$, 0);
	};

EXTDEFS:
	EXTDEF EXTDEFS{
		$$ = Record("EXTDEFS",$1,$2,NULL);
	}
	| 			  {$$ = Record("EXTDEFS",NULL);};

EXTDEF:
	SPEC EXTVARS SEMI {
		$$ = Record("EXTDEF",$1,$2,$3,NULL);
	}
	| SPEC FUNC STMTBLOCK {
		$$ = Record("EXTDEF",$1,$2,$3,NULL);
	};

EXTVARS:
	DEC {
		$$ = Record("EXTVARS", $1,NULL);
	}
	| DEC COMMA EXTVARS {
		$$ = Record("EXTVARS", $1,$2,$3,NULL);
	}
	|  {
		$$ = Record("EXTVARS",NULL);
	};

SPEC:
	TYPE {
		$$ = Record("SPEC", $1, NULL);
	}
	| STSPEC {
		$$ = Record("SPEC", $1, NULL);
	};

STSPEC:
	STRUCT OPTTAG LC DEFS RC {
		$$ = Record("STSPEC", $1,$2,$3,$4,$5,NULL);
	}
	| STRUCT ID {
		$$ = Record("STSPEC", $1,$2,NULL);
	};

OPTTAG:
	ID {
		$$ = Record("OPTTAG", $1,NULL);
	}
	| {
		$$ = Record("OPTTAG", NULL);
	};

VAR :
	ID {
		$$ = Record("VAR",$1, NULL);
	}
	| VAR LB INT RB {
		$$ = Record("VAR",$1, $2, $3, $4, NULL);
	};

FUNC:
	ID LP PARAS RP {
		$$ = Record("FUNC",$1,$2,$3,$4, NULL);
	};

PARAS:
	PARA COMMA PARAS {
		$$ = Record("PARAS", $1, $2, $3, NULL);
	}
	| PARA {
		$$ = Record("PARAS", $1, NULL);
	}
	| {
		$$ = Record("PARAS", NULL);
	};

PARA:
	SPEC VAR {
		$$ = Record("PARA", $1, $2, NULL);
	};

STMTBLOCK:
	LC DEFS STMTS RC {
		$$ = Record("STMTBLOCK",$1, $2, $3, $4, NULL);
	};

STMTS:
	STMT STMTS {
		$$ = Record("STMTS", $1, $2, NULL);
	}
	| {
		$$ = Record("STMTS", NULL);
	};

STMT :
	EXP SEMI {
		$$ = Record("STMT", $1, $2, NULL);
	}
	| STMTBLOCK {
		$$ = Record("STMT",$1, NULL);
	}
	| RETURN EXP SEMI {
		$$ = Record("STMT", $1, $2, $3, NULL);
	}
	| IF LP EXP RP STMT ESTMT {
		$$ = Record("STMT", $1,$2,$3,$4,$5,$6,NULL);
	}
	| FOR LP EXP SEMI EXP SEMI EXP RP STMT {
		$$ = Record("STMT", $1,$2,$3,$4,$5,$6,$7,$8,$9,NULL);
	}
	| CONT SEMI {
		$$ = Record("STMT", $1,$2,NULL);
	}
	| BREAK SEMI {
		$$ = Record("STMT",$1,$2,NULL);
	};

ESTMT:
	ELSE STMT {
		$$ = Record("ESTMT", $1, $2,NULL);
	}
	| {
		$$ = Record("ESTMT", NULL);
	};

DEFS:
	DEF DEFS {
		$$ = Record("DEFS",$1,$2,NULL);
	}
	| {
		$$ = Record("DEFS", NULL);
	};

DEF:
	SPEC DECS SEMI {
		$$ = Record("DEF", $1, $2, $3,NULL);
	};

DECS:
	DEC COMMA DECS {
		$$ = Record("DECS", $1, $2, $3, NULL);
	}
	| DEC {
		$$ = Record("DECS", $1, NULL);
	};

DEC:
	VAR {
		$$ = Record("DEC",$1, NULL);
	}
	| VAR ASSIGNOP INIT {
		$$ = Record("DEC", $1, $2, $3,NULL);
	};

INIT:
	EXP {
		$$ = Record("INIT", $1, NULL);
	}
	| LC ARGS RC {
		$$ = Record("INIT", $1, $2, $3, NULL);
	};

EXP:
	ID {
		$$ = Record("EXP", $1, NULL);
	}
	| EXP PLUS EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP MINUS EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP TIMES EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP DIV EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP PLUSAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP MINUSAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP TIMESAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP DIVAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP MOD EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP SHLEFT EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP SHRIGHT EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP GT EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP GEQ EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP LT EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP LEQ EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP EQU EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP NOTEQ EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP BITAND EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP BITXOR EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP BITOR EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP LOGAND EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP LOGOR EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP BANDAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP BXORAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP BORAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP SHLEFTAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP SHRIGHTAN EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP DOT EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| EXP ASSIGNOP EXP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| MINUS EXP {
		$$ = Record("EXP", $1, $2, NULL);
	}
	| INCR EXP {
		$$ = Record("EXP", $1, $2, NULL);
	}
	| DECR EXP {
		$$ = Record("EXP", $1, $2, NULL);
	}
	| BITNOT EXP {
		$$ = Record("EXP", $1, $2, NULL);
	}
	| LOGNOT EXP {
		$$ = Record("EXP", $1, $2, NULL);
	}
	| LP EXP RP {
		$$ = Record("EXP", $1, $2, $3, NULL);
	}
	| ID LP ARGS RP {
		$$ = Record("EXP", $1, $2, $3, $4, NULL);
	}
	| ID ARRS {
		$$ = Record("EXP", $1, $2, NULL);
	}
	| INT {
		$$ = Record("EXP", $1, NULL);
	}
	| {
		$$ = Record("EXP", NULL);
	};

ARRS:
	LB EXP RB ARRS {
		$$ = Record("ARRS", $1, $2, $3, $4, NULL);
	}
	| {
		$$ = Record("ARRS", NULL);
	};

ARGS:
	EXP COMMA ARGS {
		$$ = Record("ARGS", $1, $2, $3, NULL);
	}
	| EXP {
		$$ = Record("ARGS", $1, NULL);
	};

%%

Node* Record(char* content, ...){
	Node* res = (Node*)malloc(sizeof(Node));
	res->token = strdup(content);
	res->content = NULL;
	res->attr.isLeft = 0;
	va_list ap;
	va_start(ap, content);

	Node* tmp;
	tmp = va_arg(ap, Node*);

	res->child = tmp;
	res->next = NULL;

	Node* ttmp;
	while(tmp != NULL){
		ttmp = va_arg(ap, Node*);
		tmp->next = ttmp;
		tmp = ttmp;
	}
	va_end(ap);

	head = res;
	return res;
}

void walkThrough(Node* n, int layer){
	if (n->token != NULL){
		for(int i = 0;i < layer * 2; i++)
			printf(" ");
		char* c = n->token;
		//if (strcmp(c,"LB") !=0  && strcmp(c,"RB")!=0 && strcmp(c,"LP")!=0 && strcmp(c,"RP")!=0 && strcmp(c,"LC")!=0 && strcmp(c,"RC")!=0)
			printf("%s", n->token);
	}
	if (n->content != NULL){
		printf("  ");
		printf("%s", n->content);
	}
	printf("\n");
	//printf("\n");
	if (n->child != NULL){
		walkThrough(n->child, layer+1);
	}//else
		//printf("\n");
	if (n->next != NULL) walkThrough(n->next, layer);

	return;
}

int yyerror(char *msg){
    //fprintf(stderr,"%s encountered. at %d\n", msg,linecount);
	return 0;
}

Node* parse(char* filename){
	yydebug = 0;
	freopen(filename,"r",stdin);
	//fout = fopen(argv[2],"w");
	yyparse();
	//if (head == NULL)
	//	printf("0\n");
	//printf("\n");
	//walkThrough(head, 1);
	//fclose(fout);
	return head;
}
