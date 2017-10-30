#include "intermediateCode.c"
#include <stdlib.h>

const int REG_SIZE = 8;
char jumpRet[32];

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
void generateIfAss(OpThreeDir *operation);
void generateWhileAss(OpThreeDir *operation);
void generateLabelAss(OpThreeDir *operation);
void generateJump(OpThreeDir *operation);
void generateLoad(OpThreeDir *operation);

// Leo
void generateRetInt(OpThreeDir *operation);
void generateRetBool(OpThreeDir *operation);
void generateRetVoid(OpThreeDir *operation);
void generatePushParam(OpThreeDir *operation);
void generateCallFunc(OpThreeDir *operation);
void generateBeginFunc(OpThreeDir *operation);
void generateEndFunc(OpThreeDir *operation);
void generateLoadParam(OpThreeDir *operation);


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

			case IC_MINOR :
				generateMinnor(operation);
			break;

			case IC_MAJOR :
				generateMajor(operation);
			break;

			case IC_ASSIGN :
				generateAssign(operation);
			break;

			case IC_IF :
				generateIfAss(operation);
			break;

			case IC_WHILE :
				generateWhileAss(operation);
			break;

			case IC_LABEL :
				generateLabelAss(operation);
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

			case IC_LOAD_P :
				generateLoadParam(operation);
			break;
		}
	}
}

void generateAdd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) + (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	addl -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética substracción. */
void generateSub(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) - (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	subq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética multiplicación. */
void generatePlus(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) * (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	imulq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética divisón. */
void generateDiv(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) / (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	cqto \n");
		printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética módulo. */
void generateMod(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) % (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	cqto \n"); //extiende %rax para guardar el resto de la division (ver bien esto)
		printf("	idivl -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
		//Ver bien esto, porque en realidad la instruccion es una division VER QUE SERIA EL RESULT
		printf("	movq %%rdx, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
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
		printf("	movb $%d, -%d(%%rbp)\n", (operation->oper1->value), ((operation->oper1->offSet)*REG_SIZE));
		//mueve el valor de la segunda variable
		printf("	movb $%d, -%d(%%rbp)\n",(operation->oper2->value), ((operation->oper2->offSet)*REG_SIZE));
		//compara el valor de la segunda variable con la primer variable
		printf("	cmpb $%d, -%d(%%rbp)\n", (operation->oper2->value), ((operation->oper1->offSet)*REG_SIZE));
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
		printf("	movb $%d, -%d(%%rbp)\n", (operation->oper1->value), ((operation->oper1->offSet)*REG_SIZE));
		//mueve el valor de la segunda variable
		printf("	movb $%d, -%d(%%rbp)\n",(operation->oper2->value), ((operation->oper2->offSet)*REG_SIZE));
		//compara el valor de la segunda variable con la primer variable
		printf("	cmpb $%d, -%d(%%rbp)\n", (operation->oper2->value), ((operation->oper1->offSet)*REG_SIZE));
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
		printf("	movb $%d, -%d(%%rbp)\n", (operation->oper1->value), ((operation->oper1->offSet)*REG_SIZE));
		//printf("  movzbl -%d(%%rbp), (%%rax)\n",((operation->oper1->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código objeto correspondientes a una operación aritmética equal. */
void generateEqAr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) == (operation->oper2->value)){ //es true
			printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{ 
				printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		printf("	movq $%d, -%d(%%rbp)\n",(operation->oper1->value),((operation->oper1->offSet)*REG_SIZE));
		printf("	movq $%d, -%d(%%rbp)\n",(operation->oper2->value),((operation->oper2->offSet)*REG_SIZE));
		printf("  	movq -%d(%%rbp), (%%rax)\n", ((operation->oper1->offSet)*REG_SIZE));
		printf("  	cmpl -%d(%%rbp), (%%rax)\n", ((operation->oper2->offSet)*REG_SIZE));
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
		printf("	movq $%d, -%d(%%rbp)\n",(-(operation->oper1->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		printf("	movq -%d(%%rbp),(%%rax)\n",(operation->oper1->offSet)*REG_SIZE);
		printf("	negl (%%rax)\n");
		printf("	movq (%%rax), -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}


/* Genera las lineas de código objeto correspondientes a una operación aritmética minnor. */
void generateMinnor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) < (operation->oper2->value)){ //es true
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{ 
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		printf("	movq $%d, -%d(%%rbp)\n",(operation->oper1->value),((operation->oper1->offSet)*REG_SIZE));
		printf("	movq $%d, -%d(%%rbp)\n",(operation->oper2->value),((operation->oper2->offSet)*REG_SIZE));
		printf("	movq -%d(%%rbp), (%%rax)\n", ((operation->oper1->offSet)*REG_SIZE));
		printf("	cmpl -%d(%%rbp), (%%rax)\n", ((operation->oper2->offSet)*REG_SIZE));
	}
}


/* Genera las lineas de código objeto correspondientes a una operación aritmética major. */
void generateMajor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) > (operation->oper2->value)){ //es true
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{ 
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		printf("	movq $%d, -%d(%%rbp)\n",(operation->oper1->value),((operation->oper1->offSet)*REG_SIZE));
		printf("	movq $%d, -%d(%%rbp)\n",(operation->oper2->value),((operation->oper2->offSet)*REG_SIZE));
		printf("	movq -%d(%%rbp), (%%rax)\n", ((operation->oper1->offSet)*REG_SIZE));
		printf("	cmpl -%d(%%rbp), (%%rax)\n", ((operation->oper2->offSet)*REG_SIZE));
	}
}

void generateAssign(OpThreeDir *operation){
	printf("	movq -%d(%%rbp), %%rax\n", (operation->result->offSet)*REG_SIZE);
	printf("	movq %%rax, -%d(%%rbp)\n", ((operation->oper1->offSet)*REG_SIZE));}

void generateIfAss(OpThreeDir *operation){
	printf("	cmpl $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);
	printf("	je %s\n", operation->oper1->name);
}

void generateWhileAss(OpThreeDir *operation){
	printf("	cmpl $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);
	printf("	je %s\n", operation->oper1->name);
}

void generateLabelAss(OpThreeDir *operation){
	printf("%s:\n",operation->result->name);
}

void generateJump(OpThreeDir *operation){
	printf("	jmp %s\n", operation->result->name);
}

void generateLoad(OpThreeDir *operation){
	printf("	movq $%d, -%d(%%rbp)\n", operation->oper1->value, (operation->result->offSet)*REG_SIZE);
}

void generateRetInt(OpThreeDir *operation){
	printf("	movq -%d(%%rbp),  %%rax\n",(operation->result->offSet)*REG_SIZE);
	printf("	jmp %s\n",jumpRet);
}

void generateRetBool(OpThreeDir *operation){
	printf("	movq -%d(%%rbp),  %%rax\n",(operation->result->offSet)*REG_SIZE);
	printf("	jmp %s\n",jumpRet);
}

void generateRetVoid(OpThreeDir *operation){
	printf("	jmp %s\n",jumpRet);
}

void generatePushParam(OpThreeDir *operation){
	int i = operation->result->value;
	switch (i){
    case 1:
      printf("	movq -%d(%%rbp),  %%rdi\n",(operation->oper1->offSet)*REG_SIZE);
    break;
    
    case 2:
      printf("	movq -%d(%%rbp),  %%rsi\n",(operation->oper1->offSet)*REG_SIZE);
    break;
          
    case 3:
      printf("	movq -%d(%%rbp),  %%rdx\n",(operation->oper1->offSet)*REG_SIZE);
    break;
          
    case 4:
      printf("	movq -%d(%%rbp),  %%rcx\n",(operation->oper1->offSet)*REG_SIZE);
    break;

    case 5:
      printf("	movq -%d(%%rbp),  %%r8\n",(operation->oper1->offSet)*REG_SIZE);
    break;

    case 6:
      printf("	movq -%d(%%rbp),  %%r9\n",(operation->oper1->offSet)*REG_SIZE);
    break;

    default:
      printf("	pushq -%d(%%rbp)\n",(operation->oper1->offSet)*REG_SIZE);
    break;
  }
}

void generateCallFunc(OpThreeDir *operation){
	printf("	call _%s\n",operation->oper1->name);
	printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
}

void generateBeginFunc(OpThreeDir *operation){
	printf(".globl _%s\n", operation->result->name );
 	printf("_%s:\n", operation->result->name);
 	printf("enter $%d,$0\n",operation->stackSize*REG_SIZE);
 	strcpy(jumpRet,operation->result->name);
 	strcat(jumpRet, "fin");
}

void generateEndFunc(OpThreeDir *operation){
	printf("%s:\n",jumpRet);
	printf("	leave\n");
	printf("	retq\n");
}

void generateLoadParam(OpThreeDir *operation){
	int i = operation->oper1->value;
	switch (i){
    case 1:
      printf("	movq %%rdi, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
    break;
    
    case 2:
      printf("	movq %%rsi, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
    break;
          
    case 3:
      printf("	movq %%rdx,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
    break;
          
    case 4:
      printf("	movq %%rcx,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
    break;

    case 5:
      printf("	movq %%r8,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
    break;

    case 6:
      printf("  movq %%r9,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
    break;

    default:
    	printf("  movq %d(%%rbp) , %%rax\n", ((operation->oper1->value-6)*REG_SIZE)+REG_SIZE);

      printf("	movq %%rax, -%d(%%rbp)\n",((operation->result->offSet)*REG_SIZE));
    break;
  }	
}