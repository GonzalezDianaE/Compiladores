#include "intermediateCode.c"
#include <stdlib.h>

const int REG_SIZE = 4;

void generateAssembly(ListThreeDir *head);
void generateAdd(OpThreeDir *operation);

//Diana
void generateSub(OpThreeDir *operation);
void generatePlus(OpThreeDir *operation);
void generateDiv(OpThreeDir *operation);
void generateMod(OpThreeDir *operation);
void generateAnd(OpThreeDir *operation);
void generateOr(OpThreeDir *operation);
void generateNot(OpThreeDir *operation);
void generateEqAr(OpThreeDir *operation);
void generateEqLog(OpThreeDir *operation);
void generateNeg(OpThreeDir *operation);
void generateMinnor(OpThreeDir *operation);
void generateMajor(OpThreeDir *operation);

// Elisa
void generateAssign(OpThreeDir *operation);
void generateIf(OpThreeDir *operation);
void generateWhile(OpThreeDir *operation);
void generateLabel(OpThreeDir *operation);
void generateJump(OpThreeDir *operation);
void generateLoad(OpThreeDir *operation);

// Leo
void generateRetInt(OpThreeDir *operation);
void generateRetBool(OpThreeDir *operation);
void generateRetVoid(OpThreeDir *operation);
void generatePushParam(OpThreeDir *operation);
void generateCallFunc(OpThreeDir *operation);
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

			case IC_AND:
				generateAnd(operation);
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
		printf("	movl $%d, -%d(%%ebp)\n",((operation->oper1->value) + (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%ebp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	addl -%d(%%ebp),(%%eax)\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movl (%%eax), -%d(%%ebp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética substracción. */
void generateSub(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		printf("	movl $%d, -%d(%%rbp)\n",((operation->oper1->value) - (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%rbp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	imull -%d(%%rbp),(%%eax)\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movl (%%eax), -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética multiplicación. */
void generatePlus(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		printf("	movl $%d, -%d(%%rbp)\n",((operation->oper1->value) * (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%rbp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	subl -%d(%%rbp),(%%eax)\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movl (%%eax), -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética divisón. */
void generateDiv(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		printf("	movl $%d, -%d(%%rbp)\n",((operation->oper1->value) / (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%rbp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("  cltd \n"); //extiende %eax para guardar el resto de la division (ver bien esto)
		printf("	idivl -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movl (%%eax), -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética módulo. */
void generateMod(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		printf("	movl $%d, -%d(%%rbp)\n",((operation->oper1->value) % (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%rbp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("  cltd \n"); //extiende %eax para guardar el resto de la division (ver bien esto)
		printf("	idivl -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
		//Ver bien esto, porque en realidad la instruccion es una division VER QUE SERIA EL RESULT
		printf("	movl (%%edx), -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}

/* Genera las lineas de código objeto correspondientes a una operación lógica and. */
void generateAnd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		if(((operation->oper1->value) && (operation->oper2->value))==0){ //ES FALSE
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{
			printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		//mueve el valor de la primera variable 
		printf("  movb $%d, -%d(%%rbp)\n", (operation->oper1->value), ((operation->oper1->offSet)*REG_SIZE));
		//mueve el valor de la segunda variable
		printf("	movb $%d, -%d(%%rbp)\n",(operation->oper2->value), ((operation->oper2->offSet)*REG_SIZE));
		//compara el valor de la segunda variable con la primer variable
		printf("  cmpb $%d, -%d(%%rbp)\n", (operation->oper2->value), ((operation->oper1->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código objeto correspondientes a una operación lógica or. */
void generateOr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		if(((operation->oper1->value) || (operation->oper2->value))==0){ //ES FALSE
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{
			printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		//mueve el valor de la primera variable 
		printf("  movb $%d, -%d(%%rbp)\n", (operation->oper1->value), ((operation->oper1->offSet)*REG_SIZE));
		//mueve el valor de la segunda variable
		printf("	movb $%d, -%d(%%rbp)\n",(operation->oper2->value), ((operation->oper2->offSet)*REG_SIZE));
		//compara el valor de la segunda variable con la primer variable
		printf("  cmpb $%d, -%d(%%rbp)\n", (operation->oper2->value), ((operation->oper1->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código objeto correspondientes a una operación lógica not. */
void generateNot(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT){	
		if((operation->oper1->value)==0){ //ES FALSE
			printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		//PREGUNTAR POR INSTRUCCIONES QUE APARECEN
		printf("  movb $%d, -%d(%%rbp)\n", (operation->oper1->value), ((operation->oper1->offSet)*REG_SIZE));
		//printf("  movzbl -%d(%%rbp), (%%eax)\n",((operation->oper1->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética equal. */
void generateEqAr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) == (operation->oper2->value)){ //es true
			printf("	movl $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{ 
				printf("	movl $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		printf("	movl $%d, -%d(%%ebp)\n",(operation->oper1->value),((operation->oper1->offSet)*REG_SIZE));
		printf("	movl $%d, -%d(%%ebp)\n",(operation->oper2->value),((operation->oper2->offSet)*REG_SIZE));
		printf("  movl -%d(%%ebp), (%%eax)\n", ((operation->oper1->offSet)*REG_SIZE));
		printf("  cmpl -%d(%%ebp), (%%eax)\n", ((operation->oper2->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código objeto correspondientes a una operación lógica equal. */
void generateEqLog(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if(((operation->oper1->value)==0 && (operation->oper2->value))==0){ //ambos son ceros
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
		if(((operation->oper1->value)==1 && (operation->oper2->value))==1){ //ambos son unos
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{ //ES 0 Y 1 o 1 Y 0
				printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		//IGUAL QUE NOT PREGUNTAR
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética negación. */
void generateNeg(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT){	
		printf("	movl $%d, -%d(%%rbp)\n",(-(operation->oper1->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movl -%d(%%rbp),(%%eax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	negl (%%eax)\n");
		printf("	movl (%%eax), -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}


/* Genera las lineas de código objeto correspondientes a una operación aritmética minnor. */
void generateMinnor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) < (operation->oper2->value)){ //es true
			printf("	movl $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{ 
				printf("	movl $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		printf("	movl $%d, -%d(%%ebp)\n",(operation->oper1->value),((operation->oper1->offSet)*REG_SIZE));
		printf("	movl $%d, -%d(%%ebp)\n",(operation->oper2->value),((operation->oper2->offSet)*REG_SIZE));
		printf("  movl -%d(%%ebp), (%%eax)\n", ((operation->oper1->offSet)*REG_SIZE));
		printf("  cmpl -%d(%%ebp), (%%eax)\n", ((operation->oper2->offSet)*REG_SIZE));
	}
}


/* Genera las lineas de código objeto correspondientes a una operación aritmética major. */
void generateMajor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) > (operation->oper2->value)){ //es true
			printf("	movl $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{ 
				printf("	movl $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		printf("	movl $%d, -%d(%%ebp)\n",(operation->oper1->value),((operation->oper1->offSet)*REG_SIZE));
		printf("	movl $%d, -%d(%%ebp)\n",(operation->oper2->value),((operation->oper2->offSet)*REG_SIZE));
		printf("  movl -%d(%%ebp), (%%eax)\n", ((operation->oper1->offSet)*REG_SIZE));
		printf("  cmpl -%d(%%ebp), (%%eax)\n", ((operation->oper2->offSet)*REG_SIZE));
	}
}

void generateAssign(OpThreeDir *operation){
	printf("	movl -%d(%%ebp), -%d(%%ebp)\n", (operation->result->offSet)*REG_SIZE,(operation->oper1->offSet)*REG_SIZE);
}

void generateIf(OpThreeDir *operation){
	printf("	cmpl $0, -%d(%%ebp)\n", (operation->result->offSet)*REG_SIZE);
	printf("	je %s\n", operation->oper1->name);
}

void generateWhile(OpThreeDir *operation){
	printf("	cmpl $0, -%d(%%ebp)\n", (operation->result->offSet)*REG_SIZE);
	printf("	je %s\n", operation->oper1->name);
}

void generateLabel(OpThreeDir *operation){
	printf("%s:\n",operation->result->name);
}

void generateJump(OpThreeDir *operation){
	printf("	jmp %s\n", operation->result->name);
}

void generateLoad(OpThreeDir *operation){
	printf("	movl $%d, -%d(ebp)\n", operation->oper1->value, (operation->result->offSet)*REG_SIZE);
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

void generateBeginFunc(OpThreeDir *operation){

}

void genrateEndFunc(OpThreeDir *operation){

}

