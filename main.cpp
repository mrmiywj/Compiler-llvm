#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "datastruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"
//#include "smallc.tab.h"
//#include "y.tab.h"
using namespace std;
using namespace llvm;
#define CODE_LENGTH 250

ofstream fout;
int counter = 0;


extern  "C"{
    Node* parse(char* filename);
    void walkThrough(Node* n,int layer);
}


extern Node* head;

Value* code_PROGRAM(Node* n);
Value* code_EXTDEFS(Node* n);
Value* code_EXTDEF(Node* n);
Type* code_SPEC(Node* n);
string code_FUN_Name(Node* n);
vector<Type*> code_FUNC_ARG_Type(Node* n);
vector<Type*> code_PARAS_Type(Node* n);
vector<string> code_FUNC_ARG_Name(Node* n);
vector<string> code_PARA_ARG_Name(Node* n);
void code_EXTVARS(Node* n, Type* t);
void code_DECS_INNER(Node* n, Function* f, Type* t);
void code_DEC_INNER(Node* n, Function* f, Type* t);
void code_STMTBLOCK(Node* n, Function* f);
void code_STMT(Node* n, Function* f);
void code_DEF_INNER(Node* n, Function* f);
Value* code_EXP(Node* n);
Value* code_INIT(Node* n);
void code_STMTS(Node* n, Function* f);
/*
Useful functions to DEBUG;
*/

void errorOccur(string msg)
{
	fout.close();
	fout.open("CompileError");
	fout << "Error" << endl;
	fout << msg << endl;
	fout.close();
	cerr << msg;
	exit(0);
}


//using namespace llvm;
typedef map<string, Value*> env;
typedef map<string, Type*> typeEnv;

LLVMContext& context = getGlobalContext();
Module* module;
IRBuilder<> builder(getGlobalContext());
map<string, llvm::Type*> NameTypes;
map<string, llvm::Value*> namedValues;
vector<env> envs;
vector<typeEnv> typeEnvs;
map<string, Function*> fEnv;
map<string, env> funcEnv;
raw_os_ostream out(cout) ;


AllocaInst* CreateEntryBlockAlloca(Function* f,string varName, Type* t)
{
	//f->getEntryBlock().begin();
	//IRBuilder<> tmpB(&(f->getEntryBlock()), f->getEntryBlock().begin());
	//return tmpB.CreateAlloca(t, 0, varName.c_str());
	return NULL;
}

llvm::Value* code_PROGRAM(Node* n)
{
	return code_EXTDEFS(n->child);

}

Value* get_LHS(Node* n)
{
	env now = envs.back();
	if (n->next == NULL)
	{
		return now[string(n->child->content)];
	}
}

vector<Value*> code_ARGS(Node* n)
{
	vector<Value*> ret;
	n = n->child;
	if (n->child == NULL)
		return ret;
	while (true)
	{
		Value* tmp = code_EXP(n);
		ret.push_back(tmp);
		if (n->next == NULL)
			break;
		n = n->next->next->child;
	}
	return ret;
}

Value* code_EXP(Node* n)
{
	string childToken = n->child->token;
	cout << childToken<< endl;
	if (n->child == NULL)
		return NULL;
	cout << "In exp!!" << endl;
	if (childToken == "INT")
	{
		string num = n->child->content;
		int n = stoi(num);
		cout << "In EXP INT ::" << n << endl;
		return ConstantInt::get(Type::getInt32Ty(context), n);
	}
	else if (childToken == "LP")
	{
		return code_EXP(n->child->next);
	}
	else if (childToken == "ID")
	{
		if (n->child->next == NULL)
		{
			env envNow = envs.back();
			Value* v = envNow[string(n->child->content)];
			cout << "In exp ID::" << n->child->content << endl;
			if (v != NULL)
			{
				return builder.CreateLoad(v, n->child->content);
			}
		}
		else if (strcmp("LP",n->child->next->token) == 0)
		{
			string funcName = n->child->content;
			Function* f = module->getFunction(funcName);
			if (f == 0)
				errorOccur("No such function");
			vector<Value* > args = code_ARGS(n->child->next->next);
			return builder.CreateCall(f, args, "calltmp");
		}
	}
	else if (childToken == "EXP")
	{
		env now = envs.back();
		Value* LHS = code_EXP(n->child);
		Value* RHS = code_EXP(n->child->next->next);
		string op = n->child->next->token;
		cout << "I'm in EXP EXP!" << op << n->child->token << n->child->next->next->token << endl;
		if (op == "PLUS")
		{
			cout << "IN EXP EXP PULS::" << endl;
			Value* ret = builder.CreateAdd(LHS, RHS);
			return ret;
		}
		else if (op == "MINUS")
		{
			Value* ret = builder.CreateSub(LHS, RHS);
			return ret;
		}
		else if (op == "TIMES")
		{
			Value* ret = builder.CreateMul(LHS, RHS);
			return ret;
		}
		else if (op == "DIV")
		{
			Value* ret = builder.CreateSDiv(LHS, RHS);
			return ret;
		}
		else if (op == "MOD")
		{
			Value* ret = builder.CreateSRem(LHS, RHS);
			return ret;
		}
		else if (op == "SHLEFT")
		{
			Value* ret = builder.CreateShl(LHS, RHS);
			return ret;
		}
		else if (op == "SHRIGHT")
		{
			Value* ret = builder.CreateLShr(LHS, RHS);
			return ret;
		}
		else if (op == "GT")
		{
			Value* ret = builder.CreateICmpSGT(LHS, RHS);
			return ret;
		}
		else if (op == "GEQ")
		{
			Value* ret = builder.CreateICmpSGE(LHS, RHS);
			return ret;
		}
		else if (op == "LT")
		{
			Value* ret = builder.CreateICmpSLT(LHS, RHS);
			return ret;
		}
		else if (op == "LEQ")
		{
			Value* ret = builder.CreateICmpSLE(LHS, RHS);
			return ret;
		}
		else if (op == "EQU")
		{
			Value* ret = builder.CreateICmpEQ(LHS, RHS);
			return ret;
		}
		else if (op == "NOTEQ")
		{
			Value* ret = builder.CreateICmpNE(LHS, RHS);
			return ret;
		}
		else if (op == "BITAND")
		{
			Value* ret = builder.CreateAnd(LHS, RHS);
			return ret;
		}
		else if (op == "BITXOR")
		{
			return builder.CreateXor(LHS, RHS);
		}
		else if (op == "BITOR")
		{
			return builder.CreateOr(LHS, RHS);
		}
		else if (op == "LOGAND")
		{
			return builder.CreateAnd(LHS, RHS);
		}
		else if (op == "LOGOR")
		{
			return builder.CreateOr(LHS, RHS);
		}
		else
		{
			if (op == "ASSIGNOP")
			{
				Value* LHS = get_LHS(n->child);
				return builder.CreateStore(RHS, LHS);
			}
			else if (op == "BANDAN")
			{
				Value* tv = builder.CreateAnd(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "BXORAN")
			{
				Value* tv = builder.CreateXor(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "BORAN")
			{
				Value* tv = builder.CreateOr(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "SHLEFTAN")
			{
				Value* tv = builder.CreateShl(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "SHRIGHTAN")
			{
				Value* tv = builder.CreateLShr(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "PLUSAN")
			{
				Value* tv = builder.CreateAdd(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "MINUSAN")
			{
				Value* tv = builder.CreateSub(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "TIMESAN")
			{
				Value* tv = builder.CreateMul(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "DIVAN")
			{
				Value* tv = builder.CreateSDiv(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
		}
	}
	else if (childToken == "MINUS")
	{
		Value* v = code_EXP(n->child->next);
		Value* l = get_LHS(n->child->next);
		Value* t = builder.CreateSub( ConstantInt::getSigned(Type::getInt32Ty(context), 0),v);
		return builder.CreateStore(t, l);
	}
	else if (childToken == "INCR")
	{
		Value* v = code_EXP(n->child->next);
		Value* l = get_LHS(n->child->next);
		Value* t = builder.CreateAdd(ConstantInt::getSigned(Type::getInt32Ty(context), 1), v);
		return builder.CreateStore(t, l);
	}
	else if (childToken == "DECR")
	{
		Value* v = code_EXP(n->child->next);
		Value* l = get_LHS(n->child->next);
		Value* t = builder.CreateSub(v, ConstantInt::getSigned(Type::getInt32Ty(context), 1));
		return builder.CreateStore(t, l);
	}
	else if (childToken == "BITNOT")
	{
		Value* v = code_EXP(n->child->next);
		Value* l = get_LHS(n->child->next);
		Value* t = builder.CreateXor(v, ConstantInt::getSigned(Type::getInt32Ty(context), -1));
		return builder.CreateStore(t, l);
	}
	else if (childToken == "LOGNOT")
	{
		Value* v = code_EXP(n->child->next);
		Value* l = get_LHS(n->child->next);
		Value* t = builder.CreateICmpEQ(v, ConstantInt::getSigned(Type::getInt32Ty(context), 0));
		return builder.CreateStore(t, l);
	}

	return NULL;
}

llvm::Value* code_EXTDEFS(Node* n)
{
	if (n->child == NULL)
		return NULL;
	Node* c = n->child;
	Value* ext = code_EXTDEF(c);
	Value* exts = code_EXTDEFS(c->next);
	return NULL;
}

void set_Func_Env(Function* f, vector<string> argsName, vector<Type*> argsType , string name)
{
	BasicBlock *BB = BasicBlock::Create(context, "entry", f);
	builder.SetInsertPoint(BB);
	int i = 0;
	env fenv = funcEnv[name];
	env now = envs.back();
	//AllocaInst* alloca = builder.CreateAlloca(argsType[0], 0, argsName[0].c_str());
	//builder.CreateStore(f->arg_begin(), alloca);
	for (Function::arg_iterator AI = f->arg_begin(); i!= argsType.size(); ++AI,++i)
	{
		AI->print(out);
		AllocaInst* alloca = builder.CreateAlloca(argsType[i], 0, argsName[i].c_str());
		alloca->print(out);
		AI->setName(argsName[i].c_str());
		builder.CreateStore(AI, alloca);
		fenv[argsName[i]] = alloca;
		now[argsName[i]] = alloca;
	}
	funcEnv[name] = fenv;
	envs.push_back(now);
}

llvm::Value* code_EXTDEF(Node* n)
{
	Node* snd = n->child->next;
	string sndContent = snd->token;
	if (sndContent == "FUNC")
	{
		Type* retType = code_SPEC(n->child);
		string funcName = code_FUN_Name(n->child->next);
		std::vector<Type*> argsType = code_FUNC_ARG_Type(n->child->next);
		for (auto& i : argsType)
		{
			//cout << "Herere !";
			//i->print(out);
		}
		std::vector<string> argsName = code_FUNC_ARG_Name(n->child->next);
		FunctionType* funcType = FunctionType::get(retType, argsType, false);
		//funcType->print(out);
		Function* f = Function::Create(funcType, Function::ExternalLinkage, funcName.c_str(), module);
		unsigned idx = 0;
		cout << "Function Def::"<<argsName.size()<<" and "<<argsType.size();
		cout << f->arg_size() << endl;
		for (auto AI = f->arg_begin(); idx != argsName.size();++AI, ++idx)
		{
			//cout << argsName[idx];
			//AI->setName(argsName[idx]);
			//namedValues[argsName[idx]] = &a;
		}
		set_Func_Env(f, argsName, argsType, funcName);
		//cout << endl;
		fEnv[funcName] = f;
		code_STMTBLOCK(n->child->next->next, f);
		envs.pop_back();
	}
	else if (sndContent == "EXTVARS")
	{
		Type* varType = code_SPEC(n->child);
		code_EXTVARS(n->child->next, varType);
	}
	return NULL;
}

llvm::Type* code_SPEC(Node* n)
{
	string chi = n->child->token;
	if (chi == "TYPE")
	{
		return Type::getInt32Ty(context);
	}
}

string code_FUN_Name(Node* n)
{
	return string(n->child->content);
}

vector<Type*> code_FUNC_ARG_Type(Node* n)
{
	return code_PARAS_Type(n->child->next->next);
}

vector<Type*> code_PARAS_Type(Node* n)
{
	vector<Type*> ret;
	if (n->child == NULL)
		return ret;
	//cout <<"IN Para type::"<< n->token << endl << endl;
	while (true)
	{
		Node* para = n->child;
		Type* t = code_SPEC(para->child);
		ret.push_back(t);
		if (para->next == NULL)
			break;
		n = para->next->next;
	}
	return ret;
}

vector<string> code_FUNC_ARG_Name(Node* n)
{
	return code_PARA_ARG_Name(n->child->next->next);
}

vector<string> code_PARA_ARG_Name(Node* n)
{
	vector<string> ret;
	if (n->child== NULL)
		return ret;
	while (true)
	{
		Node* para = n->child;
		string t = para->child->next->child->content;
		cout << t << endl;
		ret.push_back(t);
		if (para->next == NULL)
			break;
		n = para->next->next;
	}
	return ret;
}

void code_EXTVARS(Node* n,Type* t)
{
	if (n == NULL)
	{
		return;
	}
	Node* dec = n->child;
	string tmp = dec->child->child->token;
	if (tmp== "ID")
	{
		string name = dec->child->child->content;
		Value* v = new GlobalVariable(*module, t, false, GlobalValue::ExternalLinkage, NULL);
	}
}

void code_DEFS_INNER(Node* n, Function* f)
{
	if (n->child == NULL)
		return;
	//cout << "DEFS::" << n->token << " " << n->content << endl;
	n = n->child;
	while (n != NULL)
	{
		code_DEF_INNER(n, f);
		n = n->next->child;
	}
	return;
}

void code_DEF_INNER(Node* n, Function* f)
{
	//cout << "DEF::" << n->token << " " << n->content << endl;
	Type* t = code_SPEC(n->child);
	code_DECS_INNER(n->child->next, f, t);
}

void code_DECS_INNER(Node* n, Function* f, Type* t)
{
	//cout << "DECS::" << n->token << endl;
	n = n->child;
	while (n != NULL)
	{
		code_DEC_INNER(n, f, t);
		if (n->next != NULL)
			n = n->next->next->child;
		else
			break;
	}
}


void code_DEC_INNER(Node* n, Function* f, Type* t)
{
	string name = n->child->child->content;
	cout << n->child->token << " " << n->child->child->content<<endl;
	AllocaInst* inst = builder.CreateAlloca(t,NULL,n->child->child->content);
	env now = envs.back();
	now[string(n->child->child->content)] = inst;
	envs.pop_back();
	envs.push_back(now);
	cout << "In DEC_INNER"<<envs.size() <<" "<<n->child->child->content <<now[string(n->child->child->content)]<< envs.back()[string(n->child->child->content)]<<endl;
	if (n->child->next == NULL)
	{
		return;
	}
	else
	{
		cout << n->child->next->next->token << endl;
		Value* v = code_INIT(n->child->next->next);
		builder.CreateStore(v, inst);
	}
}

Value* code_INIT(Node* n)
{
	string token = n->child->token;
	cout << "INIT::" << n->child->token << endl;
	if (token == "EXP")
	{
		return code_EXP(n->child);
	}
	return NULL;
}

void code_STMTBLOCK(Node* n, Function* f)
{
	code_DEFS_INNER(n->child->next, f);
	code_STMTS(n->child->next->next,f);
}

void code_STMTS(Node* n, Function* f)
{
	if (n->child == NULL)
		return;
	code_STMT(n->child,f);
	code_STMTS(n->child->next,f );
}

void code_STMT(Node* n,Function* f)
{
	string tmp_token = n->child->token;
	cout << tmp_token << endl;
	if (tmp_token == "STMTBLOCK")
	{
		code_STMTBLOCK(n->child, f);
	}
	else if (tmp_token == "RETURN")
	{
		Value* ret = code_EXP(n->child->next);
		builder.CreateRet(ret);
	}
	else if (tmp_token == "EXP")
	{
		cout << "I'm in STMT EXP!!" << endl;
		Value* v = code_EXP(n->child);
	}
}



int main(int argc, char* argv[])
{
    Node* head;
    fout.open("out.ll");
    if (argc == 2)
    {
        head = parse(argv[1]);
    }
	walkThrough(head, 1);
    std::cout<<"Hhahaha";
   // context = getGlobalContext();
    module = new Module("Simple C", context);
	FunctionType *FT = FunctionType::get(Type::getDoubleTy(context), false);
	Function* F = Function::Create(FT, Function::ExternalLinkage, "MABI", module);
	env globalEnv;
	envs.push_back(globalEnv);
	code_PROGRAM(head);
    module->dump();
}
