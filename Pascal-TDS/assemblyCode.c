#include "intermediateCode.c"
#include <stdlib.h>

const int REG_SIZE = 4;

void generateAssembly(ListThreeDir *head);
void generateAdd(OpThreeDir *operation);
void generateSub(OpThreeDir *operation);
void generatePlus(OpThreeDir *operation);
void generateDiv(OpThreeDir *operation);
void generateMod(OpThreeDir *operation);
void generateOr(OpThreeDir *operation);
void generateNot(OpThreeDir *operation);
void generateEqAr(OpThreeDir *operation);
void generateEqLog(OpThreeDir *operation);
void generateNeg(OpThreeDir *operation);
void generateMinnor(OpThreeDir *operation);
void generateMajor(OpThreeDir *operation);
void generateAssign(OpThreeDir *operation);
void generateIf(OpThreeDir *operation);
void generateWhile(OpThreeDir *operation);
void generateLabel(OpThreeDir *operation);
void generateJump(OpThreeDir *operation);
void generateRetInt(OpThreeDir *operation);
void generateRetBool(OpThreeDir *operation);
void generateRetVoid(OpThreeDir *operation);
void generatePushParam(OpThreeDir *operation);
void generateCallFunc(OpThreeDir *operation);
void generateLoad(OpThreeDir *operation);
void generateBeginFunc(OpThreeDir *operation);
void genrateEndFunc(OpThreeDir *operation);


void generateAssembly(ListThreeDir *head){
	ListThreeDir *aux = head;
	while (aux->next!=NULL){
		aux = aux->next;
		OpThreeDir *operation = aux->operation;
		switch (operation->instr){
			case IC_ADD :
				generateAdd(operation);
			break;

			case IC_SUB :
				generateSub(operation);
			break;

			case IC_PLUS :
				generatePlus(operation);
			break;

			case IC_DIV :
				generateDiv(operation);
			break;

			case IC_MOD :
				generateMod(operation);
			break;

			case IC_OR :
				generateOr(operation);
			break;

			case IC_NOT :
				generateNot(operation);
			break;

			case IC_EQUALAR :
				generateEqAr(operation);
			break;

			case IC_EQUALLOG :
				generateEqLog(operation);
			break;

			case IC_NEG :
				generateNeg(operation);
			break;

			case IC_MINNOR :
				generateMinnor(operation);
			break;

			case IC_MAJOR :
				generateMajor(operation);
			break;

			case IC_ASSIGN :
				generateAssign(operation);
			break;

			case IC_IF :
				generateIf(operation);
			break;

			case IC_WHILE :
				generateWhile(operation);
			break;

			case IC_LABEL :
				generateLabel(operation);
			break;

			case IC_JUMP :
				generateJump(operation);
			break;

			case IC_RETINT :
				generateRetInt(operation);
			break;

			case IC_RETBOOL :
				generateRetBool(operation);
			break;

			case IC_RETVOID :
				generateRetVoid(operation);
			break;

			case IC_PPARAM :
				generatePushParam(operation);
			break;

			case IC_CALL :
				generateCallFunc(operation);
			break;

			case IC_LOAD :
				generateLoad(operation);
			break;

			case IC_BEGIN_FUNCTION :
				generateBeginFunc(operation);
			break;

			case IC_END_FUNCTION :
				generateEndFunc(operation);
			break;
		}
	}
}

void generateAdd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		printf("	movl $%d, -%d(%%ebp)\n",((operation->oper1->value) + (operation->oper2->value)),((opertation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%ebp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	addl -%d(%%ebp),(%%eax)\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movl (%%eax), -%d(%%ebp)\n",(opertation->result->offSet)*REG_SIZE);
	}
}

void generateSub(OpThreeDir *operation){

}

void generatePlus(OpThreeDir *operation){

}

void generateDiv(OpThreeDir *operation){

}

void generateMod(OpThreeDir *operation){

}

void generateOr(OpThreeDir *operation){

}

void generateNot(OpThreeDir *operation){

}

void generateEqAr(OpThreeDir *operation){

}

void generateEqLog(OpThreeDir *operation){

}

void generateNeg(OpThreeDir *operation){

}

void generateMinnor(OpThreeDir *operation){

}

void generateMajor(OpThreeDir *operation){

}

void generateAssign(OpThreeDir *operation){

}

void generateIf(OpThreeDir *operation){

}

void generateWhile(OpThreeDir *operation){

}

void generateLabel(OpThreeDir *operation){

}

void generateJump(OpThreeDir *operation){

}

void generateRetInt(OpThreeDir *operation){

}

void generateRetBool(OpThreeDir *operation){

}

void generateRetVoid(OpThreeDir *operation){

}

void generatePushParam(OpThreeDir *operation){

}

void generateCallFunc(OpThreeDir *operation){

}

void generateLoad(OpThreeDir *operation){

}

void generateBeginFunc(OpThreeDir *operation){

}

void genrateEndFunc(OpThreeDir *operation){

}

