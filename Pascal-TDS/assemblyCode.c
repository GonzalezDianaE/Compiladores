#include "intermediateCode.c"
#include <stdlib.h>

const int REG_SIZE = 8;
char jumpRet[32];
FILE *archivo;
char result[32]; //utilizado para definir strings y guardarlos en el archivo
bool prints = false; 



/* DECLARACIÓN DE FUNCIONES */

void generateFile();
void newAssemblyString(char *c1, int i1, int i2, char *c2);

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
void generatePrints (OpThreeDir *operation);




/* IMPLEMENTACION DE FUNCIONES */

/* Generación del archivo con extensión .s que contendrá el código assembler. */
void generateFile(){
	archivo = fopen("assemblyCode.s","w");
	if (archivo == NULL) {printf("%s\n","Error: file not created." );}
	else { printf("%s\n", "Successfully created file.");}
}

/* Genera las instrucciones de forma adecuada para luego agregarlas al archivo */
void newAssemblyString(char *c1, int i1, int i2, char *c2){
	char aux[32];
	strcpy(result, c1);  
	sprintf(aux,"%d",i1); 
	strcat (result, aux); 

	/*i2 sera igual a 3 en el caso de que NO necesite un segundo integer.
	En el caso de que i2 sea distinto de 3, se crearán cadenas del tipo instrucción+integer+registro 
	por ejemplo: movq $i1, -i2(%rbp) */
	if(i2!=3){
		strcat(result, ", -"); 
		sprintf(aux,"%d",i2); 
		strcat(result, aux); 
		strcat(result, c2); 
		strcat(result, "\n"); 
	} else {
		strcat(result, c2); 
		strcat(result, "\n");
	}
}

/* Método general para la generación de la lista de instrucciones.
		Mediante un case sobre el tipo del nodo corriente, invoca al método
		correspondiente para generar su código assembler.
*/
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

			case IC_PRINT :
				generatePrints(operation);
			break;
		}
	}
}

void generatePrints (OpThreeDir *operation){
	prints = true;
	newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rdi");
	fputs(result,archivo);
	printf("	movq -%d(%%rbp),  %%rdi\n",(operation->result->offSet)*REG_SIZE);
	strcpy(result, "	call _print");
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	call print\n");

}


/* Genera las lineas de código assembler correspondientes a una operación aritmética adición. */
void generateAdd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) + (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) + (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);

		newAssemblyString("	addq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	addq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética sustracción. */
void generateSub(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) - (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) - (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);

		newAssemblyString("	subq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	subq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética multiplicación. */
void generatePlus(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) * (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) * (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);

		newAssemblyString("	imulq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	imulq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética divisón. */
void generateDiv(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) / (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);	
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) / (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);

		strcpy(result, "	cqto \n");
		fputs(result,archivo);
		printf("	cqto \n");

		newAssemblyString("	idivq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética módulo. */
void generateMod(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		newAssemblyString("	movq $", ((operation->oper1->value) % (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);
		printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) % (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);

		strcpy(result, "	cqto \n");
		fputs(result,archivo);
		printf("	cqto \n");//extiende %rax para guardar el resto de la division (ver bien esto)

		newAssemblyString("	idivq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);

		newAssemblyString("	movq %rdx, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rdx, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica and. */
void generateAnd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		if(((operation->oper1->value) && (operation->oper2->value))==0){ //ES FALSE
			newAssemblyString("	movb $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));

		newAssemblyString("	andq -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	andq -%d(%%rbp), %%rax\n", ((operation->oper2->offSet)*REG_SIZE));

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica or. */
void generateOr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		if(((operation->oper1->value) || (operation->oper2->value))==0){ //ES FALSE
			newAssemblyString("	movb $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));

		newAssemblyString("	orq -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	orq -%d(%%rbp), %%rax\n", ((operation->oper2->offSet)*REG_SIZE));

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica not. */
void generateNot(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT){	
		if((operation->oper1->value)==0){ //ES FALSE
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{
			newAssemblyString("	movb $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("  movq -%d(%%rbp), %%rax\n",((operation->oper1->offSet)*REG_SIZE));

		strcpy(result, "	xorq $-1, %rax\n");
		fputs(result,archivo);
		printf("	xorq $-1, %%rax\n");

		strcpy(result, "	andq $1, %rax\n");
		fputs(result,archivo);
		printf("	andq $1, %%rax\n");

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));

	}
}

/* Genera las lineas de código assembler correspondientes a  una comparación por igual (aritmética). */
void generateEqAr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) == (operation->oper2->value)){ //es true
			newAssemblyString("	movq $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{ 
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));

		newAssemblyString("	cmpq %rax, -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));

		strcpy(result, "	sete %dl\n");
		fputs(result,archivo);
		printf("	sete %%dl\n");

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);
		printf("	andb $1 , %%dl\n");

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);
		printf("	movzbl %%dl , %%esi\n");

		newAssemblyString("	movq %rsi, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una comparación por igual (lógica). */
void generateEqLog(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) == (operation->oper2->value)){ //es true
			newAssemblyString("	movq $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		} else{ 
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));

		newAssemblyString("	cmpq %rax, -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));

		strcpy(result, "	sete %dl\n");
		fputs(result,archivo);
		printf("	sete %%dl\n");

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);
		printf("	andb $1 , %%dl\n");

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);
		printf("	movzbl %%dl , %%esi\n");

		newAssemblyString("	movq %rsi, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética negación. */
void generateNeg(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT){	
		newAssemblyString("	movq $",(-(operation->oper1->value)),((operation->result->offSet)*REG_SIZE), "(%rbp)");
		fputs(result,archivo);
		printf("	movq $%d, -%d(%%rbp)\n",(-(operation->oper1->value)),((operation->result->offSet)*REG_SIZE));
	} else {
		newAssemblyString("	movq -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n",(operation->oper1->offSet)*REG_SIZE);

		strcpy(result, "	negq %rax\n");
		fputs(result,archivo);
		printf("	negq %%rax\n");

		newAssemblyString("	movq %rax, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}
}


/* Genera las lineas de código assembler correspondientes a una operación aritmética minnor. */
void generateMinnor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) < (operation->oper2->value)){ //es true
			newAssemblyString("	movq $",1,((operation->result->offSet)*REG_SIZE), "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{ 
			newAssemblyString("	movq $",0,((operation->result->offSet)*REG_SIZE), "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		newAssemblyString("	movq -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));

		newAssemblyString("	cmpq %rax, -",((operation->oper2->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
		printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));

		strcpy(result, "	setg %dl\n");
		fputs(result,archivo);
		printf("	setg %%dl\n");

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);
		printf("	andb $1 , %%dl\n");

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);
		printf("	movzbl %%dl , %%esi\n");

		newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}


/* Genera las lineas de código assembler correspondientes a una operación aritmética major. */
void generateMajor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) > (operation->oper2->value)){ //es true
			newAssemblyString("	movq $",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
		} else{ 
			newAssemblyString("	movq $",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
		}
	} else{
		newAssemblyString("	movq -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rax");
		fputs(result,archivo);
		printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));

		newAssemblyString("	cmpq %rax, -",((operation->oper2->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
		printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));

		strcpy(result, "	setl %dl\n");
		fputs(result,archivo);
		printf("	setl %%dl\n");

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);
		printf("	andb $1 , %%dl\n");

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);
		printf("	movzbl %%dl , %%esi\n");

		newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una asignación. */
void generateAssign(OpThreeDir *operation){
	newAssemblyString("	movq -",((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
	fputs(result,archivo);
	printf("	movq -%d(%%rbp), %%rax\n", (operation->result->offSet)*REG_SIZE);

	newAssemblyString("	movq %rax, -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);
	printf("	movq %%rax, -%d(%%rbp)\n", ((operation->oper1->offSet)*REG_SIZE));
}

/* Genera las lineas de código assembler correspondientes a un if. */
void generateIfAss(OpThreeDir *operation){
	newAssemblyString("	cmpl $0, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);
	printf("	cmpl $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);

	strcpy(result, "	je ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	je %s\n", operation->oper1->name);
}

/* Genera las lineas de código assembler correspondientes a un while. */
void generateWhileAss(OpThreeDir *operation){
	newAssemblyString("	cmpl $0, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);
	printf("	cmpl $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);

	strcpy(result, "	je ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	je %s\n", operation->oper1->name);
}

/* Genera las lineas de código assembler correspondientes a un label. */
void generateLabelAss(OpThreeDir *operation){
	strcpy(result,(operation->result->name));
	strcat(result, ":\n");
	fputs(result,archivo);
	printf("%s:\n",operation->result->name);
}

/* Genera las lineas de código assembler correspondientes a un jump. */
void generateJump(OpThreeDir *operation){
	strcpy(result, "	jmp ");
	strcat(result, (operation->result->name));
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	jmp %s\n", operation->result->name);
}

/* Genera las lineas de código assembler correspondientes a una operación load. */
void generateLoad(OpThreeDir *operation){
	newAssemblyString("	movq $",operation->oper1->value, ((operation->result->offSet)*REG_SIZE), "(%rbp)");
	fputs(result,archivo);
	printf("	movq $%d, -%d(%%rbp)\n", operation->oper1->value, (operation->result->offSet)*REG_SIZE);
}

/* Genera las lineas de código assembler correspondientes a un return int. */
void generateRetInt(OpThreeDir *operation){
	newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
	fputs(result,archivo);
	printf("	movq -%d(%%rbp),  %%rax\n",(operation->result->offSet)*REG_SIZE);

	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	jmp %s\n",jumpRet);
}

/* Genera las lineas de código assembler correspondientes a un return bool. */
void generateRetBool(OpThreeDir *operation){
	newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
	fputs(result,archivo);
	printf("	movq -%d(%%rbp),  %%rax\n",(operation->result->offSet)*REG_SIZE);

	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	jmp %s\n",jumpRet);
}

/* Genera las lineas de código assembler correspondientes a un return void. */
void generateRetVoid(OpThreeDir *operation){
	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	jmp %s\n",jumpRet);
}

/* Genera las lineas de código assembler correspondientes a la carga de parametros. */
void generatePushParam(OpThreeDir *operation){
	int i = operation->result->value;
	switch (i){
		case 1:
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rdi");
			fputs(result,archivo);
			printf("	movq -%d(%%rbp),  %%rdi\n",(operation->oper1->offSet)*REG_SIZE);
		break;

		case 2:
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rsi");
			fputs(result,archivo);
			printf("	movq -%d(%%rbp),  %%rsi\n",(operation->oper1->offSet)*REG_SIZE);
		break;

		case 3:
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rdx");
			fputs(result,archivo);
			printf("	movq -%d(%%rbp),  %%rdx\n",(operation->oper1->offSet)*REG_SIZE);
		break;

		case 4:
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rcx");
			fputs(result,archivo);
			printf("	movq -%d(%%rbp),  %%rcx\n",(operation->oper1->offSet)*REG_SIZE);
		break;

		case 5:
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %r8");
			fputs(result,archivo);
			printf("	movq -%d(%%rbp),  %%r8\n",(operation->oper1->offSet)*REG_SIZE);
		break;

		case 6:
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %r9");
			fputs(result,archivo);
			printf("	movq -%d(%%rbp),  %%r9\n",(operation->oper1->offSet)*REG_SIZE);
		break;

		default:
			if (operation->oper1->type==REGISTER){
				strcpy(result, "	pushq %rbx\n");
				fputs(result,archivo);
				printf("	pushq %%rbx\n");
			}
			else{
				newAssemblyString("	pushq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp)");
				fputs(result,archivo);
				printf("	pushq -%d(%%rbp)\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
}

/* Genera las lineas de código assembler correspondientes a una llamada a función. */
void generateCallFunc(OpThreeDir *operation){
	strcpy(result, "	call ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	call %s\n",operation->oper1->name);
	int i =operation->result->value;
	if (i>6){
		newAssemblyString("	addq $", (i-6)*REG_SIZE , 3 , ", %rsp");
		fputs(result,archivo);
		printf("	addq $%d , %%rsp\n", (i-6)*REG_SIZE);
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);
	printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
}

/* Genera las lineas de código assembler correspondientes al inicio de una función. */
void generateBeginFunc(OpThreeDir *operation){
	//guardo nombre funcion
	bool isMain = (strcmp("main",operation->result->name)==0);
	if (isMain){
		strcpy(result, ".globl _");
		strcat(result, (operation->result->name));
		strcat(result, "\n");
		fputs(result,archivo);
		printf(".globl _%s\n", operation->result->name );

		strcpy(result, "_");
		strcat(result, (operation->result->name));
		strcat(result, ":\n");
		fputs(result,archivo);
		printf("_%s:\n", operation->result->name);
	}
	else {
		strcpy(result, (operation->result->name));
		strcat(result, ":\n");
		fputs(result,archivo);
		printf("%s:\n", operation->result->name);
	}
	//si el stackSize es impar, entonces el tamaño del stack no es multiplo de 16
	if ((operation->stackSize%2) != 0){
		operation->stackSize++;
	}
	newAssemblyString("	enter $", (operation->stackSize*REG_SIZE),3, ", $0");
	fputs(result,archivo);
	printf("enter $%d,$0\n",operation->stackSize*REG_SIZE);
	//le agrego al nombre de la funcion fin para dsp generar label de fin de funcion
	strcpy(jumpRet,operation->result->name);
	strcat(jumpRet, "fin");
}

/* Genera las lineas de código assembler correspondientes al fin de una función. */
void generateEndFunc(OpThreeDir *operation){
	strcpy(result, jumpRet);
	strcat(result, ":\n");
	fputs(result,archivo);
	printf("%s:\n",jumpRet);

	strcpy(result, "	leave\n");
	fputs(result,archivo);
	printf("	leave\n");

	strcpy(result, "	retq\n");
	strcat(result, "\n");
	fputs(result,archivo);
	printf("	retq\n");
	bool condicion = ((prints));
	if ((strcmp("mainfin",jumpRet)==0) && (prints)){
		strcpy(result, "_print:\n");
		fputs(result,archivo);
		printf("_print:\n");

		strcpy(result, "enter $16, $0\n");
		fputs(result,archivo);
		printf("enter $16 , $0\n");

		strcpy(result, "	movq	%rdi, %rax\n");
		fputs(result,archivo);
		printf("	movl	%%rdi, %%rax\n");

		strcpy(result, "	leaq	_printAux(%rip), %rdi\n");
		fputs(result,archivo);
		printf("	leaq	_printAux(%%rip), %%rdi\n");


		strcpy(result, "	movl	%eax, %esi\n");
		fputs(result,archivo);
		printf("	movl	%%eax, %%esi\n");

		strcpy(result, "	movb	$0, %al\n");
		fputs(result,archivo);
		printf("	movb	$0, %%al\n");

		strcpy(result, "	callq _printf\n");
		fputs(result,archivo);
		printf("	callq	_printf\n");

		strcpy(result, "	xorl %esi, %esi\n");
		fputs(result,archivo);
		printf("	xorl	%%esi, %%esi\n");

		strcpy(result, "	leave\n");
		fputs(result,archivo);
		printf("	leave\n");

		strcpy(result, "	retq\n");
		strcat(result, "\n");
		fputs(result,archivo);
		printf("	retq\n");

		strcpy(result, "_printAux: \n");
		fputs(result,archivo);
		printf("_printAux: \n");

		strcpy(result, ".asciz	\"Result:  %d \\n\"");
		fputs(result,archivo);
		printf(".asciz	\"Result:  %%d \\n\"");

	}

}

/* Genera las lineas de código assembler correspondientes a la lectura de parámetros. */
void generateLoadParam(OpThreeDir *operation){
	int i = operation->oper1->value;
	switch (i){
		case 1:
			newAssemblyString("	movq %rdi, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq %%rdi, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		break;
		
		case 2:
			newAssemblyString("	movq %rsi, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq %%rsi, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		break;
					
		case 3:
			newAssemblyString("	movq %rdx, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq %%rdx,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		break;
					
		case 4:
			newAssemblyString("	movq %rcx, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq %%rcx,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		break;

		case 5:
			newAssemblyString("	movq %r8, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq %%r8,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		break;

		case 6:
			newAssemblyString("	movq %r9, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("  movq %%r9,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		break;

		default:
			newAssemblyString("	movq ", ((operation->oper1->value-6)*REG_SIZE)+REG_SIZE ,3, "(%rbp), %rax");
			fputs(result,archivo);
			printf("  movq %d(%%rbp) , %%rax\n", ((operation->oper1->value-6)*REG_SIZE)+REG_SIZE);

			newAssemblyString("	movq  %rax, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			printf("	movq %%rax, -%d(%%rbp)\n",((operation->result->offSet)*REG_SIZE));
		break;
	}	
}