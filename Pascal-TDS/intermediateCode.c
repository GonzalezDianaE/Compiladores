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

//ListThreeDir *head;
//ListThreeDir *last;
//std::ofstream file;
//string fileName = "CTD.txt";

char *generateLabel();
char *generateTemp();
void initListThreeDir(ListThreeDir *head, ListThreeDir *last);
void generateInterCode (node *tree, ListThreeDir *last);
void showOperation (ListThreeDir *head);
void showOperation2 (OpThreeDir *operation);
void insertOperation (OpThreeDir *operation, ListThreeDir *last);
void generateLoad (node *tree, ListThreeDir *last);
void generateAssing (node *tree, ListThreeDir *last);
void generateEqual(node *tree, ListThreeDir *last);
void generateOpArit(node *tree, ListThreeDir *last);
void generateOpAritUn(node *tree, ListThreeDir *last);
void generateOpLog(node *tree, ListThreeDir *last);
void generateOpLogUn(node *tree, ListThreeDir *last);
void generateOpRel(node *tree, ListThreeDir *last);
void generateFunctionCall(node *tree, ListThreeDir *last);
void generateIf(node *tree, ListThreeDir *last);
void generateIfElse(node *tree, ListThreeDir *last);
void generateWhile(node *tree, ListThreeDir *last);
void generateReturnVoid(node *tree, ListThreeDir *last);
void generateReturnExp(node *tree, ListThreeDir *last);

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
void initListThreeDir (ListThreeDir *head, ListThreeDir *last){
	//file.open(fileName.c_str());
	head =(ListThreeDir *) malloc(sizeof(ListThreeDir));
	head->next = NULL;
	last = head;

/* Método general para la generación de la lista de instrucciones.*/
void generateInterCode (node *tree, ListThreeDir *last){
	if (tree->content != NULL){
		printf("%d\n", tree->content->type );
		switch (tree->content->type){
			case VAR:
				printf("%s\n", "variable");
				generateLoad(tree,last);
			break;

			case CONSTANT :
				printf("%s\n", "constante");
				generateLoad(tree,last);
			break;

			case OPER_AR :
				generateOpArit(tree,last);
			break;

			case OPER_LOG :
				generateOpLog(tree,last);
			break;

			case OPER_REL :
				generateOpRel(tree,last);
			break;

			case FUNCTION:
				printf("funcion \n");
			break;

			case FUNCTION_CALL_NP :
				generateFunctionCall(tree,last);
			break;

			case FUNCTION_CALL_P :
				generateFunctionCall(tree,last);				
			break;

			case PARAMETER :
				generateLoad(tree,last);
			break;

			case IFAUX :
				generateIf(tree,last);
			break;

			case IF_ELSE :
				generateIfElse(tree,last);
			break;

			case ASSIGN :
				printf("%s\n", "assign" );
				generateAssing(tree,last);
			break;

			case WHILEAUX :
				generateWhile(tree,last);
			break;

			case RETURNAUX :
				generateReturnExp(tree,last);
			break;

			case RETURN_EXPR :
				generateReturnVoid(tree,last);
			break;

			case STATEMENTS :
				generateInterCode(tree->left,last);
				generateInterCode(tree->right,last);
			break;

			case BLOCK :
				generateInterCode(tree->left,last);
			break;

			case OPER_AR_UN :
				generateOpAritUn(tree,last);
			break;

			case OPER_LOG_UN :
				generateOpLogUn(tree,last);
			break;

			case OPER_EQUAL :
				generateEqual(tree,last);
			break;
		}
	}
}

/* Inserta la operación pasada como parámetro en la lista de instrucciones. */
void insertOperation (OpThreeDir *operation, ListThreeDir *last){
	showOperation2(operation);
	printf("%s\n", "insert" );
	ListThreeDir *aux = (ListThreeDir *) malloc(sizeof(ListThreeDir));
	printf("last = NULL %d\n", last == NULL );
	last->next = aux;
	printf("%s\n", "malloc" );
	last = last->next;
	printf("%s\n", "last" );
	last->next = NULL;
	printf("%s\n", "null" );
	last->operation = operation;
	printf("%s\n", "fin" );
}

void showOperation (ListThreeDir *head){
	ListThreeDir *aux = head;
	if(aux->next!=NULL){
		aux = aux->next;
		while(aux!=NULL){
			OpThreeDir *operation = aux->operation;
			switch (operation->instr) {
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
			aux = aux->next;
		}
	}
}

void showOperation2 (OpThreeDir *operation){
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
	
/* Genera las lineas de codigo intermedio correspondientes a la carga de variables, constantes, parametros, etc. */
void generateLoad (node *tree, ListThreeDir *last){
	printf("%s\n", "generateLoad" );
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	operation->oper1 = tree->content;
	strcpy(result->name,generateTemp());
	result->value = tree->content->value;
	result->type = VAR;
	result->ret = tree->content->ret;
	operation->instr = IC_LOAD;
	operation->result = result;
	insertOperation(operation,last);

}

/* Genera las lineas de codigo intermedio correspondientes a una asignación. */
void generateAssing (node *tree, ListThreeDir *last){
	printf("%s\n", "GA" );
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	// Variable
	printf("%s\n", tree->right->content->name);
	item *variable = (item *) malloc(sizeof(item));
	strcpy(variable->name,tree->right->content->name);
	printf("%s\n",variable->name);
	operation->oper1 = variable;
	//Expresion
	generateInterCode(tree->left,last);
	printf("%s\n", "algo" );
	operation->result = last->operation->result;
	operation->instr = IC_ASSING;
	insertOperation(operation,last);
}

/* Genera las lineas de codigo intermedio correspondientes a una comparación por igual (aritmética o lógica). */
void generateEqual(node *tree, ListThreeDir *last){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left,last);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right,last);
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
	insertOperation(operation,last);
}

/* Genera las lineas de codigo intermedio correspondientes a una operación aritmética binaria. */
void generateOpArit(node *tree, ListThreeDir *last){
	char name[32];
	strcpy(name,tree->content->name);
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left,last);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right,last);
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
	insertOperation(operation,last);
}

/* Genera las lineas de codigo intermedio correspondientes a una operación aritmética unaria. */
void generateOpAritUn(node *tree, ListThreeDir *last){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left,last);
	operation->oper1 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	operation->instr = IC_NEG;
	strcpy(result->name,generateTemp());
	operation->result=result;
	insertOperation(operation,last);

}

/* Genera las lineas de codigo intermedio correspondientes a una operación lógica binaria. */
void generateOpLog(node *tree, ListThreeDir *last){
	char name[32];
	strcpy(name,tree->content->name);
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left,last);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right,last);
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
	insertOperation(operation,last);
}

/* Genera las lineas de codigo intermedio correspondientes a una operación lógica unaria. */
void generateOpLogUn(node *tree, ListThreeDir *last){
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left,last);
	operation->oper1 = last->operation->result;
	result->value = 0;
	result->type = VAR;
	operation->instr = IC_NOT;
	strcpy(result->name,generateTemp());
	operation->result=result;
	insertOperation(operation,last);
}

/* Genera las lineas de codigo intermedio correspondientes a una comparación por mayor o menor (int). */
void generateOpRel(node *tree, ListThreeDir *last){
	char name[32];
	strcpy(name,tree->content->name);
	OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	item *result = (item *) malloc(sizeof(item));
	generateInterCode(tree->left,last);
	operation->oper1 = last->operation->result;
	generateInterCode(tree->right,last);
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
	insertOperation(operation,last);
}

/* Genera las lineas de codigo intermedio correspondientes a una llamada a función. */
void generateFunctionCall(node *tree, ListThreeDir *last){
	// Load params (if it has)
	if(tree->content->type==FUNCTION_CALL_P){
		paramsCall *currentParam = tree->content->paramsCall;
		if(currentParam->next!=NULL){
			currentParam = currentParam->next;
			while (currentParam!=NULL){
				generateInterCode(currentParam->param,last);
				OpThreeDir *loadParam = (OpThreeDir *) malloc(sizeof(OpThreeDir));
				loadParam->instr = IC_LOAD;
				loadParam->result = last->operation->result;
				insertOperation(loadParam,last);
				currentParam = currentParam->next;
			}
		}
	}
	// Load function call
	OpThreeDir *functionCall = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	functionCall->instr = IC_CALL;
	strcpy(functionCall->result->name, tree->content->name);
	insertOperation(functionCall,last);
}

/* Genera las lineas de codigo intermedio correspondientes a un if-then. */
void generateIf(node *tree, ListThreeDir *last){
	// Labels
	item *labelEnd = (item *) malloc(sizeof(item));
	strcpy(labelEnd->name,generateLabel());

	// Main instruction
	OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	mainOperation->instr = IC_IF;
	mainOperation->oper2 = labelEnd;

	// Condition
	generateInterCode(tree->left,last);
	mainOperation->result = last->operation->result;

	// Insert if instruction
	insertOperation(mainOperation,last);

	// Then
	generateInterCode(tree->middle,last);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd->instr = IC_LABEL;
	instrLabelEnd->result = labelEnd;
	insertOperation(instrLabelEnd,last);
}

/* Genera las lineas de codigo intermedio correspondientes a un if-then-else. */
void generateIfElse(node *tree, ListThreeDir *last){
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
	generateInterCode(tree->left,last);
	mainOperation->result = last->operation->result;

	// Insert if instruction
	insertOperation(mainOperation,last);

	// Then
	generateInterCode(tree->middle,last);

	// Jump
	OpThreeDir *jumpToEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	jumpToEnd->instr = IC_JUMP;
	jumpToEnd->result = labelEnd;
	insertOperation(jumpToEnd,last);

	// Label Else
	OpThreeDir *instrLabelElse = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelElse->instr = IC_LABEL;
	instrLabelElse->result = labelElse;
	insertOperation(instrLabelElse,last);

	// Else
	generateInterCode(tree->right,last);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd->instr = IC_LABEL;
	instrLabelEnd->result = labelEnd;
	insertOperation(instrLabelEnd,last);
}

/* Genera las lineas de codigo intermedio correspondientes a un ciclo while. */
void generateWhile(node *tree, ListThreeDir *last){
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
	insertOperation(instrLabelWhile,last);

	// Condition
	generateInterCode(tree->left,last);
	mainOperation->result = last->operation->result;	

	// Insert while instruction
	insertOperation(mainOperation,last);

	// Block
	generateInterCode(tree->right,last);

	// Jump
	OpThreeDir *jumpToCond = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	jumpToCond->instr = IC_JUMP;
	jumpToCond->result = labelWhile;
	insertOperation(jumpToCond,last);

	// Label End
	OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	instrLabelEnd->instr = IC_LABEL;
	instrLabelEnd->result = labelEnd;
	insertOperation(instrLabelEnd,last);
}

/* Genera las lineas de codigo intermedio correspondientes a un return void. */
void generateReturnVoid(node *tree, ListThreeDir *last){
	OpThreeDir *returnVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	returnVoid->instr = IC_RETVOID;
	insertOperation(returnVoid,last);
}

/* Genera las lineas de codigo intermedio correspondientes a un return de una expresión (int o bool). */
void generateReturnExp(node *tree, ListThreeDir *last){
	OpThreeDir *returnNotVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
	if(tree->content->ret==5){
		returnNotVoid->instr = IC_RETINT;
	} else {
		returnNotVoid->instr = IC_RETBOOL;
	}
	generateInterCode(tree->left,last);
	returnNotVoid->result = last->operation->result;
	insertOperation(returnNotVoid,last);
}
