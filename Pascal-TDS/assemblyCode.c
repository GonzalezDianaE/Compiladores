#include "intermediateCode.c"
#include <stdlib.h>

const int REG_SIZE = 8;
char jumpRet[32];
FILE *archivo;
char result[32]; //utilizado para definir strings y guardarlos en el archivo 
int flag;
int os;


/* DECLARACIÓN DE FUNCIONES */

void generateFile(int flag);
void newAssemblyString(char *c1, int i1, int i2, char *c2);

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
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética adición. */
void generateAdd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) + (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);

		if( flag == 3 || flag == 7){
			printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) + (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	addq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			printf("	addq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE); 
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética sustracción. */
void generateSub(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) - (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) - (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	subq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		
		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			printf("	subq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética multiplicación. */
void generatePlus(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) * (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) * (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	imulq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			printf("	imulq -%d(%%rbp),%%rax\n",(operation->oper2->offSet)*REG_SIZE);
			printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética divisón. */
void generateDiv(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		newAssemblyString("	movq $", ((operation->oper1->value) / (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);	
		
		if(flag == 3 || flag == 7){
			printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) / (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);

		strcpy(result, "	cqto \n");
		fputs(result,archivo);

		newAssemblyString("	idivq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			printf("	cqto \n");
			printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
			printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética módulo. */
void generateMod(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		newAssemblyString("	movq $", ((operation->oper1->value) % (operation->oper2->value)) , ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq $%d, -%d(%%rbp)\n",((operation->oper1->value) % (operation->oper2->value)),((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE) , 3 , "(%rbp), %rax");
		fputs(result,archivo);
		
		strcpy(result, "	cqto \n");
		fputs(result,archivo);

		newAssemblyString("	idivq -", ((operation->oper2->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		newAssemblyString("	movq %rdx, -", ((operation->result->offSet)*REG_SIZE) , 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp),%%rax\n",(operation->oper1->offSet)*REG_SIZE);
			printf("	cqto \n");//extiende %rax para guardar el resto de la division (ver bien esto)
			printf("	idivq -%d(%%rbp)\n",(operation->oper2->offSet)*REG_SIZE);
			printf("	movq %%rdx, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica and. */
void generateAnd(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		if(((operation->oper1->value) && (operation->oper2->value))==0){ //ES FALSE
			newAssemblyString("	movb $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		} else{
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			
			if(flag == 3 || flag == 7){
				printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	andq -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));
			printf("	andq -%d(%%rbp), %%rax\n", ((operation->oper2->offSet)*REG_SIZE));
			printf("	movq %%rax, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica or. */
void generateOr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){	
		if(((operation->oper1->value) || (operation->oper2->value))==0){ //ES FALSE
			newAssemblyString("	movb $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movb $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		} else{
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movb $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	orq -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));
			printf("	orq -%d(%%rbp), %%rax\n", ((operation->oper2->offSet)*REG_SIZE));
			printf("	movq %%rax, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación lógica not. */
void generateNot(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT){	
		if((operation->oper1->value)==0){ //ES FALSE
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		} else{
			newAssemblyString("	movb $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else {
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

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
}

/* Genera las lineas de código assembler correspondientes a  una comparación por igual (aritmética). */
void generateEqAr(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) == (operation->oper2->value)){ //es true
			newAssemblyString("	movq $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		} else{ 
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else{
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	cmpq %rax, -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);

		strcpy(result, "	sete %dl\n");
		fputs(result,archivo);

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);

		newAssemblyString("	movq %rsi, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));
			printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));
			printf("	sete %%dl\n");
			printf("	andb $1 , %%dl\n");
			printf("	movzbl %%dl , %%esi\n");
			printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una comparación por igual (lógica). */
void generateEqLog(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) == (operation->oper2->value)){ //es true
			newAssemblyString("	movq $", 0, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		} else{ 
			newAssemblyString("	movb $", 1, ((operation->result->offSet)*REG_SIZE) , "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else{
		newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE), 3 , "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	cmpq %rax, -", ((operation->oper2->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);

		strcpy(result, "	sete %dl\n");
		fputs(result,archivo);

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);

		newAssemblyString("	movq %rsi, -", ((operation->result->offSet)*REG_SIZE), 3 , "(%rbp)");
		fputs(result,archivo);
		
		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));
			printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));
			printf("	sete %%dl\n");
			printf("	andb $1 , %%dl\n");
			printf("	movzbl %%dl , %%esi\n");
			printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una operación aritmética negación. */
void generateNeg(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT){	
		newAssemblyString("	movq $",(-(operation->oper1->value)),((operation->result->offSet)*REG_SIZE), "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq $%d, -%d(%%rbp)\n",(-(operation->oper1->value)),((operation->result->offSet)*REG_SIZE));
		}
	} else {
		newAssemblyString("	movq -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rax");
		fputs(result,archivo);

		strcpy(result, "	negq %rax\n");
		fputs(result,archivo);

		newAssemblyString("	movq %rax, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n",(operation->oper1->offSet)*REG_SIZE);
			printf("	negq %%rax\n");
			printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);	
		}	
	}
}


/* Genera las lineas de código assembler correspondientes a una operación aritmética minnor. */
void generateMinnor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) < (operation->oper2->value)){ //es true
			newAssemblyString("	movq $",1,((operation->result->offSet)*REG_SIZE), "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		} else{ 
			newAssemblyString("	movq $",0,((operation->result->offSet)*REG_SIZE), "(%rbp)");
			fputs(result,archivo);
			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else{
		newAssemblyString("	movq -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	cmpq %rax, -",((operation->oper2->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

		strcpy(result, "	setg %dl\n");
		fputs(result,archivo);

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);

		newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));
			printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));
			printf("	setg %%dl\n");
			printf("	andb $1 , %%dl\n");
			printf("	movzbl %%dl , %%esi\n");
			printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
		}
	}
}


/* Genera las lineas de código assembler correspondientes a una operación aritmética major. */
void generateMajor(OpThreeDir *operation){
	if(operation->oper1->type == CONSTANT && operation->oper2->type == CONSTANT){
		if((operation->oper1->value) > (operation->oper2->value)){ //es true
			newAssemblyString("	movq $",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",1,((operation->result->offSet)*REG_SIZE));
			}
		} else{ 
			newAssemblyString("	movq $",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
			fputs(result,archivo);

			if(flag == 3 || flag == 7){
				printf("	movq $%d, -%d(%%rbp)\n",0,((operation->result->offSet)*REG_SIZE));
			}
		}
	} else{
		newAssemblyString("	movq -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rax");
		fputs(result,archivo);

		newAssemblyString("	cmpq %rax, -",((operation->oper2->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

		strcpy(result, "	setl %dl\n");
		fputs(result,archivo);

		strcpy(result, "	andb $1, %dl\n");
		fputs(result,archivo);

		strcpy(result, "	movzbl %dl, %esi\n");
		fputs(result,archivo);

		newAssemblyString("	movq %rsi, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	movq -%d(%%rbp), %%rax\n", ((operation->oper1->offSet)*REG_SIZE));
			printf("	cmpq %%rax, -%d(%%rbp)\n", ((operation->oper2->offSet)*REG_SIZE));
			printf("	setl %%dl\n");
			printf("	andb $1 , %%dl\n");
			printf("	movzbl %%dl , %%esi\n");
			printf("	movq %%rsi, -%d(%%rbp)\n", ((operation->result->offSet)*REG_SIZE));
		}
	}
}

/* Genera las lineas de código assembler correspondientes a una asignación. */
void generateAssign(OpThreeDir *operation){
	newAssemblyString("	movq -",((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
	fputs(result,archivo);
	
	newAssemblyString("	movq %rax, -",((operation->oper1->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	movq -%d(%%rbp), %%rax\n", (operation->result->offSet)*REG_SIZE);
		printf("	movq %%rax, -%d(%%rbp)\n", ((operation->oper1->offSet)*REG_SIZE));
	}
}

/* Genera las lineas de código assembler correspondientes a un if. */
void generateIfAss(OpThreeDir *operation){
	newAssemblyString("	cmpl $0, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	strcpy(result, "	je ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	cmpl $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);
		printf("	je %s\n", operation->oper1->name);
	}
}

/* Genera las lineas de código assembler correspondientes a un while. */
void generateWhileAss(OpThreeDir *operation){
	newAssemblyString("	cmpl $0, -",((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	strcpy(result, "	je ");
	strcat(result, (operation->oper1->name));
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	cmpl $0, -%d(%%rbp)\n", (operation->result->offSet)*REG_SIZE);
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
	newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
	fputs(result,archivo);

	strcpy(result, "	jmp ");
	strcat(result, jumpRet);
	strcat(result, "\n");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	movq -%d(%%rbp),  %%rax\n",(operation->result->offSet)*REG_SIZE);
		printf("	jmp %s\n",jumpRet);
	}
}

/* Genera las lineas de código assembler correspondientes a un return bool. */
void generateRetBool(OpThreeDir *operation){
	newAssemblyString("	movq -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp), %rax");
	fputs(result,archivo);

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
    	newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rdi");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq -%d(%%rbp),  %%rdi\n",(operation->oper1->offSet)*REG_SIZE);
		}
    break;
    
    case 2:
    	newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rsi");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq -%d(%%rbp),  %%rsi\n",(operation->oper1->offSet)*REG_SIZE);
		}
    break;
          
    case 3:
    	newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rdx");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq -%d(%%rbp),  %%rdx\n",(operation->oper1->offSet)*REG_SIZE);
		}
    break;
          
    case 4:
    	newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %rcx");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq -%d(%%rbp),  %%rcx\n",(operation->oper1->offSet)*REG_SIZE);
		}
    break;

    case 5:
    	newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %r8");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq -%d(%%rbp),  %%r8\n",(operation->oper1->offSet)*REG_SIZE);
		}
    break;

    case 6:
    	newAssemblyString("	movq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp), %r9");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
      		printf("	movq -%d(%%rbp),  %%r9\n",(operation->oper1->offSet)*REG_SIZE);
		}
    break;

    default:
    	newAssemblyString("	pushq -", ((operation->oper1->offSet)*REG_SIZE),3, "(%rbp)");
		fputs(result,archivo);
      	
      	if(flag == 3 || flag == 7){
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

	newAssemblyString("	movq %rax, -", ((operation->result->offSet)*REG_SIZE),3, "(%rbp)");
	fputs(result,archivo);

	if(flag == 3 || flag == 7){
		printf("	call %s\n",operation->oper1->name);
		printf("	movq %%rax, -%d(%%rbp)\n",(operation->result->offSet)*REG_SIZE);
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

	/*if (strcmp("mainfin",jumpRet)==0){
	if(flag == 3 || flag == 7){
		printf("%s:\n",jumpRet);
	}
	
	if (strcmp("mainfin",jumpRet)==0){

		strcpy(result, "	leaq	_print(%rip), %rdi\n");
		fputs(result,archivo);

		strcpy(result, "	movl	%eax, %esi\n");
		fputs(result,archivo);

		strcpy(result, "	movb	$0, %al\n");
		fputs(result,archivo);

		strcpy(result, "	callq _printf\n");
		fputs(result,archivo);

		strcpy(result, "	xorl %esi, %esi\n");
		fputs(result,archivo);

		strcpy(result, "	leave\n");
		fputs(result,archivo);

		strcpy(result, "	retq\n\n");
		fputs(result,archivo);

		strcpy(result, "_print: \n");
		fputs(result,archivo);

		strcpy(result, ".asciz	\"Result:  %d \\n\"");
		fputs(result,archivo);

		if(flag == 3 || flag == 7){
			printf("	leaq	_print(%%rip), %%rdi\n");
			printf("	movl	%%eax, %%esi\n");
			printf("	movb	$0, %%al\n");
			printf("	callq	_printf\n");
			printf("	xorl	%%esi, %%esi\n");
			printf("	leave\n");
			printf("	retq\n\n");
			printf("_print: \n");
			printf(".asciz	\"Result:  %%d \\n\"");
		}
	}
	else{
		strcpy(result, "	leave\n");
		fputs(result,archivo);

		strcpy(result, "	retq\n\n");
		fputs(result,archivo);
		printf("	retq\n");
	}*/

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
    	newAssemblyString("	movq ", ((operation->oper1->value-6)*REG_SIZE)+REG_SIZE ,3, "(%rbp), rax");
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