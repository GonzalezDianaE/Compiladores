#include "intermediateCode.c"
#include <stdlib.h>

const int REG_SIZE = 8;
char jumpRet[32];
FILE *archivo;
char result[32]; //utilizado para definir strings y guardarlos en el archivo
bool prints = false; 
int flag;
int os;


/* DECLARACIÓN DE FUNCIONES */

void generateFile(int flag);
void newAssemblyString(char *c1, int i1, int i2, char *c2);
void newAssemblyVarGlobal(char *c1, char *c2, char *c3);

void generateAssembly(ListThreeDir *head, int flag, int os);

void generateAdd(OpThreeDir *operation);
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
void generateAssign(OpThreeDir *operation);
void generateIfAss(OpThreeDir *operation);
void generateWhileAss(OpThreeDir *operation);
void generateLabelAss(OpThreeDir *operation);
void generateJump(OpThreeDir *operation);
void generateLoad(OpThreeDir *operation);
void generateRetInt(OpThreeDir *operation);
void generateRetBool(OpThreeDir *operation);
void generateRetVoid(OpThreeDir *operation);
void generatePushParam(OpThreeDir *operation);
void generateCallFunc(OpThreeDir *operation);
void generateBeginFunc(OpThreeDir *operation);
void generateEndFunc(OpThreeDir *operation);
void generateLoadParam(OpThreeDir *operation);
void generatePrints (OpThreeDir *operation);
void generateVarGlobal (OpThreeDir *operation);




/* IMPLEMENTACION DE FUNCIONES */

/* Generación del archivo con extensión .s que contendrá el código assembler. */
void generateFile(int flag){
	archivo = fopen("assemblyCode.s","w");
	if (archivo == NULL) {
		printf("%s\n","Error: file not created." );
	} else { 
		if(flag==1 || flag==3){
			printf("%s\n\n", "File successfully created.");
		}
	}
	strcpy(result, "	.globl ");
	if(os!=0){
		strcat(result,"_");
	}
	strcat(result, "main\n");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		if(os==0){
			printf("	.globl main\n\n");
		} else {
			printf("	.globl _main\n\n");
		}
	}
}
/* Genera las intrcciones para llamar una variableGlobal*/
void newAssemblyVarGlobal(char *c1, char *c2, char *c3){
	strcpy(result, c1);
	strcat (result, c2);
	strcat (result, c3); 
	strcat(result, "\n"); 
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
void generateAssembly(ListThreeDir *head, int flag, int os){
	flag = flag;
	os = os;
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

			case IC_VARGLOBAL:
				generateVarGlobal(operation);
			break;
		}
	}
}

 /*genera las lineas de codigo correspondientes para declarar una variable*/
void generateVarGlobal (OpThreeDir *operation){
	newAssemblyVarGlobal ("	.comm _", operation->result->name, ",8,8");
	fputs(result,archivo);
	if( flag == 3 || flag == 7){
		printf("	.comm _%s,8,8 \n", operation->result->name);
	}
}
 /*genera las lineas de codigo correspondientes para imprimir una expresion*/
void generatePrints (OpThreeDir *operation){
	prints = true;
	switch (operation->result->type){
		case VAR :
			newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp), %rdi");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rdi\n",(operation->result->offSet)*REG_SIZE);
			}
		break;
		case CONSTANT :
			newAssemblyString("	movq $", (operation->result->value) , 3 , ", %rdi");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rdi  \n", operation->result->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->result->name, "(%rip), %rdi");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rdi  \n", operation->result->name);
			}
		break;
	}
	strcpy(result, "	call _print");
	strcat(result, "\n");
	fputs(result,archivo);
	if( flag == 3 || flag == 7){
		printf("	movq -%d(%%rbp),  %%rdi\n",(operation->result->offSet)*REG_SIZE);
		printf("	call print\n");
	}

}


/* Genera las lineas de código assembler correspondientes a una operación aritmética adición. */
void generateAdd(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	addq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	addq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	addq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	addq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	addq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	addq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética sustracción. */
void generateSub(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;

	}
	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	subq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	subq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	subq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	subqq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	subq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	subq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética multiplicación. */
void generatePlus(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	imulq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	imulq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	imulq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	imulq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	imulq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	imluq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética divisón. */
void generateDiv(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	strcpy(result, "	cqto \n");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	cqto \n");
	}
	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper2->value) , 3 , ", %rbx");
			fputs(result,archivo);
			strcpy(result, "	idivq %rbx \n");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	idivq $%d\n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	idivq _", operation->oper2->name, "(%rip)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	idivq _%s(%%rip)\n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	idivq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética módulo. */
void generateMod(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	strcpy(result, "	cqto \n");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	cqto \n");
	}
	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper2->value) , 3 , ", %rbx");
			fputs(result,archivo);
			strcpy(result, "	idivq %rbx \n");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	idivq $%d\n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	idivq _", operation->oper2->name, "(%rip)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	idivq _%s(%%rip)\n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	idivq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rdx, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rdx, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica and. */
void generateAnd(OpThreeDir *operation){
	switch (operation->oper1->type){

		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	andq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	andq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	andq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	andq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	andq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	andq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica or. */
void generateOr(OpThreeDir *operation){
	switch (operation->oper1->type){

		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	switch (operation->oper2->type){

		case CONSTANT :
			newAssemblyString("	orq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	orq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	orq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	orq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	andq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	andq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica not. */
void generateNot(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}
	
	strcpy(result, "	xorq $-1, %rax\n");
	fputs(result,archivo);

	strcpy(result, "	andq $1, %rax\n");
	fputs(result,archivo);

	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("  movq -%d(%%rbp), %%rax\n",((operation->oper1->offSet)*REG_SIZE));
		printf("	xorq $-1, %%rax\n");
		printf("	andq $1, %%rax\n");
		printf("	movq %%rax, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a  una comparación por igual (aritmética). */
void generateEqAr(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}

	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	cmpq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	cmpq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	cmpq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	cmpq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	sete %dl\n");
	fputs(result,archivo);

	strcpy(result, "	andb $1, %dl\n");
	fputs(result,archivo);

	strcpy(result, "	movzbl %dl, %esi\n");
	fputs(result,archivo);

	newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	sete %%dl\n");
		printf("	andb $1 , %%dl\n");
		printf("	movzbl %%dl , %%esi\n");
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una comparación por igual (lógica). */
void generateEqLog(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}

	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	cmpq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	cmpq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	cmpq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	cmpq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	sete %dl\n");
	fputs(result,archivo);

	strcpy(result, "	andb $1, %dl\n");
	fputs(result,archivo);

	strcpy(result, "	movzbl %dl, %esi\n");
	fputs(result,archivo);

	newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	sete %%dl\n");
		printf("	andb $1 , %%dl\n");
		printf("	movzbl %%dl , %%esi\n");
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética negación. */
void generateNeg(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	negq %rax\n");
	fputs(result,archivo);

	newAssemblyString("	movq %rax, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	negq %%rax\n");
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
	}	
}


/* Genera las lineas de código assembler correspondientes a una operación aritmética minnor. */
void generateMinnor(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}

	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	cmpq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	cmpq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	cmpq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	cmpq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	setl %dl\n");
	fputs(result,archivo);

	strcpy(result, "	andb $1, %dl\n");
	fputs(result,archivo);

	strcpy(result, "	movzbl %dl, %esi\n");
	fputs(result,archivo);

	newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	setl %%dl\n");
		printf("	andb $1 , %%dl\n");
		printf("	movzbl %%dl , %%esi\n");
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}


/* Genera las lineas de código assembler correspondientes a una operación aritmética major. */
void generateMajor(OpThreeDir *operation){
	switch (operation->oper1->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->oper1->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			}
		break;
	}

	switch (operation->oper2->type){
		case CONSTANT :
			newAssemblyString("	cmpq $", (operation->oper2->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $%d, %%rax  \n", operation->oper2->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	cmpq _", operation->oper2->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq _%s(%%rip), %%rax  \n", operation->oper2->name);
			}
		break;
		default :
			newAssemblyString("	cmpq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	cmpq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	setg %dl\n");
	fputs(result,archivo);

	strcpy(result, "	andb $1, %dl\n");
	fputs(result,archivo);

	strcpy(result, "	movzbl %dl, %esi\n");
	fputs(result,archivo);

	newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	setg %%dl\n");
		printf("	andb $1 , %%dl\n");
		printf("	movzbl %%dl , %%esi\n");
		printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a una asignación. */
void generateAssign(OpThreeDir *operation){
	switch (operation->result->type){
		case CONSTANT :
			newAssemblyString("	movq $" , operation->result->value,3,", %rax ");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->result->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->result->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->result->name);
			}
		break;
		default :
			newAssemblyString("	movq -",((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp), %%rax\n", (operation->result->offSet)*REG_SIZE);
			}
		break;
	}
	switch (operation->oper1->type){
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq %rax , _", operation->oper1->name, "(%rip)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq %%rax, _%s(%%rip)   \n", operation->oper1->name);
			}
		break;
		default :
			newAssemblyString("	movq %rax, -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq %%rax, -%d(%%rbp)\n", ((operation->oper1->offSet)*REG_SIZE));
			}
		break;
	}
}

/* Genera las lineas de código assembler correspondientes a un if. */
void generateIfAss(OpThreeDir *operation){
	switch (operation->result->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->result->value) , 3 , ", %rax");
			fputs(result,archivo);
			strcpy( result, "	cmpq $0 , %rax \n");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $0 , $%d  \n", operation->result->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	cmpq $0 , _", operation->result->name, "(%rip)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $0 , _%s(%%rip)  \n", operation->result->name);
			}
		break;
		default :
			newAssemblyString("	cmpq $0, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	cmpq $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	je ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	je %s\n", operation->oper1->name);
	}
}

/* Genera las lineas de código assembler correspondientes a un while. */
void generateWhileAss(OpThreeDir *operation){
	switch (operation->result->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->result->value) , 3 , ", %rax");
			fputs(result,archivo);
			strcpy( result, "	cmpq $0 , %rax \n");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $0 , $%d  \n", operation->result->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	cmpq $0 , _", operation->result->name, "(%rip)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	cmpq $0 , _%s(%%rip)  \n", operation->result->name);
			}
		break;
		default :
			newAssemblyString("	cmpq $0, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	cmpq $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);
			}
		break;
	}
	//newAssemblyString("	cmpl $0, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	//fputs(result,archivo);

	strcpy(result, "	je ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	je %s\n", operation->oper1->name);
	}
}

/* Genera las lineas de código assembler correspondientes a un label. */
void generateLabelAss(OpThreeDir *operation){
	strcpy(result,(operation->result->name));
	strcat(result, ":\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("%s:\n",operation->result->name);
	}
}

/* Genera las lineas de código assembler correspondientes a un jump. */
void generateJump(OpThreeDir *operation){
	strcpy(result, "	jmp ");
	strcat(result, (operation->result->name));
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	jmp %s\n", operation->result->name);
	}
}

/* Genera las lineas de código assembler correspondientes a una operación load. */
void generateLoad(OpThreeDir *operation){
	newAssemblyString("	movq $",operation->oper1->value, ((operation->result->offSet)*REG_SIZE), "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	movq $%d, -%d(%%rbp)\n", operation->oper1->value, (operation->result->offSet)*REG_SIZE);
	}
}

/* Genera las lineas de código assembler correspondientes a un return int. */
void generateRetInt(OpThreeDir *operation){
	switch (operation->result->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->result->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->result->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->result->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->result->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->result->offSet)*REG_SIZE);
			}
		break;
	}

	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	jmp %s\n",jumpRet);
	}
}

/* Genera las lineas de código assembler correspondientes a un return bool. */
void generateRetBool(OpThreeDir *operation){
	switch (operation->result->type){
		case CONSTANT :
			newAssemblyString("	movq $", (operation->result->value) , 3 , ", %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, %%rax  \n", operation->result->value);
			}
		break;
		case VARGLOBAL :
			newAssemblyVarGlobal ("	movq _", operation->result->name, "(%rip), %rax");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq _%s(%%rip), %%rax  \n", operation->result->name);
			}
		break;
		default :
			newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq -%d(%%rbp),%%rax\n",(operation->result->offSet)*REG_SIZE);
			}
		break;
	}
	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	movq -%d(%%rbp),  %%rax\n",(operation->result->offSet)*REG_SIZE);
		printf("	jmp %s\n",jumpRet);
	}
}

/* Genera las lineas de código assembler correspondientes a un return void. */
void generateRetVoid(OpThreeDir *operation){
	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	jmp %s\n",jumpRet);
	}
}

/* Genera las lineas de código assembler correspondientes a la carga de parametros. */
void generatePushParam(OpThreeDir *operation){
	int i = operation->result->value;
	switch (i){
    case 1:
		switch (operation->oper1->type){
			case CONSTANT :
				newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rdi");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq $%d, %%rdi  \n", operation->oper1->value);
				}
			break;
			case VARGLOBAL :
				newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rdi");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq _%s(%%rip), %%rdi  \n", operation->oper1->name);
				}
			break;
			default :
				newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rdi");
				fputs(result,archivo);

				if(flag == 3 || flag == 7){
					printf("	movq -%d(%%rbp),%%rdi\n",(operation->oper1->offSet)*REG_SIZE);
				}
			break;
		}
    break;
    
    case 2:
		switch (operation->oper1->type){
			case CONSTANT :
				newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rsi");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq $%d, %%rsi  \n", operation->oper1->value);
				}
			break;
			case VARGLOBAL :
				newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rsi");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq _%s(%%rip), %%rsi  \n", operation->oper1->name);
				}
			break;
			default :
				newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rsi");
				fputs(result,archivo);

				if(flag == 3 || flag == 7){
					printf("	movq -%d(%%rbp),%%rsi\n",(operation->oper1->offSet)*REG_SIZE);
				}
			break;
		}
    break;
          
    case 3:
		switch (operation->oper1->type){
			case CONSTANT :
				newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rdx");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq $%d, %%rdx  \n", operation->oper1->value);
				}
			break;
			case VARGLOBAL :
				newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rdx");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq _%s(%%rip), %%rdx  \n", operation->oper1->name);
				}
			break;
			default :
				newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rdx");
				fputs(result,archivo);

				if(flag == 3 || flag == 7){
					printf("	movq -%d(%%rbp),%%rdx\n",(operation->oper1->offSet)*REG_SIZE);
				}
			break;
		}
    break;
          
    case 4:
		switch (operation->oper1->type){
			case CONSTANT :
				newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %rcx");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq $%d, %%rcx  \n", operation->oper1->value);
				}
			break;
			case VARGLOBAL :
				newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %rcx");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq _%s(%%rip), %%rcx  \n", operation->oper1->name);
				}
			break;
			default :
				newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rcx");
				fputs(result,archivo);

				if(flag == 3 || flag == 7){
					printf("	movq -%d(%%rbp),%%rcx\n",(operation->oper1->offSet)*REG_SIZE);
				}
			break;
		}
    break;

    case 5:
		switch (operation->oper1->type){
			case CONSTANT :
				newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %r8");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq $%d, %%r8  \n", operation->oper1->value);
				}
			break;
			case VARGLOBAL :
				newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %r8");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq _%s(%%rip), %%r8  \n", operation->oper1->name);
				}
			break;
			default :
				newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %r8");
				fputs(result,archivo);

				if(flag == 3 || flag == 7){
					printf("	movq -%d(%%rbp),%%r8\n",(operation->oper1->offSet)*REG_SIZE);
				}
			break;
		}
    break;

    case 6:
		switch (operation->oper1->type){
			case CONSTANT :
				newAssemblyString("	movq $", (operation->oper1->value) , 3 , ", %r9");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq $%d, %%r9  \n", operation->oper1->value);
				}
			break;
			case VARGLOBAL :
				newAssemblyVarGlobal ("	movq _", operation->oper1->name, "(%rip), %r9");
				fputs(result,archivo);
				if(flag == 3 || flag == 7){
					printf("	movq _%s(%%rip), %%r9  \n", operation->oper1->name);
				}
			break;
			default :
				newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %r9");
				fputs(result,archivo);

				if(flag == 3 || flag == 7){
					printf("	movq -%d(%%rbp),%%r9\n",(operation->oper1->offSet)*REG_SIZE);
				}
			break;
		}
    break;

    default:
		if (operation->oper1->type==REGISTER){
			strcpy(result, "	pushq %rbx\n");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
      			printf("	pushq %%rbx\n");
			}
		}
		else{
			switch (operation->oper1->type){
				case CONSTANT :
					newAssemblyString("	pushq $", (operation->oper1->value) , 3 , "");
					fputs(result,archivo);
					if(flag == 3 || flag == 7){
						printf("	pushq $%d  \n", operation->oper1->value);
					}
				break;
				case VARGLOBAL :
					newAssemblyVarGlobal ("	pushq _", operation->oper1->name, "(%rip)");
					fputs(result,archivo);
					if(flag == 3 || flag == 7){
						printf("	psuh _%s(%%rip)\n", operation->oper1->name);
					}
				break;
				default :
					newAssemblyString("	pushq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp)");
					fputs(result,archivo);
					if(flag == 3 || flag == 7){
      					printf("	pushq -%d(%%rbp)\n",(operation->oper1->offSet)*REG_SIZE);
					}
				break;
			}
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
	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);
	if(flag == 3 || flag == 7){
		printf("	call %s\n",operation->oper1->name);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
	}
	int i =operation->result->value;
	if (i>6){
		newAssemblyString("	addq $", (i-6)*REG_SIZE , 3 , ", %rsp");
		fputs(result,archivo);
		if(flag == 3 || flag == 7){
			printf("	addq $%d , %%rsp\n", (i-6)*REG_SIZE);
		}
	}
}

/* Genera las lineas de código assembler correspondientes al inicio de una función. */
void generateBeginFunc(OpThreeDir *operation){
	bool isMain = (strcmp("main",operation->result->name)==0);
	if (isMain){
		strcpy(result,"");
		if(os!=0){
			strcat(result, "_");
		}
		strcat(result, (operation->result->name));
		strcat(result, ":\n");
		fputs(result,archivo);
	}
	else {
		strcpy(result, (operation->result->name));
		strcat(result, ":\n");
		fputs(result,archivo);
	}
	//si el stackSize es impar, entonces el tamaño del stack no es multiplo de 16
	if ((operation->stackSize%2) != 0){
		operation->stackSize++;
	}
	newAssemblyString("	enter $", (operation->stackSize*REG_SIZE),3, ", $0");
	fputs(result,archivo);

	//le agrego al nombre de la funcion fin para dsp generar label de fin de funcion
	strcpy(jumpRet,operation->result->name);
	strcat(jumpRet, "fin");

	if(flag == 3 || flag ==7){
		if(isMain){
			if(os==0){
				printf("%s:\n", operation->result->name);
			} else {
				printf("_%s:\n", operation->result->name);
			}
		} else {
			printf("%s:\n",operation->result->name);
		}
		printf("	enter $%d,$0\n",operation->stackSize*REG_SIZE);
	}
}

/* Genera las lineas de código assembler correspondientes al fin de una función. */
void generateEndFunc(OpThreeDir *operation){
	strcpy(result, jumpRet);
	strcat(result, ":\n");
	fputs(result,archivo);
	strcpy(result, "	leave\n");
	fputs(result,archivo);

	strcpy(result, "	retq\n");
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("%s:\n",jumpRet);
		printf("	leave\n");
		printf("	retq\n");
	}
	
	bool condicion = ((prints));
	if ((strcmp("mainfin",jumpRet)==0) && (prints)){
		strcpy(result, "_print:\n");
		fputs(result,archivo);

		strcpy(result, "enter $16, $0\n");
		fputs(result,archivo);

		strcpy(result, "	movq	%rdi, %rax\n");
		fputs(result,archivo);

		strcpy(result, "	leaq	_printAux(%rip), %rdi\n");
		fputs(result,archivo);

		strcpy(result, "	movq	%rax, %rsi\n");
		fputs(result,archivo);

		strcpy(result, "	movq	$0, %rax\n");
		fputs(result,archivo);
		
		if(os!=0){
			strcpy(result, "	callq _printf\n");
		}
		else{
			strcpy(result, "	callq printf@PLT\n");
		}
		fputs(result,archivo);

		strcpy(result, "	xorq %rsi, %rsi\n");
		fputs(result,archivo);

		strcpy(result, "	leave\n");
		fputs(result,archivo);

		strcpy(result, "	retq\n\n");
		fputs(result,archivo);

		strcpy(result, "_printAux: \n");
		fputs(result,archivo);

		if(os!=0){
			strcpy(result, ".asciz	\"Print:  %d \\n\"");
		}
		else{
			strcpy(result, ".string	\"Print:  %d \\n\"");
		}
		fputs(result,archivo);
		if(flag == 3 || flag == 7){
			printf("_print:\n");
			printf("enter $16 , $0\n");
			printf("	movq	%%rdi, %%rax\n");
			printf("	leaq	_printAux(%%rip), %%rdi\n");
			printf("	movq	%%rax, %%rsi\n");
			printf("	movq	$0, %%rax\n");
			if(os!=0){
				printf("	callq	_printf\n");
			}
			else{
				printf("	callq printf@PLT\n");
			}
			printf("	xorq	%%rsi, %%rsi\n");
			printf("	leave\n");
			printf("	retq\n\n");
			printf("_printAux: \n");
			if(os!=0){
				printf(".asciz	\"Print:  %%d \\n\"");
			}
			else{
				printf(".string	\"Print:  %%d \\n\"");
			}
		}
	}
}

/* Genera las lineas de código assembler correspondientes a la lectura de parámetros. */
void generateLoadParam(OpThreeDir *operation){
	int i = operation->oper1->value;
	switch (i){

    case 1:
    	newAssemblyString("	movq %rdi, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

      	if(flag == 3 || flag == 7){
      		printf("	movq %%rdi, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
    break;
    
    case 2:
    	newAssemblyString("	movq %rsi, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq %%rsi, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
    break;
          
    case 3:
    	newAssemblyString("	movq %rdx, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq %%rdx,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
    break;
          
    case 4:
    	newAssemblyString("	movq %rcx, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq %%rcx,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
    break;

    case 5:
    	newAssemblyString("	movq %r8, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq %%r8,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
    break;

    case 6:
    	newAssemblyString("	movq %r9, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("  movq %%r9,  -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
    break;

    default:
    	newAssemblyString("	movq ", ((operation->oper1->value-6)*REG_SIZE)+REG_SIZE ,3, "(%rbp), %rax");
		fputs(result,archivo);

    	newAssemblyString("	movq  %rax, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

      	if(flag == 3 || flag == 7){
      		printf("  	movq %d(%%rbp) , %%rax\n", ((operation->oper1->value-6)*REG_SIZE)+REG_SIZE);
      		printf("	movq %%rax, -%d(%%rbp)\n",((operation->result->offSet)*REG_SIZE));
      	}

    break;
  }	
}