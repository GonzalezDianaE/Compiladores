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



typedef struct OpThreeDir{
	int instr;
	item oper1;
	item oper2;
	item result;
}OpThreeDir;

typedef struct ListThreeDir{
	codIntermedio *operation;
	struct ListThreeDir *;
}ListThreeDir;
int labels = 0;
int temps = 0;


ListThreeDir generList (node *head) {
	ListThreeDir head;
	int type = head->content->type;
	if (type==OPER_AR){
		char name[32] = head->content->name;
		if (strcmp((name,"OP_ADD")==0)){
			head = (ListThreeDir *) malloc(sizeof(ListThreeDir)); 
			ListThreeDir aux;
			ListThreeDir aux2;
			//ESTA CONDICION DEBERIA SER UNA FUNCION POR QUE ME VOY A CANSAR DE USARLA
			if (head->left->content->type == VAR || head->left->content->type == PARAMETER || head->left->content->type == CONSTANT){
				aux = ListThreeDir generList (head->left);
				head->operation->oper1 = aux->oper->result;
			}
			else {

			}
			aux = ListThreeDir generList (head->left);
			aux2 = ListThreeDir generList (head->right);
			concatList(aux,aux2);
			concatList(aux2,head);
			head->operation->oper1 = aux->oper->result;
			head->operation->oper2 = aux->oper->result;
			return aux;
		}  

	}
}


