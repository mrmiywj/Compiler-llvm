#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "datastruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_os_ostream.h"
#include <llvm/Support/SourceMgr.h>
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include <llvm/Support/MemoryBuffer.h>
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/DynamicLibrary.h>
#include "llvm/Support/Debug.h"
#include <llvm/IRReader/IRReader.h>
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"
#include "llvm/Transforms/Utils/UnifyFunctionExitNodes.h"
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


bool elseFlag = true;
bool thenFlag = true;
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
int temp;

string getTemp()
{
  return to_string(temp++);
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
env nowEnv;
raw_os_ostream out(cout) ;
BasicBlock* breakBlock;
BasicBlock* contBlock;
env globalEnv;
ExecutionEngine* ee;
bool isLeft = false;
bool retFlag = false;
bool contbreakFlag = false;
map<string, Type*> structEnv;
map<string, string > idstEnv;
map<string, map<string, int> > stField;
string structName;
void remove_multi_termi(Function* f)
{
	for (auto b = f->begin(); b != f->end(); ++b)
	{
		bool flag = false;
		for (auto i=b->begin(); i!= b->end(); ++i)
		{
			if (i->isTerminator())
			{
				if (flag)
					i->eraseFromParent();
				else
					flag = true;
			}
		}
	}
}


AllocaInst* CreateEntryBlockAlloca(Function* f,string varName, Type* t)
{
	f->getEntryBlock().begin();
	IRBuilder<> tmpB(&(f->getEntryBlock()), f->getEntryBlock().begin());
	return tmpB.CreateAlloca(t, 0, varName.c_str());
	return NULL;
}

Function* createPrintfFunction()
{
  vector<Type*> printf_arg_types;
  printf_arg_types.push_back(Type::getInt8PtrTy(context));

  FunctionType* printf_type = FunctionType::get(Type::getInt32Ty(context), printf_arg_types, true);

  Function* func = Function::Create(printf_type, Function::ExternalLinkage, Twine("printf"),module);

  func->setCallingConv(CallingConv::C);
  return func;
}



Function* createWriteFunction(Function* printFn)
{
  vector<Type*> echo_arg_types;
  echo_arg_types.push_back(Type::getInt32Ty(context));

  FunctionType* echo_type = FunctionType::get(Type::getVoidTy(context), echo_arg_types, false);

  Function* func = Function::Create(echo_type, Function::InternalLinkage,Twine("write"), module);

  BasicBlock* bblock = BasicBlock::Create(context, "entry", func);

  const char* constValue = "%d\n";
  Constant* format_const = ConstantDataArray::getString(context, constValue);

  GlobalVariable* var = new GlobalVariable(*module, ArrayType::get(IntegerType::get(context,8), strlen(constValue) + 1), true, GlobalValue::PrivateLinkage, format_const, ".str");
  Constant* zero = Constant::getNullValue(IntegerType::getInt32Ty(context));
  vector<Constant*> indices;

  indices.push_back(zero);
  indices.push_back(zero);
  Constant* var_ref = ConstantExpr::getGetElementPtr(ArrayType::get(IntegerType::get(context, 8), strlen(constValue) + 1),var, indices);

  vector<Value*> args;
  args.push_back(var_ref);

  Function::arg_iterator argsValues = func->arg_begin();
  Value* toPrint = argsValues++;
  toPrint->setName("toPrint");
  args.push_back(toPrint);

  CallInst::Create(printFn, args, "",bblock);
  ReturnInst::Create(context, bblock);
  return func;
}

llvm::Function* createScanfFunction()
{
    std::vector<llvm::Type*> scanf_arg_types;
    scanf_arg_types.push_back(llvm::Type::getInt8PtrTy(getGlobalContext())); //char*
    
    llvm::FunctionType* scanf_type =
    llvm::FunctionType::get(
                            llvm::Type::getInt32Ty(getGlobalContext()), scanf_arg_types, true);
    
    llvm::Function *func = llvm::Function::Create(
                                                  scanf_type, llvm::Function::ExternalLinkage,
                                                  llvm::Twine("scanf"),
                                                  module
                                                  );
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

Function* createReadFunction(llvm::Function* scanfFn)
{
    std::vector<llvm::Type*> read_arg_types; //no arg
    read_arg_types.push_back(llvm::Type::getInt32PtrTy(getGlobalContext()));
    
    
    llvm::FunctionType* read_type =
    llvm::FunctionType::get(
                            llvm::Type::getVoidTy(getGlobalContext()), read_arg_types, false);
    
    llvm::Function *func = llvm::Function::Create(
                                                  read_type, llvm::Function::InternalLinkage,
                                                  llvm::Twine("read"),
                                                  module
                                                  );
    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(getGlobalContext(), "entry", func, 0);

    
    const char *constValue = "%d";
    llvm::Constant *format_const = llvm::ConstantDataArray::getString(getGlobalContext(), constValue);
    llvm::GlobalVariable *var =
    new llvm::GlobalVariable(
                             *module, llvm::ArrayType::get(llvm::IntegerType::get(getGlobalContext(), 8), strlen(constValue)+1),
                             true, llvm::GlobalValue::PrivateLinkage, format_const, ".str");
    llvm::Constant *zero =
    llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(getGlobalContext()));
    
    std::vector<llvm::Constant*> indices;
    indices.push_back(zero);
    indices.push_back(zero);
    llvm::Constant *var_ref =
      llvm::ConstantExpr::getGetElementPtr(ArrayType::get(IntegerType::get(context, 8), strlen(constValue) +1),var, indices);
    
    std::vector<Value*> args;
    args.push_back(var_ref);
    args.push_back(func->arg_begin());
    
    CallInst::Create(scanfFn, makeArrayRef(args), "", bblock);
    ReturnInst::Create(getGlobalContext(), bblock);
    
    return func;
}

llvm::Value* code_PROGRAM(Node* n)
{

	return code_EXTDEFS(n->child);

}

Value* get_LHS(Node* n)
{
  //  cout<<"I'm in get_LHS"<<endl;
  if (n->child->next == NULL)
    return nowEnv[string(n->child->content)];
  else
    {
      //cout<<"Hey I'm here, get array element as lsh"<<endl;
      vector<Value*> vecind;
      if (strcmp("ARRS", n->child->next->token) == 0)
        {
          string name = n->child->content;
          Node* arrs = n->child->next;
          // while (arrs != NULL)
          //{
          //  if (arrs->child == NULL)
          //    break;
          //  Node* arr = arrs->child->next;
          //  Value* v = code_EXP(arr->next);
          //  vecind.push_back(v);
          //  arrs = arrs->child->next->next->next;
          //}
          vecind.push_back(ConstantInt::get(Type::getInt32Ty(context),0));
          Value* v = code_EXP(arrs->child->next);
          vecind.push_back(v);
          ArrayRef<Value*> inds(vecind);
          Value* loadinst = builder.CreateInBoundsGEP(nowEnv[name], inds);
          //loadinst->print(out);
          return loadinst;
        }
      else if (strcmp("DOT", n->child->next->token) == 0)
        {
          string name = n->child->child->content;
          string field = n->child->next->next->content;
          int offset = stField[idstEnv[name]][field];
          vecind.push_back(ConstantInt::get(Type::getInt32Ty(context),0));
          vecind.push_back(ConstantInt::get(Type::getInt32Ty(context),offset));
          ArrayRef<Value*> inds(vecind);
          Value* loadinst = builder.CreateInBoundsGEP(nowEnv[name], inds);
          //loadinst->print(out);
          //cout<<"Get struct element as left succ!"<<endl;
          return loadinst;
        }
    }
  return NULL;
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
	//cout << childToken<< endl;
	if (n->child == NULL)
		return NULL;
	//cout << "In exp!!" << endl;
	if (childToken == "INT")
    {
      char* p;
      int num = strtol(n->child->content, &p, 0);
      //string num = n->child->content;
      //int n = stoi(num);
      //cout << "In EXP INT ::" << num << endl;
		return ConstantInt::get(Type::getInt32Ty(context), num);
	}
	else if (childToken == "LP")
	{
		return code_EXP(n->child->next);
	}
	else if (childToken == "ID")
	{
		if (n->child->next == NULL)
		{
			//env envNow = envs.back();
			Value* v = nowEnv[string(n->child->content)];
			//cout << "In exp ID::" << n->child->content << endl;
			if (v != NULL)
			{
				return builder.CreateLoad(v, n->child->content);
			}
      else
        {
          errorOccur("No such identifier called" + string(n->child->content));
        }
		}
		else if (strcmp("LP",n->child->next->token) == 0)
		{
			string funcName = n->child->content;
			Function* f = module->getFunction(funcName);
			if (f == 0)
				errorOccur("No such function called " + funcName);
			vector<Value* > args = code_ARGS(n->child->next->next);
      if (funcName == "write")
        {
          //for (auto &v:args)
            //v->print(out);
          return builder.CreateCall(f,args); 
        }
      else  if (funcName == "read")
        {
          //cout<<"Read value:"<<n->child->next->next->child->token<<endl;
          Value* v = get_LHS(n->child->next->next->child);
          ArrayRef<Value*> arg(v);
          return builder.CreateCall(f, arg);
        }
      else
        return builder.CreateCall(f, args, "calltmp");
		}
    else if (strcmp("ARRS", n->child->next->token) == 0)
      {
        //cout<<"I'm ID Array::"<<endl;
        vector<Value*> vecindex;
        Node* arrs = n->child->next;
        vecindex.push_back(ConstantInt::get(Type::getInt32Ty(context),0));
        //while (true)
        // {
        //   if (arrs->child == NULL)
        //     break;
            Node* e = arrs->child->next;
            Value* v = code_EXP(e);
            // cout<<"I'm in array index!::"<<endl;
            //v->print(out);
            vecindex.push_back(v);
            //arrs = arrs->child->next->next->next;
            // }
        ArrayRef<Value*> inds(vecindex);
        //      for (auto i:inds)
        //          i->print(out);
        string name = n->child->content;
        //cout<<"Array name::"<<name<<endl;
        Value* pt = nowEnv[name];
       
        if (pt)
          {
        
          }
          //cout<<"I found array ptr"<<endl;
        else
          {
            //cout<<"I cant find array ptr"<<endl;
            errorOccur("Can't find array named "+ name);
            //exit(-1);
          }
        //pt->print(out);
        Value* eleptr=  builder.CreateInBoundsGEP(pt, inds);
        //   if (isLeft)
        //     return eleptr;
        //    else
        return builder.CreateLoad(eleptr);
      }
	}
	else if (childToken == "EXP")
    {
      if (strcmp(n->child->next->token, "DOT")==0)
        {
          string name = n->child->child->content;
          string field = n->child->next->next->content;
          int offset = stField[idstEnv[name]][field];
          vector<Value*> vecind;
          vecind.push_back(ConstantInt::get(Type::getInt32Ty(context),0));
          vecind.push_back(ConstantInt::get(Type::getInt32Ty(context),offset));
          ArrayRef<Value*> inds(vecind);
          Value* loadinst = builder.CreateInBoundsGEP(nowEnv[name], inds);
          //loadinst->print(out);
          //cout<<"Get struct element as left succ!"<<endl;
          return builder.CreateLoad(loadinst);
        }
		//env now = envs.back();
		string op = n->child->next->token;
		//cout << "I'm in EXP EXP!" << op << n->child->token << n->child->next->next->token << endl;
		if (op == "PLUS")
      {		Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
        //cout << "IN EXP EXP PULS::" << endl;
			Value* ret = builder.CreateAdd(LHS, RHS);
			return ret;
		}
		else if (op == "MINUS")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateSub(LHS, RHS);
			return ret;
		}
		else if (op == "TIMES")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateMul(LHS, RHS);
			return ret;
		}
		else if (op == "DIV")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateSDiv(LHS, RHS);
			return ret;
		}
		else if (op == "MOD")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateSRem(LHS, RHS);
			return ret;
		}
		else if (op == "SHLEFT")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateShl(LHS, RHS);
			return ret;
		}
		else if (op == "SHRIGHT")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateLShr(LHS, RHS);
			return ret;
		}
		else if (op == "GT")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateICmpSGT(LHS, RHS);
			return ret;
		}
		else if (op == "GEQ")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateICmpSGE(LHS, RHS);
			return ret;
		}
		else if (op == "LT")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateICmpSLT(LHS, RHS);
			return ret;
		}
		else if (op == "LEQ")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateICmpSLE(LHS, RHS);
			return ret;
		}
		else if (op == "EQU")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateICmpEQ(LHS, RHS);
			return ret;
		}
		else if (op == "NOTEQ")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateICmpNE(LHS, RHS);
			return ret;
		}
		else if (op == "BITAND")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			Value* ret = builder.CreateAnd(LHS, RHS);
			return ret;
		}
		else if (op == "BITXOR")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			return builder.CreateXor(LHS, RHS);
		}
		else if (op == "BITOR")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			return builder.CreateOr(LHS, RHS);
		}
		else if (op == "LOGAND")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			return builder.CreateAnd(LHS, RHS);
		}
		else if (op == "LOGOR")
      {
        Value* LHS = code_EXP(n->child);
        Value* RHS = code_EXP(n->child->next->next);
			return builder.CreateOr(LHS, RHS);
		}
		else
		{
			if (op == "ASSIGNOP")
        {
          Value* RHS = code_EXP(n->child->next->next);
				Value* LHS = get_LHS(n->child);
				return builder.CreateStore(RHS, LHS);
			}
			else if (op == "BANDAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateAnd(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "BXORAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateXor(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "BORAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateOr(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "SHLEFTAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateShl(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "SHRIGHTAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateLShr(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "PLUSAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
          //cout << "In PLUSAN!!" << endl;
				Value* tv = builder.CreateAdd(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "MINUSAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateSub(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "TIMESAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateMul(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
			else if (op == "DIVAN")
        {
          Value* LHS = code_EXP(n->child);
          Value* RHS = code_EXP(n->child->next->next);
				Value* tv = builder.CreateSDiv(LHS, RHS);
				Value* l = get_LHS(n->child);
				return builder.CreateStore(tv, l);
			}
		}
	}
	else if (childToken == "MINUS")
    {
      // Value* RHS = code_EXP(n->child->next->next);
		Value* v = code_EXP(n->child->next);
		Value* t = builder.CreateSub( ConstantInt::getSigned(Type::getInt32Ty(context), 0),v);
		return t;
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
    //cout<<"I'm in DECR::"<<endl;
		Value* t = builder.CreateSub(v, ConstantInt::getSigned(Type::getInt32Ty(context), 1));
    //cout<<"CreateSub succ!"<<endl;
    Value* ret = builder.CreateStore(t,l);
    return ret;
		//return builder.CreateStore(t, l);
	}
	else if (childToken == "BITNOT")
	{
		Value* v = code_EXP(n->child->next);
		//Value* l = get_LHS(n->child->next);
    return builder.CreateXor(v, ConstantInt::getSigned(Type::getInt32Ty(context), -1));
		//return builder.CreateStore(t, l);
	}
	else if (childToken == "LOGNOT")
    {
      //cout<<"Yep I'm in LOGNOT!!"<<endl;
		Value* v = code_EXP(n->child->next);
		//Value* l = get_LHS(n->child->next);
    return builder.CreateICmpEQ(v, ConstantInt::getSigned(Type::getInt32Ty(context),0));
    //   	Value* t = builder.CreateICmpEQ(v, ConstantInt::getSigned(Type::getInt32Ty(context), 0));
    //	return builder.CreateStore(t, l);
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
	//env fenv = funcEnv[name];
	//env now = envs.back();
	//AllocaInst* alloca = builder.CreateAlloca(argsType[0], 0, argsName[0].c_str());
  nowEnv = globalEnv;
	//builder.CreateStore(f->arg_begin(), alloca);
	for (Function::arg_iterator AI = f->arg_begin(); i!= argsType.size(); ++AI,++i)
	{
		//AI->print(out);
		AllocaInst* alloca = builder.CreateAlloca(argsType[i], 0, argsName[i].c_str());
		//alloca->print(out);
		AI->setName(argsName[i].c_str());
		builder.CreateStore(AI, alloca);
		//fenv[argsName[i]] = alloca;
		nowEnv[argsName[i]] = alloca;
	}
	//funcEnv[name] = fenv;
	//envs.push_back(now);
}

llvm::Value* code_EXTDEF(Node* n)
{
	Node* snd = n->child->next;
	string sndContent = snd->token;
	if (sndContent == "FUNC")
	{
		Type* retType = code_SPEC(n->child);
		string funcName = code_FUN_Name(n->child->next);
    if (fEnv.find(funcName) != fEnv.end())
      {
        errorOccur("Function " + funcName+" has been declared before!");
      }
		std::vector<Type*> argsType = code_FUNC_ARG_Type(n->child->next);
		//for (auto& i : argsType)
		//{
			//cout << "Herere !";
			//i->print(out);
		//}
		std::vector<string> argsName = code_FUNC_ARG_Name(n->child->next);
		FunctionType* funcType = FunctionType::get(retType, argsType, false);
		//funcType->print(out);
		Function* f = Function::Create(funcType, Function::ExternalLinkage, funcName.c_str(), module);
		unsigned idx = 0;
		//cout << "Function Def::"<<argsName.size()<<" and "<<argsType.size();
		//cout << f->arg_size() << endl;
		//for (auto AI = f->arg_begin(); idx != argsName.size();++AI, ++idx)
		//{
			//cout << argsName[idx];
			//AI->setName(argsName[idx]);
			//namedValues[argsName[idx]] = &a;
		//}
		set_Func_Env(f, argsName, argsType, funcName);
		//cout << endl;
		fEnv[funcName] = f;
		code_STMTBLOCK(n->child->next->next, f);
		//envs.pop_back();
	}
	else if (sndContent == "EXTVARS")
	{
		Type* varType = code_SPEC(n->child);
		code_EXTVARS(n->child->next, varType);
	}
	return NULL;
}

int get_DEFS_size(Node* n)
{
  int res = 0;
  while(n->child != NULL)
    {
      Node* def = n->child;
      res++;
      n = def->next;
    }
  return res;
}

vector<string> get_DEFS_names (Node* n)
{
  vector<string> res;
  while(n->child!= NULL)
    {
      Node* def = n->child;
      Node* decs = def->child->next;
      while (true)
        {
          Node* dec = decs->child;
          string name = dec->child->child->content;
          res.push_back(name);
          if (dec->next == NULL)
            break;
          else
            {
              decs = dec->next->next;
            }
        }
      n = def->next;
    }
  return res;
}



llvm::Type* code_SPEC(Node* n)
{
	string chi = n->child->token;
	if (chi == "TYPE")
	{
		return Type::getInt32Ty(context);
	}
  else
    {
      Node* stspec = n->child;
      if (stspec->child->next->next== NULL)
        {
          Type* t = structEnv[stspec->child->next->content];
          structName = stspec->child->next->content;
          if (t)
            return t;
          else
            {
              errorOccur("No such struct");
            }
        }
      else
        {
          bool flag = false;
          Node* opttag = stspec->child->next;
          string name;
          if (opttag->child != NULL)
            {
              name = opttag->child->content;
              flag = true;
            }
          else
            {
              name = getTemp();
            }
          Node* defNode = stspec->child->next->next->next;
          vector<string> names = get_DEFS_names(defNode);
          int num = get_DEFS_size(defNode);
          //cout<<"A struct with "<<num<<" elements"<<endl;
          for(auto i:names)
            {
              cout<<i<<"  ";
            }
          cout<<endl;
          vector<Type*> elementsType;
          for (int i = 0; i < num; i++)
            {
              // stField[name][names[i]] = i;
              elementsType.push_back(Type::getInt32Ty(context));
            }
          StructType* t = StructType::create(context, elementsType, name.c_str());
          structEnv[name] = t;
          structName = name;
          return t;
        }
    }
  return NULL;
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
		//cout << t << endl;
		ret.push_back(t);
		if (para->next == NULL)
			break;
		n = para->next->next;
	}
	return ret;
}

Type* code_VAR_ARRAY_TYPE(Node* n)
{
	string token = n->child->token;
	if (token == "ID")
		return Type::getInt32Ty(context);
	if (n->child->next == NULL)
		return Type::getInt32Ty(context);
	int num = atoi(n->child->next->next->content);
	cout<<"In var_array_type::"<<num<<endl;
	 Type* eleType = code_VAR_ARRAY_TYPE(n->child);
	return ArrayType::get(eleType, num);
	//return NULL;

}

string get_VAR_Name(Node* n)
{
	string childToken = n->child->token;
	n = n->child;
	while (childToken != "ID")
	{
		childToken = n->child->token;
		n = n->child;
	}
	return n->content;
}

vector<int> get_INIT(Node* n, int len)
{
	vector<int> ret;
	if (n->child->next == NULL)
	{
		errorOccur("Array must initilized wtih LC");
	}
	Node* args = n->child->next;
	while (true)
	{
		int v = atoi(args->child->child->content);
		ret.push_back(v);
		if (args->child->next == NULL)
			break;
		else
		{
			args = args->child->next->next;
		}
	}
	if (ret.size() == 1 && len != 1)
	{
		int v = ret.front();
		for (int i = 0; i < len - 1; ++i)
			ret.push_back(v);
	}
	return ret;
	
}

void code_DEC_GLO(Node* n, Type* t)
{
	if (n->child->child->next == NULL)
    {
      if (t->isStructTy())
        {
          string name = n->child->child->content;
          if (nowEnv.find(name) != nowEnv.end())
            {
              errorOccur("The identifier "+name+" has been declared before");
            }
          idstEnv[name] = structName;
          Constant* c = ConstantAggregateZero::get(t);
          Value* v = new GlobalVariable(*module, t, false, GlobalValue::ExternalLinkage, c,name);
          globalEnv[name] = v;
        }
      else
        {
          string name = n->child->child->content;
          if (nowEnv.find(name) != nowEnv.end())
            {
              errorOccur("The identifier "+name+" has been declared before");
            }
          if (n->child->next == NULL)
            {
              APInt i(32,0 ,true);
              Constant* c  = Constant::getIntegerValue(t,i);
              Value* v = new GlobalVariable(*module, t, false, GlobalValue::ExternalLinkage, c, name);
              globalEnv[name] = v;
            }
          else
            {
              //cout << "I'm in global def with init" << endl;
              string initToken = n->child->next->next->child->token;
              if (initToken == "EXP")
                {
                  int init = atoi(n->child->next->next->child->child->content);
                  //cout << "In global init::" << init << endl;
                  APInt i(32, init, true);
                  Constant* c = Constant::getIntegerValue(t, i);
                  //c->print(out);
                  Value* v = new GlobalVariable(*module, t, false, GlobalValue::ExternalLinkage, c, name);
                  globalEnv[name] = v;
                }
            }
        }
    }
	else
	{
		if (n->child->next == NULL)
		{
			//cout << "I want to declare a global array!" << endl;
			Type* t = code_VAR_ARRAY_TYPE(n->child);
			//t->print(out);
			string name = get_VAR_Name(n);
      if (nowEnv.find(name) != nowEnv.end())
        {
          errorOccur("The identifier "+name+" has been declared before");
        }
      int len = atoi(n->child->child->next->next->content);
      vector<Constant*> cv;
      for (int i = 0; i < len; i++)
        {
          APInt init(32,0,true);
          Constant* c = Constant::getIntegerValue(Type::getInt32Ty(context), init);
          cv.push_back(c);
        }
      ArrayRef<Constant*> ar(cv);
      Constant* conarr = ConstantArray::get(cast<ArrayType>(t), ar);      
			Value* v = new GlobalVariable(*module, t, false, GlobalValue::ExternalLinkage, conarr, name);
      globalEnv[name] = v;
		}
		else
		{
			cout<<"I want to declare a global array with init!"<<endl;
			Type* t = code_VAR_ARRAY_TYPE(n->child);
			string name = get_VAR_Name(n);
      if (nowEnv.find(name) != nowEnv.end())
        {
          errorOccur("The identifier "+name+" has been declared before");
        }
			int len = atoi(n->child->child->next->next->content);
			vector<int> initialvalue = get_INIT(n->child->next->next, len);
			vector<Constant*> cv;
			for (auto i : initialvalue)
			{
				APInt  init(32, i, true);
				Constant* c = Constant::getIntegerValue(Type::getInt32Ty(context), init);
				cv.push_back(c);
			}
			ArrayRef<Constant*> ar(cv);
			Constant* conarr = ConstantArray::get(cast<ArrayType>(t), ar);
			//conarr->print(out);
			Value* v = new GlobalVariable(*module, t, false, GlobalValue::ExternalLinkage, conarr, name);
      globalEnv[name] = v;
		}
	}
	//builder.CreateLoad(val, v);
}

void code_EXTVARS(Node* n,Type* t)
{
	if (n == NULL)
	{
		return;
	}
	n = n->child;
	//string tmp = dec->child->child->token;
	while (true)
    {
      if (n == NULL)
        break;
		code_DEC_GLO(n,t);
		if (n->next == NULL)
			break;
		n = n->next->next->child;
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
  if (n->child->child->next == NULL)
    {
      if(t->isStructTy())
        {
          string name = n->child->child->content;
          auto it = nowEnv.find(name);
          if (it != nowEnv.end())
            {
              errorOccur("You have declared " + name+" before");
            }
          AllocaInst* inst = builder.CreateAlloca(t,NULL,n->child->child->content);
          nowEnv[name] = inst;
          idstEnv[name] = structName;
        }
      else
        {
          string name = n->child->child->content;
          auto it = nowEnv.find(name);
          if (it != nowEnv.end())
            {
              errorOccur("You have declared " + name+" before");
            }
          //cout << n->child->token << " " << n->child->child->content<<endl;
          AllocaInst* inst = builder.CreateAlloca(t,NULL,n->child->child->content);
          //env now = envs.back();
          nowEnv[string(n->child->child->content)] = inst;
          if (nowEnv[string(n->child->child->content)])
            //cout << "Insert succesully"<<n->child->child->content;
          //envs.pop_back();
          //envs.push_back(now);
          //cout << "In DEC_INNER"<<envs.size() <<" "<<n->child->child->content <<now[string(n->child->child->content)]<< envs.back()[string(n->child->child->content)]<<endl;
            if (n->child->next == NULL)
              {
                return; 
              }
          else
            {
              Node* init = n->child->next->next;
              if (strcmp("EXP", init->child->token) == 0)
                {
                  //cout << n->child->next->next->token << endl;
                  Value* v = code_INIT(n->child->next->next);
                  builder.CreateStore(v, inst); 
                }
            }    
        }
      
    }
  else
    {
      int arrlen = atoi(n->child->child->next->next->content);
      string name = n->child->child->child->content;
      auto it = nowEnv.find(name);
      if (it != nowEnv.end())
        {
          errorOccur("You have declared " + name+" before");
        }
      //cout<<"I'm allocating array::"<<name<<endl;
      AllocaInst* inst = builder.CreateAlloca(ArrayType::get(t,arrlen),NULL, n->child->child->child->content);
      //inst->print(out);
      nowEnv[name] = inst;
      if (nowEnv[name])
        //cout<<"Alloca array successfully"<<endl;
      if (n->child->next == NULL)
        return;
      else
        {
          int num = atoi(n->child->child->next->next->content);
          vector<int> vc = get_INIT(n->child->next->next,num);
          int index = 0;
          /*for (auto& i:vc)
            {
              cout<<"I'm declare inner array"<<endl;
              cout<<i<<endl;
              }*/
          for (auto& i: vc)
            {
              APInt tmp(32,i);
              Constant* a = ConstantInt::get(Type::getInt32Ty(context), tmp);
              vector<Value*> inds;
              inds.push_back(ConstantInt::get(Type::getInt32Ty(context), 0));
              inds.push_back(ConstantInt::get(Type::getInt32Ty(context), index));
              Value* ptr = builder.CreateInBoundsGEP(inst, inds);
              Value* s = builder.CreateStore(a,ptr);
              ++index;
              }
        }
    }

	/*string name = n->child->child->content;
	cout << n->child->token << " " << n->child->child->content<<endl;
	AllocaInst* inst = builder.CreateAlloca(t,NULL,n->child->child->content);
	//env now = envs.back();
	nowEnv[string(n->child->child->content)] = inst;
	if (nowEnv[string(n->child->child->content)])
		cout << "Insert succesully"<<n->child->child->content;
	//envs.pop_back();
	//envs.push_back(now);
	//cout << "In DEC_INNER"<<envs.size() <<" "<<n->child->child->content <<now[string(n->child->child->content)]<< envs.back()[string(n->child->child->content)]<<endl;
	if (n->child->next == NULL)
	{
		return;
	}
	else
    {
      Node* init = n->child->next->next;
      if (strcmp("EXP", init->child->token))
        {
          cout << n->child->next->next->token << endl;
          Value* v = code_INIT(n->child->next->next);
          builder.CreateStore(v, inst); 
        }
      else
        {
          int num = atoi(n->child->child->next->next->content);
          vector<int> vc = get_INIT(n->child->next->next,num);
          int index = 0;
          for (auto& i: vc)
            {
              APInt tmp(32,i);
              Constant* a = ConstantInt::get(Type::getInt32Ty(context), tmp);
              vector<Value*> inds;
              inds.push_back(ConstantInt::get(Type::getInt32Ty(context), 0));
              inds.push_back(ConstantInt::get(Type::getInt32Ty(context), index));
              Value* ptr = builder.CreateInBoundsGEP(inst, inds);
              Value* s = builder.CreateStore(a,ptr);
              ++index;
            }
        }
        }*/
}

Value* code_INIT(Node* n)
{
	string token = n->child->token;
	//cout << "INIT::" << n->child->token << endl;
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
	//cout << tmp_token << endl;
	if (tmp_token == "STMTBLOCK")
	{
		code_STMTBLOCK(n->child, f);
	}
	else if (tmp_token == "RETURN")
	{
		Value* ret = code_EXP(n->child->next);
		builder.CreateRet(ret);
    retFlag = true;
    elseFlag = false;
    thenFlag = false;
	}
	else if (tmp_token == "EXP")
	{
		//cout << "I'm in STMT EXP!!" << endl;
		Value* v = code_EXP(n->child);
	}
	else if (tmp_token == "IF")
    {
      cout<<"I'm going to codegen IF"<<endl;
      bool tempThen = thenFlag;
      bool tempElse = elseFlag;
      thenFlag = true;
      elseFlag = true;
      bool tempRet = retFlag;
      retFlag = false;
      bool tempContBreak = contbreakFlag;
      contbreakFlag = false;
		Value* condV = code_EXP(n->child->next->next);
		Function* f = builder.GetInsertBlock()->getParent();
		BasicBlock *thenBB = BasicBlock::Create(context, "then", f);
		BasicBlock *elseBB = BasicBlock::Create(context, "else");
		BasicBlock *mergeBB = BasicBlock::Create(context, "ifcont");
		breakBlock = mergeBB;
    if (condV->getType() == Type::getInt32Ty(context))
      condV = builder.CreateICmpNE(condV,ConstantInt::get(Type::getInt32Ty(context), 0, true));
		builder.CreateCondBr(condV, thenBB, elseBB);
		bool flag = (n->child->next->next->next->next->next->child != NULL);
		builder.SetInsertPoint(thenBB);
		code_STMT(n->child->next->next->next->next, f);
    bool mergeFlag = false;
    if (retFlag)
      thenFlag = false;
    if (contbreakFlag)
      thenFlag = false;
    if (thenFlag)
      {
        builder.CreateBr(mergeBB);
        mergeFlag = true;
      }
    retFlag = false;
    thenBB = builder.GetInsertBlock();
		f->getBasicBlockList().push_back(elseBB);
		builder.SetInsertPoint(elseBB);
		if (flag)
		{
			Node* estmt = n->child->next->next->next->next->next->child->next;
			code_STMT(estmt,f );
		}
    if (retFlag)
      elseFlag = false;
    if (contbreakFlag)
      elseFlag = false;
    if(elseFlag)
      {
        builder.CreateBr(mergeBB);
        mergeFlag = true;
      }
		elseBB = builder.GetInsertBlock();
    retFlag = false;
    if (elseFlag || thenFlag)
      {
        f->getBasicBlockList().push_back(mergeBB);
        builder.SetInsertPoint(mergeBB); 
      }
    retFlag = tempRet;
    contbreakFlag = tempContBreak;
    thenFlag = tempThen;
    elseFlag = tempElse;
	}
	else if (tmp_token == "FOR")
	{
		Node* cond = n->child->next->next->next->next;
    if (n->child->next->next->child != NULL)
      Value* exp1 = code_EXP(n->child->next->next);
		BasicBlock* condBB = BasicBlock::Create(context, "Cond", f);
		builder.CreateBr(condBB);
		builder.SetInsertPoint(condBB);
		Value* condV = code_EXP(cond);


		BasicBlock* loopBB = BasicBlock::Create(context, "loop", f);
		BasicBlock* stepBB = BasicBlock::Create(context, "step");
		contBlock = stepBB;
		BasicBlock* outLoopBB = BasicBlock::Create(context, "outloop");
		breakBlock = outLoopBB;
		//buidler.CreateBr(condBB);
    if (condV->getType() == Type::getInt32Ty(context))
      condV = builder.CreateICmpNE(condV,ConstantInt::get(Type::getInt32Ty(context), 0, true));
		builder.CreateCondBr(condV, loopBB, outLoopBB);
		Node* body = n->child->next->next->next->next->next->next->next->next;
		builder.SetInsertPoint(loopBB);
		code_STMT(body,f );
		builder.CreateBr(stepBB);
		builder.SetInsertPoint(stepBB);
		Node* step = n->child->next->next->next->next->next->next;
		Value* stepValue = code_EXP(step);
		builder.CreateBr(condBB);
		f->getBasicBlockList().push_back(stepBB);
		f->getBasicBlockList().push_back(outLoopBB);
		builder.SetInsertPoint(outLoopBB);
		verifyFunction(*f);
	}
	else if (tmp_token == "CONT")
    {
      contbreakFlag = true;
		if (contBlock != NULL)
		{
			builder.CreateBr(contBlock);
		}
	}
	else if (tmp_token == "BREAK")
    {
      contbreakFlag = true;
		if (breakBlock != NULL)
		{
			builder.CreateBr(breakBlock);
		}
	}
}

RTDyldMemoryManager* RTDyldMM = NULL;
using namespace legacy;
int main(int argc, char* argv[])
{
    Node* head;
    fout.open("out.ll");
    if (argc == 2)
    {
        head = parse(argv[1]);
    }
    else
      {
        errorOccur("Please execute like ./prog (your file name)");
      }
	walkThrough(head, 1);
  //    std::cout<<"Hhahaha";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();
  string ErrStr;
  RTDyldMM = new SectionMemoryManager();

    // context = getGlobalContext();
    unique_ptr<Module> Owner = make_unique<Module>("Simple C", context);
    module = Owner.get();
    ee = EngineBuilder(move(Owner))
    .setEngineKind(EngineKind::JIT)
    .setErrorStr(&ErrStr)
    .setVerifyModules(true)
    .setMCJITMemoryManager(std::unique_ptr<RTDyldMemoryManager>(RTDyldMM))
    .setOptLevel(CodeGenOpt::Default)
    .create();
    //ExecutionEngine* ee = EngineBuilder(move(Owner)).create();
    if (ErrStr.length() != 0)
      {
        cerr<<"Create Engine Error!"<<endl<<ErrStr<<endl;
      }
    ee->finalizeObject();
    //exeeng = EngineBuilder(move(Owner)).create();
    //    vector<llvm::Type*> printf_arg_types;
    //    printf_arg_types.push_back(Type::getInt32Ty(context));
    //    FunctionType *printf_type = FunctionType::get(Type::getInt32Ty(context), printf_arg_types,true);
    //    Function* printf_dec = Function::Create(printf_type, Function::ExternalLinkage,Twine("printf"), module);
    Function* printfFunc = createPrintfFunction();
    Function* writeFunc = createWriteFunction(printfFunc);
    Function* scanfFunc = createScanfFunction();
    Function* readFunc = createReadFunction(scanfFunc);
    error_code err;
    raw_fd_ostream f("out.ll", err,sys::fs::OpenFlags::F_RW);
    //envs.push_back(globalEnv);
    code_PROGRAM(head);
    //for (auto f = module->begin(); f != module->end(); ++f)
    //{
    //  remove_multi_termi(f);
    //}
    //Function* mainFunc = module->getFunction("main");
    // if (mainFunc)
    // {
    //   cout<<"Main func find"<<endl;
    //   exit(0);
    // }
    //ArrayRef<GenericValue> mainarg;
    //exeeng->runFunction(mainFunc,mainarg);

    //unit64_t func_addr = ee->getFunctionAddress("main");
    Function* m = ee->FindFunctionNamed("main");
    if (m == NULL)
    {
          cout<<"No main function"<<endl;
    }
    ArrayRef<GenericValue> arg;
    // m->print(out);
    //ee->runFunction(m,arg);
    //delete ee;
    FunctionPassManager* FPM = new FunctionPassManager(module);

    //FPM->add(new DataLayout(*ee->getDataLayout()));
    FPM->add(createBasicAliasAnalysisPass());
    FPM->add(createPromoteMemoryToRegisterPass());
    FPM->add(createInstructionCombiningPass());
    FPM->add(createReassociatePass());
    FPM->add(createGVNPass());
    FPM->add(createCFGSimplificationPass());

    FPM->doInitialization();
    for (auto it=module->begin(); it != module->end(); ++it)
      {
        FPM->run(*it);
      }
    module->dump();
    module->print(f,NULL);
    //llvm_shutdown();
}
