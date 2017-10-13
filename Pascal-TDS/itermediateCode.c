#include "structures.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define IC_ADD 0
#define IC_SUB 1
#define IC_PLUS 2
#define IC_DIV 3
#define IC_MOD 4
#define IC_AND 5
#define IC_OR 6
#define IC_NOT 7
#define IC_EQUALAR 8
#define IC_EUQALLOG 9
#define IC_NEG 10
#define IC_MINOR 11
#define IC_MAJOR 12
#define IC_ASSING 13
#define IC_IF 14
#define IC_WHILE 15
#define IC_LABEL 16
#define IC_JUMP 17
#define IC_RETINT 18
#define IC_RETBOOL 19
#define IC_RETVOID 20
#define IC_PPARAM 21
#define IC_CALL 22
#define IC_LOAD 23



typedef struct OpThreeDir{
	int instr;
	item oper1;
	item oper2;
	item result;
}OpThreeDir;

typedef struct ListThreeDir{
	OpThreeDir *operation;
	struct ListThreeDir *next;
}ListThreeDir;
int labels = 0;
int temps = 0;

ListThreeDir *head;
ListThreeDir *last;


//3-inicializacion elemento ficticio
void initListThreeDir (ListThreeDir *head){
	head =(symbol *) malloc(sizeof(symbol));
	head->next = NULL;
	last = head;
}
//3-metodo general de generacion de listas, aca tmb se debe tener en cuenta los casos a ignorar para la recursion
void generateInterCode (node *tree){
	if node->content = NULL{
		switch (node->content->type){
			case VAR:
				generateLoad(node);
			break;

			case CONSTANT :
				generateLoad(node);
			break;

			case OPER_AR :
				generateOperAr(node);
			break;

			case OPER_LOG :
				generateOperLog(node);
			break;

			case OPER_REL :
				generateOperRel(node);
			break;

			case FUNCTION:
			//??
			break;

			case FUNCTION_CALL_NP :
				generateCallFunction(node);
			break;

			case FUNCTION_CALL_P :
				generateCallFunction(node);				
			break;

			case PARAMETER :
				generateLoad(node);
			break;

			case IFAUX :
				generateIf(node);
			break;

			case IF_ELSE :
				generateIfElse(node);
			break;

			case ASSIGN :
				generateAssing(node);
			break;

			case WHILEAUX :
				generateWhile(node);
			break;

			case RETURNAUX :
				generateReturnExp(node);
			break;

			case RETURN_EXPR :
				generateReturnVoid(node);
			break;

			case STATEMENTS :
				generateInterCode(node->left);
				generateInterCode(node->right);
			break;

			case BLOCK :
				generateInterCode(node->left);
			break;

			case OPER_AR_UN :
				generateOperArUn(node);
			break;

			case OPER_LOG_UN :
				generateOperLogUn(node);
			break;

			case OPER_EQUAL :
				generateEqual(node)
			break;
		}
	}
}

void insertOperation (OpThreeDir *operation){
	last->next = (symbol *) malloc(sizeof(symbol));
	last = last->next;
	last->next = NULL;
	last->operation = operation;
}

	
//3-metodo load Variable, Parametro, constante
void generateAssing (node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	operation->oper1 = node->content;
	result->name = "LOAD";
	result->value = node->content->value;
	result->type = VAR;
	result->ret = node->content->ret;
	operation->intr = IC_LOAD;
	operation->result = result;
	insertOperation(operation);

}
//3-metodo assign
void generateAssing (node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	/*deberia guardar la variable ???
	generateInterCode(tree->right);
	operation->result = last->result;
	*/
	operation->intr = IC_EQUALLOG;
	operation->result = result;

	insertOperation(operation);
}
//3-metodo opEqqual
void generateEqual(node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	generateInterCode(tree->right);
	operation->oper2 = last->result;
	result->value = 0;
	result->type = VAR;
	if(tree->content->ret = INTEGERAUX){
		operation->intr = IC_EQUALAR;
		result->name = "resultEqualAr";
	}
	else{
		operation->intr = IC_EQUALLOG;
		result->name = "resultEqualLog";

	}
	operation->result = result;
	insertOperation(operation);
}

//1-metodo opArit
void generateOpArit(node *tree){
	char name[32] = tree->content->name;
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	generateInterCode(tree->right);
	operation->oper2 = last->result;
	result->value = 0;
	result->type = VAR;
	//Comienza chequeos sobre tipo de operacion
	if (strcmp((name,"OP_ADD")==0)){
		operation->intr = IC_ADD;
		result->name = "resultADD";
	}
	if (strcmp((name,"OP_SUB")==0)){
		operation->intr = IC_SUB;
		result->name = "resultSUB";
	}
	if (strcmp((name,"OP_PROD")==0)){
		operation->intr = IC_PLUS;
		result->name = "resultPLUS";
	}
	if (strcmp((name,"OP_DIV")==0)){
		operation->intr = IC_DIV;
		result->name = "resultDIV";
	}
	if (strcmp((name,"OP_MOD")==0)){
		operation->intr = IC_MOD;
		result->name = "resultMOD";
	}
}

//1-metodo opAritUnario TIENE UNICAMENTE LA OPERACION IC_NEG EN YACC OP_SUB
void generateOpAritUn(node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	result->value = 0;
	result->type = VAR;
	operation->intr = IC_NEG;
	result->name = "resultNEG";
}

//1-metodo opLog
void generateOpLog(node *tree){
	char name[32] = tree->content->name;
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	generateInterCode(tree->right);
	operation->oper2 = last->result;
	result->value = 0;
	result->type = VAR;
	//Comienza chequeos sobre tipo de operacion
	if (strcmp((name,"OP_AND")==0)){
		operation->intr = IC_AND;
		result->name = "resultAND";
	}
	if (strcmp((name,"OP_OR")==0)){
		operation->intr = IC_OR;
		result->name = "resultOR";
	}
}

//1-metodo opLog unario TIENE UNICAMENTE LA OPERACION IC_NOT EN YACC OP_NOT
void generateOpLogUn(node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	result->value = 0;
	result->type = VAR;
	operation->intr = IC_NOT;
	result->name = "resultNOT";
}

//1-metodo opRel
void generateOpRel(node *tree){
	char name[32] = tree->content->name;
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	generateInterCode(tree->right);
	operation->oper2 = last->result;
	result->value = 0;
	result->type = VAR;
	//Comienza chequeos sobre tipo de operacion
	if (strcmp((name,"OP_MINOR")==0)){
		operation->intr = IC_MINOR;
		result->name = "resultMINOR";
	}
	if (strcmp((name,"OP_MAJOR")==0)){
		operation->intr = IC_MAJOR;
		result->name = "resultMAJOR";
	}
}

string generateLabel(){
	string ret = "Label"+labels;
	labels++;
	return ret;
}

string generateTemp(){
	string tmp = "Tmp"+temps;
	temps++;
	return tmp;
}

//2-metodo function call
void generateFunctionCall(ListThreeDir node){

}

//2-metodo if
void generateIf(ListThreeDir node){

}

//2-metodo if else
void generateIfElse(ListThreeDir node){

}

//2-metodo while
void generateWhile(ListThreeDir node){

}

//2-metodo return void -- con posibilidades de cambio
void generateReturnVoid(ListThreeDir node){

}

//2-metodo return con expresion -- con posibilidades de cambio
void generateReturnExp(ListThreeDir node){

}
