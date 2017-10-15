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
std::ofstream file;
string fileName = "CTD.txt";

/* Genera un nuevo nombre para label distinto a todos los anteriores. */
string generateLabel(){
	string ret = "Label"+labels;
	labels++;
	return ret;
}

/* Genera un nuevo nombre para temporal distinto a todos los anteriores. */
string generateTemp(){
	string tmp = "Tmp"+temps;
	temps++;
	return tmp;
}

/* Inicialización de la lista de instrucciones con elemento ficticio.*/
void initListThreeDir (ListThreeDir *head){
	file.open(fileName.c_str());
	head =(symbol *) malloc(sizeof(symbol));
	head->next = NULL;
	last = head;
}

/* Método general para la generación de la lista de instrucciones.*/
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
				generateFunctionCall(node);
			break;

			case FUNCTION_CALL_P :
				generateFunctionCall(node);				
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

/* Inserta la operación pasada como parámetro en la lista de instrucciones. */
void insertOperation (OpThreeDir *operation){
	saveOperation(operation);
	last->next = (symbol *) malloc(sizeof(symbol));
	last = last->next;
	last->next = NULL;
	last->operation = operation;
}

/* Registra la operación pasada como parámetro en un archivo de tipo txt.*/
void saveOperation (OpThreeDir *operation){
	string instr;
	switch (operation->instr){
		case 0 : {
			file << "ADD       ";
			break;
		}
		case 1 : {
			file << "SUB       ";
			break;
		}
		case 2 : {
			file << "PLUS      ";
			break;
		}
		case 3 : {
			file << "DIV       ";
			break;
		}
		case 4 : {
			file << "MOD       ";
			break;
		}
		case 5 : {
			file << "AND       ";
			break;
		}
		case 6 : {
			file << "OR        ";
			break;
		}
		case 7 : {
			file << "NOT       ";
			break;
		}
		case 8 : {
			file << "EQAR      ";
			break;
		}
		case 9 : {
			file << "EQLOG     ";
			break;
		}
		case 10 : {
			file << "NEG       ";
			break;
		}
		case 11 : {
			file << "MINNOR    ";
			break;
		}
		case 12 : {
			file << "MAJOR     ";
			break;
		}
		case 13 : {
			file << "ASSIGN    ";
			break;
		}
		case 14 : {
			file << "IF        ";
			break;
		}
		case 15 : {
			file << "WHILE     ";
			break;
		}
		case 16 : {
			file << "LABEL     ";
			break;
		}
		case 17 : {
			file << "JUMP      ";
			break;
		}
		case 18 : {
			file << "RETINT    ";
			break;
		}
		case 19 : {
			file << "RETBOOL   ";
			break;
		}
		case 20 : {
			file << "RETVOID   ";
			break;
		}
		case 21 : {
			file << "PPARAM    ";
			break;
		}
		case 22 : {
			file << "CALL      ";
			break;
		}
		case 23 : {
			file << "LOAD      ";
			break;
		}
	}
	file << operation->oper1->name << "			";
	file << operation->oper2->name << "			";
	file << operation->result->name << "\n";
}

/* Cierra el archivo en el que se registraron las instrucciones.*/
void closeFile(){
	file.close();
}
	
/* Genera las lineas de codigo intermedio correspondientes a la carga de variables, constantes, parametros, etc. */
void generateLoad (node *tree){
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

/* Genera las lineas de codigo intermedio correspondientes a una asignación. */
void generateAssing (node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->result;
	generateInterCode(tree->right);
	operation->result = last->result;
	operation->intr = IC_ASSING;
	operation->result = result;
	insertOperation(operation);
}

/* Genera las lineas de codigo intermedio correspondientes a una comparación por igual (aritmética o lógica). */
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

/* Genera las lineas de codigo intermedio correspondientes a una operación aritmética binaria. */
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

/* Genera las lineas de codigo intermedio correspondientes a una operación aritmética unaria. */
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

/* Genera las lineas de codigo intermedio correspondientes a una operación lógica binaria. */
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

/* Genera las lineas de codigo intermedio correspondientes a una operación lógica unaria. */
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

/* Genera las lineas de codigo intermedio correspondientes a una comparación por mayor o menor (int). */
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

/* Genera las lineas de codigo intermedio correspondientes a una llamada a función. */
void generateFunctionCall(node *tree){
	// Load params (if it has)
	if(tree->content->type==FUNCTION_CALL_P){
		paramsCall *currentParam = tree->content->paramsCall;
		if(currentParam->next!=NULL){
			currentParam = currentParam->next;
			while (currentParam!=NULL){
				generateInterCode(currentParam->param);
				OpThreeDir *loadParam = (OpThreeDir *) malloc(sizeof(OpThreeDir));
				loadParam.instr = IC_LOAD;
				loadParam.result = last->operation->result;
				insertOperation(loadParam);
				currentParam = currentParam->next;
			}
		}
	}
	// Load function call
	OpThreeDir *functionCall = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	functionCall.instr = IC_CALL;
	functionCall.result = tree->content->name;
	insertOperation(functionCall);
}

/* Genera las lineas de codigo intermedio correspondientes a un if-then. */
void generateIf(node *tree){
	// Labels
	string strLabelEnd = generateLabel();
	item *labelEnd = (item *) malloc(sizeof(item));
	labelEnd.name = strLabelEnd;

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation.instr = IC_IF;
	mainOperation.oper2 = labelEnd;

	// Condition
	generateInterCode(tree->left);
	mainOperation.result = last->operation->result;

	// Insert if instruction
	insertOperation(mainOperation);

	// Then
	generateInterCode(tree->middle);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd.instr = IC_LABEL;
	instrLabelEnd.result = labelEnd;
	insertOperation(instrLabelEnd);
}

/* Genera las lineas de codigo intermedio correspondientes a un if-then-else. */
void generateIfElse(node *tree){
	// Labels
	string strLabelElse = generateLabel();
	item *labelElse = (item *) malloc(sizeof(item));
	labelElse.name = strLabelElse;
	string strLabelEnd = generateLabel();
	item *labelEnd = (item *) malloc(sizeof(item));
	labelEnd.name = strLabelEnd;

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation.instr = IC_IF;
	mainOperation.oper1 = labelElse;
	mainOperation.oper2 = labelEnd;

	// Condition
	generateInterCode(tree->left);
	mainOperation.result = last->operation->result;

	// Insert if instruction
	insertOperation(mainOperation);

	// Then
	generateInterCode(tree->middle);

	// Jump
	OpThreeDir *jumpToEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	jumpToEnd.instr = IC_JUMP;
	jumpToEnd.result = labelEnd;
	insertOperation(jumpToEnd);

	// Label Else
	OpThreeDir *instrLabelElse = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelElse.instr = IC_LABEL;
	instrLabelElse.result = labelElse;
	insertOperation(instrLabelElse);

	// Else
	generateInterCode(tree->rigth);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd.instr = IC_LABEL;
	instrLabelEnd.result = labelEnd;
	insertOperation(instrLabelEnd);
}

/* Genera las lineas de codigo intermedio correspondientes a un ciclo while. */
void generateWhile(node *tree){
	// Labels
	string strLabelWhile = generateLabel();
	item *labelWhile = (item *) malloc(sizeof(item));
	labelWhile.name = strLabelWhile;
	string strLabelEnd = generateLabel();
	item *labelEnd = (item *) malloc(sizeof(item));
	labelEnd.name = strLabelEnd;

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation.instr = IC_WHILE;
	mainOperation.oper1 = labelEnd;

	// Label While
	OpThreeDir *instrLabelWhile = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelWhile.instr = IC_LABEL;
	instrLabelWhile.result = labelWhile;
	insertOperation(instrLabelWhile);

	// Condition
	generateInterCode(tree->left);
	mainOperation.result = last->operation->result;	

	// Insert while instruction
	insertOperation(mainOperation);

	// Block
	generateInterCode(tree->rigth);

	// Jump
	OpThreeDir *jumpToCond = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	jumpToCond.instr = IC_JUMP;
	jumpToCond.result = labelWhile;
	insertOperation(jumpToCond);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd.instr = IC_LABEL;
	instrLabelEnd.result = labelEnd;
	insertOperation(instrLabelEnd);
}

/* Genera las lineas de codigo intermedio correspondientes a un return void. */
void generateReturnVoid(node *tree){
	OpThreeDir *returnVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	returnVoid.instr = IC_RETVOID;
	//returnVoid.result = label??;
	insertOperation(returnVoid);
}

/* Genera las lineas de codigo intermedio correspondientes a un return de una expresión (int o bool). */
void generateReturnExp(node *tree){
	OpThreeDir *returnNotVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	if(tree->content->ret==5){
		returnNotVoid.instr = IC_RETINT;
	} else {
		returnNotVoid.instr = IC_RETBOOL;
	}
	// returnNotVoid.result = label???;
	insertOperation(returnNotVoid);
}
