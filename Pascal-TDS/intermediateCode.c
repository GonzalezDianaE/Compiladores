#include "structures.c"
#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <stdbool.h>

#define IC_ADD 0
#define IC_SUB 1
#define IC_PLUS 2
#define IC_DIV 3
#define IC_MOD 4
#define IC_AND 5
#define IC_OR 6
#define IC_NOT 7
#define IC_EQUALAR 8
#define IC_EQUALLOG 9
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
	item *oper1;
	item *oper2;
	item *result;
}OpThreeDir;

typedef struct ListThreeDir{
	OpThreeDir *operation;
	struct ListThreeDir *next;
}ListThreeDir;
int labels = 0;
int temps = 0;
char label[32];
char tmp[32];

ListThreeDir *head;
ListThreeDir *last;
//std::ofstream file;
//string fileName = "CTD.txt";

char *generateLabel();
char *generateTemp();
void initListThreeDir();
void generateInterCode (node *tree);
void showOperation (OpThreeDir *operation);
void insertOperation (OpThreeDir *operation);
void generateLoad (node *tree);
void generateAssing (node *tree);
void generateEqual(node *tree);
void generateOpArit(node *tree);
void generateOpAritUn(node *tree);
void generateOpLog(node *tree);
void generateOpLogUn(node *tree);
void generateOpRel(node *tree);
void generateFunctionCall(node *tree);
void generateIf(node *tree);
void generateIfElse(node *tree);
void generateWhile(node *tree);
void generateReturnVoid(node *tree);
void generateReturnExp(node *tree);

/* Genera un nuevo nombre para label distinto a todos los anteriores. */
char *generateLabel(){
	char c[32];
	sprintf(c,"%d",labels);
	strcpy(label,"Label ");
	strcat (label, c);
	labels++;
	return label;
}

/* Genera un nuevo nombre para temporal distinto a todos los anteriores. */
char *generateTemp(){
	char c[32];
	sprintf(c,"%d",temps);
	strcpy(tmp,"Temp ");
	strcat (tmp, c);
	temps++;
	return tmp;
}

/* Inicialización de la lista de instrucciones con elemento ficticio.*/
void initListThreeDir (){
	//file.open(fileName.c_str());
	head =(ListThreeDir *) malloc(sizeof(ListThreeDir));
	head->next = NULL;
	last = head;
}

/* Método general para la generación de la lista de instrucciones.*/
void generateInterCode (node *tree){
	if (tree->content != NULL){
		switch (tree->content->type){
			case VAR:
				generateLoad(tree);
			break;

			case CONSTANT :
				generateLoad(tree);
			break;

			case OPER_AR :
				generateOpArit(tree);
			break;

			case OPER_LOG :
				generateOpLog(tree);
			break;

			case OPER_REL :
				generateOpRel(tree);
			break;

			case FUNCTION:
			//??
			break;

			case FUNCTION_CALL_NP :
				generateFunctionCall(tree);
			break;

			case FUNCTION_CALL_P :
				generateFunctionCall(tree);				
			break;

			case PARAMETER :
				generateLoad(tree);
			break;

			case IFAUX :
				generateIf(tree);
			break;

			case IF_ELSE :
				generateIfElse(tree);
			break;

			case ASSIGN :
				generateAssing(tree);
			break;

			case WHILEAUX :
				generateWhile(tree);
			break;

			case RETURNAUX :
				generateReturnExp(tree);
			break;

			case RETURN_EXPR :
				generateReturnVoid(tree);
			break;

			case STATEMENTS :
				generateInterCode(tree->left);
				generateInterCode(tree->right);
			break;

			case BLOCK :
				generateInterCode(tree->left);
			break;

			case OPER_AR_UN :
				generateOpAritUn(tree);
			break;

			case OPER_LOG_UN :
				generateOpLogUn(tree);
			break;

			case OPER_EQUAL :
				generateEqual(tree);
			break;
		}
	}
}

/* Inserta la operación pasada como parámetro en la lista de instrucciones. */
void insertOperation (OpThreeDir *operation){
	showOperation(operation);
	last->next = (ListThreeDir *) malloc(sizeof(ListThreeDir));
	last = last->next;
	last->next = NULL;
	last->operation = operation;
}

void showOperation (OpThreeDir *operation){
	switch (operation->instr){
		case 0 : {
			printf("ADD       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 1 : {
			printf( "SUB       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 2 : {
			printf("PLUS      ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 3 : {
			printf( "DIV       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 4 : {
			printf( "MOD       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 5 : {
			printf( "AND       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 6 : {
			printf( "OR        ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 7 : {
			printf( "NOT       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 8 : {
			printf( "EQAR      ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 9 : {
			printf( "EQLOG     ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 10 : {
			printf( "NEG       ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 11 : {
			printf( "MINNOR    ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 12 : {
			printf( "MAJOR     ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 13 : {
			printf( "ASSIGN    ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 14 : {
			printf( "IF        ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 15 : {
			printf( "WHILE     ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 16 : {
			printf( "LABEL     ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 17 : {
			printf( "JUMP      ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 18 : {
			printf( "RETINT    ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 19 : {
			printf( "RETBOOL   ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 20 : {
			printf( "RETVOID   ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 21 : {
			printf( "PPARAM    ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 22 : {
			printf( "CALL      ");
			printf("%s 		",operation->oper1->name);
			printf("%s 		",operation->oper2->name);
			printf("%s\n",operation->result->name);
			break;
		}
		case 23 : {
			printf( "LOAD      ");
			if (operation->oper1->type == VAR || operation->oper1->type == PARAMETER){
				printf("%s 		        ",operation->oper1->name);
				printf("%s 		",operation->oper2->name);
				printf("%s\n",operation->result->name);
			}
			else{
				if(operation->oper1->ret == INTEGERAUX){
					printf("%d 		        ",operation->oper1->value);
					printf("%s 		",operation->oper2->name);
					printf("%s\n",operation->result->name);
				}
				else{
					printf("%s 		        ",operation->oper1->value? "true":"false");
					printf("%s 		",operation->oper2->name);
					printf("%s\n",operation->result->name);
				}
			}
			break;
		}
	}

}

/* Cierra el archivo en el que se registraron las instrucciones.*/
/*void closeFile(){
	file.close();
}*/

/* Registra la operación pasada como parámetro en un archivo de tipo txt.*/
/*void saveOperation (OpThreeDir *operation){
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
}*/

/* Cierra el archivo en el que se registraron las instrucciones.*/
/*void closeFile(){
	file.close();
}*/
	
/* Genera las lineas de codigo intermedio correspondientes a la carga de variables, constantes, parametros, etc. */
void generateLoad (node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	operation->oper1 = tree->content;
	strcpy(result->name,generateTemp());
	result->value = tree->content->value;
	result->type = VAR;
	result->ret = tree->content->ret;
	operation->instr = IC_LOAD;
	operation->result = result;
	insertOperation(operation);

}

/* Genera las lineas de codigo intermedio correspondientes a una asignación. */
void generateAssing (node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right);
	operation->result = last->operation->result;
	operation->instr = IC_ASSING;
	insertOperation(operation);
}

/* Genera las lineas de codigo intermedio correspondientes a una comparación por igual (aritmética o lógica). */
void generateEqual(node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right);
	operation->oper2 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	if(tree->content->ret == INTEGERAUX){
		operation->instr = IC_EQUALAR;
		strcpy(result->name,generateTemp());
	}
	else{
		operation->instr = IC_EQUALLOG;
		strcpy(result->name,generateTemp());
	}
	operation->result = result;
	insertOperation(operation);
}

/* Genera las lineas de codigo intermedio correspondientes a una operación aritmética binaria. */
void generateOpArit(node *tree){
	char name[32];
	strcpy(name,tree->content->name);
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right);
	operation->oper2 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	//Comienza chequeos sobre tipo de operacion
	if (strcmp(name,"OP_ADD")==0){
		operation->instr = IC_ADD;
		strcpy(result->name,generateTemp());
	}
	if (strcmp(name,"OP_SUB")==0){
		operation->instr = IC_SUB;
		strcpy(result->name,generateTemp());
	}
	if (strcmp(name,"OP_PROD")==0){
		operation->instr = IC_PLUS;
		strcpy(result->name,generateTemp());
	}
	if (strcmp(name,"OP_DIV")==0){
		operation->instr = IC_DIV;
		strcpy(result->name,generateTemp());
	}
	if (strcmp(name,"OP_MOD")==0){
		operation->instr = IC_MOD;
		strcpy(result->name,generateTemp());
	}
	operation->result=result;
	insertOperation(operation);
}

/* Genera las lineas de codigo intermedio correspondientes a una operación aritmética unaria. */
void generateOpAritUn(node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	operation->instr = IC_NEG;
	strcpy(result->name,generateTemp());
	operation->result=result;
	insertOperation(operation);

}

/* Genera las lineas de codigo intermedio correspondientes a una operación lógica binaria. */
void generateOpLog(node *tree){
	char name[32];
	strcpy(name,tree->content->name);
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right);
	operation->oper2 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	//Comienza chequeos sobre tipo de operacion
	if (strcmp(name,"OP_AND")==0){
		operation->instr = IC_AND;
		strcpy(result->name,generateTemp());
	}
	if (strcmp(name,"OP_OR")==0){
		operation->instr = IC_OR;
		strcpy(result->name,generateTemp());
	}
	operation->result=result;
	insertOperation(operation);
}

/* Genera las lineas de codigo intermedio correspondientes a una operación lógica unaria. */
void generateOpLogUn(node *tree){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	operation->instr = IC_NOT;
	strcpy(result->name,generateTemp());
	operation->result=result;
	insertOperation(operation);
}

/* Genera las lineas de codigo intermedio correspondientes a una comparación por mayor o menor (int). */
void generateOpRel(node *tree){
	char name[32];
	strcpy(name,tree->content->name);
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right);
	operation->oper2 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	//Comienza chequeos sobre tipo de operacion
	if (strcmp(name,"OP_MINOR")==0){
		operation->instr = IC_MINOR;
		strcpy(result->name,generateTemp());
	}
	if (strcmp(name,"OP_MAJOR")==0){
		operation->instr = IC_MAJOR;
		strcpy(result->name,generateTemp());
	}
	operation->result=result;
	insertOperation(operation);
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
				loadParam->instr = IC_LOAD;
				loadParam->result = last->operation->result;
				insertOperation(loadParam);
				currentParam = currentParam->next;
			}
		}
	}
	// Load function call
	OpThreeDir *functionCall = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	functionCall->instr = IC_CALL;
	strcpy(functionCall->result->name, tree->content->name);
	insertOperation(functionCall);
}

/* Genera las lineas de codigo intermedio correspondientes a un if-then. */
void generateIf(node *tree){
	// Labels
	item *labelEnd = (item *) malloc(sizeof(item));
	strcpy(labelEnd->name,generateLabel());

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation->instr = IC_IF;
	mainOperation->oper2 = labelEnd;

	// Condition
	generateInterCode(tree->left);
	mainOperation->result = last->operation->result;

	// Insert if instruction
	insertOperation(mainOperation);

	// Then
	generateInterCode(tree->middle);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd->instr = IC_LABEL;
	instrLabelEnd->result = labelEnd;
	insertOperation(instrLabelEnd);
}

/* Genera las lineas de codigo intermedio correspondientes a un if-then-else. */
void generateIfElse(node *tree){
	// Labels
	item *labelElse = (item *) malloc(sizeof(item));
	strcpy(labelElse->name,generateLabel());

	item *labelEnd = (item *) malloc(sizeof(item));
	strcpy(labelEnd->name,generateLabel());

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation->instr = IC_IF;
	mainOperation->oper1 = labelElse;
	mainOperation->oper2 = labelEnd;

	// Condition
	generateInterCode(tree->left);
	mainOperation->result = last->operation->result;

	// Insert if instruction
	insertOperation(mainOperation);

	// Then
	generateInterCode(tree->middle);

	// Jump
	OpThreeDir *jumpToEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	jumpToEnd->instr = IC_JUMP;
	jumpToEnd->result = labelEnd;
	insertOperation(jumpToEnd);

	// Label Else
	OpThreeDir *instrLabelElse = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelElse->instr = IC_LABEL;
	instrLabelElse->result = labelElse;
	insertOperation(instrLabelElse);

	// Else
	generateInterCode(tree->right);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd->instr = IC_LABEL;
	instrLabelEnd->result = labelEnd;
	insertOperation(instrLabelEnd);
}

/* Genera las lineas de codigo intermedio correspondientes a un ciclo while. */
void generateWhile(node *tree){
	// Labels
	item *labelWhile = (item *) malloc(sizeof(item));
	strcpy(labelWhile->name,generateLabel());
	item *labelEnd = (item *) malloc(sizeof(item));
	strcpy(labelEnd->name,generateLabel());

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation->instr = IC_WHILE;
	mainOperation->oper1 = labelEnd;

	// Label While
	OpThreeDir *instrLabelWhile = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelWhile->instr = IC_LABEL;
	instrLabelWhile->result = labelWhile;
	insertOperation(instrLabelWhile);

	// Condition
	generateInterCode(tree->left);
	mainOperation->result = last->operation->result;	

	// Insert while instruction
	insertOperation(mainOperation);

	// Block
	generateInterCode(tree->right);

	// Jump
	OpThreeDir *jumpToCond = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	jumpToCond->instr = IC_JUMP;
	jumpToCond->result = labelWhile;
	insertOperation(jumpToCond);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd->instr = IC_LABEL;
	instrLabelEnd->result = labelEnd;
	insertOperation(instrLabelEnd);
}

/* Genera las lineas de codigo intermedio correspondientes a un return void. */
void generateReturnVoid(node *tree){
	OpThreeDir *returnVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	returnVoid->instr = IC_RETVOID;
	//returnVoid.result = label??;
	insertOperation(returnVoid);
}

/* Genera las lineas de codigo intermedio correspondientes a un return de una expresión (int o bool). */
void generateReturnExp(node *tree){
	OpThreeDir *returnNotVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	if(tree->content->ret==5){
		returnNotVoid->instr = IC_RETINT;
	} else {
		returnNotVoid->instr = IC_RETBOOL;
	}
	// returnNotVoid.result = label???;
	insertOperation(returnNotVoid);
}
