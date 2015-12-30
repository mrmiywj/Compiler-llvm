#include <iostream>
#include <string>
#include <fstream>
#include "../include/datastruct.h"
#include <stdio.h>


using namespace std;

ofstream fout;

extern  "C"{
    Node* parse(char* filename);
    void walkThrough(Node* n,int layer);
}

void code_PROGRAM(Node* n){
	string code = "";
	code += ";author YWJ\n";
	code += "@.str = private unnamed_addr constant [3 x i8] c\"%d\\00\", align 1\n";
	code += "@.str1 = private unnamed_addr constant [4 x i8] c\"%d\\00\", align 1\n";
	code += "declare i32 @printf(*i8, ...)\n";
	code += "declare i32 @__isoc99_scanf(i8*, ...)\n";
	cout<<code;
	return;
}

int main(int argc, char* argv[])
{
    Node* head;
    fout.open("out.ll");
    if (argc == 2)
    {
        head = parse(argv[1]);
    }
    code_PROGRAM(head);
    std::cout<<"Hhahaha";
    //walkThrough(head,1);
}
