//
// Created by apple on 2018/6/5.
//

#ifndef SIMPLEPASCALCOMPILER_ABSTRACTTREE_H
#define SIMPLEPASCALCOMPILER_ABSTRACTTREE_H
//Author: Ken
//file: AbstractTree.cpp
// Created by Ken on 2018/6/5.
//QAQ

#include <string>
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include "llvm/IR/Value.h"

#include "CodeGenContext.h"



//non-terminal node
class IdNode;
class RoutineNode;
class RoutineHeadNode;
class RoutineBodyNode;
class LabelPartNode;
class ConstExprNode;
class TypeDeclNode;
class TypeDefineNode;
class VarDeclNode;
class RoutineDeclNode;//include procedure and fucntion node
class ConstValueNode;
class SimpleTypeDeclNode;
class ArrayTypeDeclNode;
class RecordTypeDeclNode;
class StmtListNode;
class StmtNode;

//list node
class ConstExprListNode;
class TypeDefineListNode;
class VarDeclListNode;
class RoutineDeclListNode;

typedef std::list<ConstExprNode*> ConstExprList;
typedef std::list<TypeDefineNode*> TypeDefineList;
typedef std::list<VarDeclNode*> VarDeclList;
typedef std::list<RoutineDeclNode*> RoutineDeclList;

namespace AbstractTree {
enum {
    NODE=0,PROGRAM,ID,ROUTINE,ROUTINE_HEAD,LABEL_PART,CONST_EXPR_LIST,CONST_EXPR,CONST_VALUE,TYPE_DEFINE_LIST,TYPE_DEFINITION,TYPE_DECL,SIMPLE_TYPE_DECL,ARRAY_TYPE_DECL,RECORD_TYPE_DECL,STMT_LIST
}NodeType;

    enum class TypeName
        {
            integer, real, character, boolean, error
        };

    class Node {
    public:
        std::list<Node*> child;
        int type = NODE;
        virtual ~Node() {}
        virtual bool addChild() = 0;
        virtual std::string info() = 0;
        virtual llvm::Value *CodeGen(CodeGenContext& context) = 0;

    };
    class SysCon{
    public:
        bool True = true;
        bool False = false;
        int maxint = INT_MAX;
        int minint = INT_MIN;
    };
    enum SysFuncAndProc{
      abs,chr,odd,ord,pred,sqr,sqrt,succ,write,writeln
    };
    enum SysType{
        Integer,Real,Char,Bool
    };


    class ProgramNode:public Node{
    public:

        RoutineNode* routine;

        ProgramNode(){
            this->type = PROGRAM;
        }
        ProgramNode(IdNode* program_head,RoutineNode* routine){
            this->type = PROGRAM;
            this->child.push_back(program_head);
            this->child.push_back(routine);
            this->routine = routine;
        }

        virtual std::string info(){
            return "Program:";
        }
        virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class RoutineNode:public Node{
    public:
        RoutineHeadNode* routineHead;
        RoutineBodyNode* routineBody;

        RoutineNode(){
            this->type = ROUTINE;
        }
        RoutineNode(RoutineHeadNode routine_head,RoutineBodyNode routine_body){
            //routine head
            this->type = ROUTINE;
            // this->child.push_back(routine_head);
            this->child.push_back(routine_body);
            routineBody = routine_body;
            routineHead = routine_head;
        }
        virtual std::string info(){
            return "Routine:";
        }
        virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class RoutineHeadNode : public Node
    {
    public:
        VarDeclListNode* varDeclList;
        virtual llvm::Value* CodeGen(CodeGenContext& context);
    }

    class IdNode: public Node
    {
    public:
        std::string name;
        IdNode(const std::string& name): name(name){};
        virtual llvm::Value* CodeGen(CodeGenContext& context);
    }

    class StmtListNode:public Node{
        StmtListNode():type(STMT_LIST){}

        virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class StmtNode:public Node{
        string ProcCall;
        string argument;
        virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class VarDeclListNode: public Node
    {
    public:
        std::vector<VarDeclNode*> list;
        void insert(VarDeclNode* node)
        {
            list.push_back(node);
        }
        virtual llvm::Value* CodeGen(CodeGenContext& context);
    }; 

    class VarDeclNode: public Node
    {
    public:
        IdNode* name;
        TypeDeclNode* type;
        VarDeclNode* (IdNode* name, TypeDeclNode* type): name(name), type(type)
        {
            //Put Children
        };
        virtual llvm::Value* CodeGen(CodeGenContext& context);

    };

    class TypeDeclNode: public Node
    {
    public:
        std::string rawName = "";
        TypeName sysName = TypeName::error;
        TypeDeclNode(const std::string &str) : rawNname(str){init();}
        TypeDeclNode(const char * ptr_c) : rawNname(*(new std::string(ptr_c))) {init();}

        void init();
        virtual llvm::Value* CodeGen(CodeGenContext& context) {};
        llvm::Type* toLLVMType();
    }

    

};


#endif //SIMPLEPASCALCOMPILER_ABSTRACTTREE_H
