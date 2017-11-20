
/*
	Libreria que contiene los tipos y funciones necesarias para generar la tabla
	de símbolos y el árbol sintáctico de la grámatica para el proyecto.
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/* 
	Constantes para definir tipo de valor en los items de lista
	y tablas (variables, constantes, operaciones, etc.).
*/
#define VAR 0
#define CONSTANT 1
#define OPER_AR 2
#define OPER_LOG 3
#define OPER_REL 4
#define INTEGERAUX 5
#define BOOLAUX 6
#define VOIDAUX 7
#define ERROR 8
#define INDETERMINATE 9
#define FUNCTION 10
#define FUNCTION_CALL_NP 11
#define FUNCTION_CALL_P 12
#define PARAMETER 13
#define IFAUX 14
#define IF_ELSE 15
#define ASSIGN 16
#define WHILEAUX 17
#define RETURNAUX 18
#define RETURN_EXPR 19
#define STATEMENTS 20
#define BLOCK 21
#define OPER_AR_UN 22
#define OPER_LOG_UN 23
#define OPER_EQUAL 24
#define PRINTAUX 25
#define REGISTER 27
#define VARGLOBAL 26






/* DECLARACIÓN DE TIPOS */

typedef struct tree node;
typedef struct itemsFunc itemFunc;
typedef struct items item;
typedef struct symbolTable symbol;
typedef struct paramsCalls paramsCall;
typedef struct parameters parameter;

/* Define una lista de parametros para utilizar en la llamada de la función.
	param: Árbol que representa el parametro como una expresión.
	next: Siguiente parametro.
*/
typedef struct paramsCalls{
	node *param;
	struct paramsCalls *next;
} paramsCall;

/* Define un item adicional para las funciones del árbol o tabla de símbolos.
	ret: Tipo de retorno de la función.
	value: Lista de parámetros de la función.
	type: Árbol de sentencias de la función.
*/
typedef struct itemsFunc{
	symbol *params;
	node *tree;
	int paramsSize;
	int stackSize;
} itemFunc;

/* Define un item del árbol o tabla de símbolos.
	name: Si es una variable define su nombre, en caso de ser una operacion define el tipo de operacion (*,+,(,etc.).
	value: Valor de la constante o variable.
	type: Tipo de item (operacion, variable, constante, etc.).
*/
typedef struct items{
	char name[32];
	int value;
	int type;
	int ret;
	int offSet;
	itemFunc *function;
	paramsCall *paramsCall;
} item;

/* Tabla de símbolos del lenguaje.
	content: Item que representa el símbolo.
	next: Siguiente elemento.
*/
typedef struct symbolTable{
	item *content;
	struct symbolTable *next;
} symbol;

/* Árbol de evaluación del lenguaje.
	content: Nodo del árbol (tipo item).
	middle: Árbol hijo medio (para el caso del if).
	left: Árbol hijo izquierdo.
	rigth: Árbol hijo derecho.
*/
typedef struct tree{
	item *content;
	struct tree *middle;
	struct tree *left;
	struct tree *right;
	int lineNo;
} node;


/* DECLARACIÓN DE FUNCIONES */

// Tabla de símbolos
int isEmpty();
int isFull();
void openLevel();
void closeLevel();
int typeLastVar();
item * findVar(char n[32],int type, int flag);
item * findFunction(char n[32], int flag);
void insertTable(char n[32], int v, int t, int r, bool var, int offSet, int flag);

// Listas
item * findInList(symbol *head, char n[32], int flag);
void insertList(symbol *head, char n[32], int v, int t ,int r, bool var, int offSet, int flag);
void insertFunction(char n[32], int v, int t, int r, symbol *p, node *tree, int functionVariables, int flag);
symbol * initParamCall();
void addParamCall(paramsCall *l, node *p, int flag);

// Árbol de evaluación
node * insertTree (char n[32], int v, int t, int r, int lineNo, int flag);
node * insertVoidNode(int lineNo);
void concatLeft (node *father, node *son);
void concatRight (node *father, node *son);
void concatMiddle (node *father, node *son);

// Checkeo de tipos
void checks (symbol *head, int flag);
void checkParams (node *head);
void checkTree (node *head, int functionRet, int flag);
int evalExpr (node *head);
int checkOpAritUn(node *head);
int checkOpLogUn(node *head);
int checkOpAritBin(node *head);
int checkOpLogBin(node *head);
int checkOpRel(node *head);
int checkOpEqual(node *head);


/* VARIABLES GLOBALES */

/* Arreglo que simula una pila para manejar los niveles de la tabla. */
int MAXSIZE = 20;
symbol *levels[20];
int top = -1;


/* IMPLEMENTACION DE FUNCIONES */

/* TABLA DE SÍMBOLOS*/

/* Controla si la pila de niveles esta vacía. */
int isEmpty() {
	if(top == -1)
		return 1;
	else
		return 0;
}

/* Controla si la pila de niveles esta llena. */
int isFull() {
	if(top == MAXSIZE)
		return 1;
	else
		return 0;
}

/* Abre un nuevo nivel en la pila. Para ello, controla que no este llena y
	 genera el elemento ficticio que será el primer elemento de la lista. */
void openLevel(){
	if(!isFull()){
		top = top + 1;
		symbol *head = (symbol *) malloc(sizeof(symbol));
		head->next = NULL;
		levels[top]=head;
	} else {
		printf("Could not open a new level.\n");
	}
}

/* Cierra el último nivel de la pila. Para ello, controla que no este llena y
	 genera el elemento ficticio que será el primer elemento de la lista. */
void closeLevel(){
	if(!isEmpty()){
		top = top - 1;
	} else {
		printf("No levels open.\n");
	}
}

/* Recupera el tipo de la última variable guardada. Esto se utiliza para 
	 registrar varias variables del mismo tipo separadas por comas. */
int typeLastVar(){
	symbol *aux = levels[top];
	if ((aux->next)!=NULL){
		while (aux->next!=NULL)
			aux = aux->next;
	}
	return (aux->content)->ret;
}

/* Busca una variable en la tabla de símbolos. Para ello, comienza por el
   último nivel abierto y si no encuentra ocurrecia de la misma, retrocede
   a los niveles más antiguos. Ante la primera ocurrencia, devuelve el item
   correspondiente. */
item * findVar(char n[32], int type, int flag){
  int i = top;
  item *aux = (item *) malloc(sizeof(item));
  if(flag==5){
      printf("Searching for variable %s\n",n);
  }
  while (i>=0){
    aux = findInList(levels[i],n,flag);
    if(aux != NULL){
      return aux;
    }
    i = i - 1;
  }
  return NULL;
}

/* Busca una variable en la tabla de símbolos, unicamente en el nivel base (el
   más antiguo de todos), ya que las funciones no pueden estar anidadas. Ante 
   la primera ocurrencia, devuelve el item correspondiente.*/
item * findFunction (char n[32], int flag){
  item *aux = (item *) malloc(sizeof(item));
  if(flag==5){
      printf("Searching for function %s\n",n);
  }
  aux = findInList(levels[0],n,flag);
  if(aux != NULL){
    if(flag == 5){
      printf("  Found\n");
    }
    return aux;
  }
  if(flag == 5){
      printf("  Not found\n");
  }
  return NULL;
}

/* Inserta una variable en la tabla de símbolos, en el nivel correspondiente.
   Para ello, llama a insertList con el nivel en donde se insertará.*/
void insertTable(char n[32], int v, int t, int r, bool var, int offSet, int flag){
  if(flag==5){
    printf("Inserting variable %s ... \n",n);
  }
  insertList(levels[top],n,v,t,r,var,offSet,flag);
}

/* LISTAS */

/* Busca un elemento en la tabla de simbolos, en un nivel particular (compara 
   solo por nombre). */
item * findInList(symbol *head,char n[32], int flag){
  symbol *aux = head;
  if((aux->next)!=NULL){
    aux=aux->next;
    while(aux!=NULL && strcmp((aux->content)->name,n)){
      if(flag==5){
        printf("    Compare to %s\n",aux->content->name);
      }
      aux = aux->next;
    }
    if(aux == NULL){
      if(flag==5){
        printf("  Not found\n");
      }
      return NULL;
    } else {
      if(flag==5){
        printf("  Found\n");
      }
      return aux->content;
    }
  } 
  return  NULL;
}

/* Inserta un elemento en la tabla de símbolos. Para ello, primero busca 
   una ocurrencia previa de lo que se quiere insertar. */
void insertList(symbol *head,char n[32], int v, int t,int r, bool var, int offSet, int flag){
  if(flag==5){
    printf("  Looking for previous occurrence ... \n");
  }
  if (findInList(head,n,flag)){
    fprintf(stderr, "Error: %s declared before\n", n);
    exit(EXIT_FAILURE);
  }else{
    symbol *element;
    element = (symbol *) malloc(sizeof(symbol));
    item *content;
    content = (item *) malloc(sizeof(item));
    strcpy(content->name,n);
    content->value = v;
    content->type = t;
    content->ret = r;
    if(var){
      content->offSet = offSet;
    }
    element->content = content;
    element->next = NULL;
    if(head->next==NULL){
      head->next = element;
    }else{
      symbol *aux = head->next;
      while(aux->next != NULL){
        aux = aux->next;
      }
      aux->next = element;
    }
  }
}

/* Inserta una funcion en la tabla de símbolos. Para ello, primero busca 
   una ocurrencia previa de la que se quiere insertar. */
void insertFunction(char n[32], int v, int t, int r, symbol *p, node *tree, int functionVariables, int flag){
  if (flag==5){
    printf("Inserting function %s ...\n",n);
  }
  if (findFunction(n,flag)){
    fprintf(stderr, "Error: %s declared before\n", n);
    exit(EXIT_FAILURE);
  }else{
    symbol *element;
    element = (symbol *) malloc(sizeof(symbol));
    item *content;
    content = (item *) malloc(sizeof(item));
    itemFunc *contentFunc;
    contentFunc = (itemFunc *) malloc(sizeof(itemFunc));
    strcpy(content->name,n);
    content->value = v;
    content->type = t;
    content->ret=r;
    contentFunc->params = p;
    contentFunc->tree = tree;
    contentFunc->stackSize = functionVariables;
    content->function= contentFunc;
    element->content = content;
    element->next = NULL;
    symbol *head = levels[0];
    if(head->next==NULL){
      head->next = element;
    } else {
      symbol *aux = head->next;
      while(aux->next != NULL){
        aux = aux->next;
      }
      aux->next = element;
    }
  }
}

/* Inicializa la lista de parametros utilizados en la llamada 
	 a una función. */
symbol * initParamCall(){
	symbol *head = (symbol *) malloc(sizeof(symbol));
	head->next = NULL;
	return head;
}

/* Agrega una nueva expresión que representa un parameto a la lista 
   de parametros utlizados en la llamada a una función. */
void addParamCall(paramsCall *head,node *p, int flag){
  paramsCall *element = (paramsCall *) malloc(sizeof(paramsCall));
  element->param = p;
  element->next = NULL;
  if(head->next==NULL){
      head->next = element;
    } else {
      paramsCall *aux = head->next;
      while(aux->next != NULL){
        aux = aux->next;
      }
      aux->next = element;
    }
}

/* ÁRBOL DE EVALUACIÓN */

/* Crea un elemento de tipo árbol con sus hijos en NULL. Si el elemento 
es de tipo Var busca sus datos en la tabla de símbolos. */
node * insertTree (char n[32], int v, int t, int r, int lineNo, int flag){
  if(flag==5){
    printf("Inserting in tree %s ",n);
  }
  item *content;
  if (t == ASSIGN){
    if(flag==5){
      printf("(assign) ...\n");
    }
    //medio al pedo ahora
    if (!findVar(n,t,flag)){ //chequeo de variable declarada
      fprintf(stderr, "Error: var %s undeclared.  %d \n", n, lineNo);
      exit(EXIT_FAILURE);
    }else{
      content = (item *) malloc(sizeof(item));
      item *contentAux = findVar(n,t,flag);
      strcpy(content->name,contentAux->name);
      content->value = v;
      content->type = ASSIGN;
    }
  }
  if (t == VAR || t == PARAMETER){
      if(flag==5){
        printf("(variable) ...\n");
      }
      content = (item *) malloc(sizeof(item));
      item *contentAux = findVar(n,t,flag);
    if (!contentAux){
        fprintf(stderr, "Error: %s undeclared  %d \n", n, lineNo);
        exit(EXIT_FAILURE);
    }else{
      content = contentAux;
    }
  }else{
    if(flag==5){
      printf("...\n");
    }
    content = (item *) malloc(sizeof(item));
    strcpy(content->name,n);
    content->value = v;
    content->type = t;
    content->ret = r;
  }
  node *element;
  element = (node *) malloc(sizeof(node));
  element->content = content;
  element->lineNo = lineNo;
  //printf("End insertTree\n");
  element->right=NULL;
  element->left=NULL;
  element->middle=NULL;
  return element;
}

/* Inserta un nodo "vacío" en el árbol. Utilizado para bloques 
	vacíos, solo con declaraciones de variables y puntos y comas 
	huérfanos. */
node * insertVoidNode(int lineNo){
	node *element;
	element = (node *) malloc(sizeof(node));
	element->content = NULL;
	element->lineNo = lineNo;
	element->right = NULL;
	element->left = NULL;
	element->middle = NULL;  
	return element;
}

/* Agrega el árbol parametro 'son' como hijo izquierdo del 
	 árbol parametro 'father'. */
void concatLeft (node *father, node *son){
	father->left=son;
}

/* Agrega el árbol parametro 'son' como hijo derecho del 
	 árbol parametro 'father'. */
void concatRight (node *father, node *son){
	father->right=son;
}

/* Agrega el árbol parametro 'son' como hijo medio del 
	 árbol parametro 'father'. */
void concatMiddle (node *father, node *son){
	father->middle=son;
}

/* CHEQUEO DE TIPOS */

/* Recorre los niveles el nivel base de la tabla de símbolos en busca de funciones 
   y por cada una invoica a checkTree con el árbol correspondiente. */
void checks (symbol *head, int flag){
  if(flag==5){
    printf("Checking ...\n");
  }
  symbol *aux = head;
  int r;
  while((aux->next)!=NULL){
    aux=aux->next;
    if (aux->content->type==FUNCTION){
      if(flag==5){
        printf("Checking function %s ...\n",aux->content->name);
      }
      checkTree(aux->content->function->tree, aux->content->ret, flag);
    }
  }
}

/* Controla los parametros formales y actuales, entre la declaración de 
	 la función guardada en la tabla de simbolos, y la lista de párametros 
	 que se utilizara en su invocación. Chequea cantidad, tipos y orden. */
void checkParams (node *head){
	paramsCall *pc= head->content->paramsCall;
	symbol *pl= head->content->function->params;
	if (pl!=NULL && pc!=NULL){
		if  ((pl->next!=NULL && pc->next==NULL)){
			fprintf(stderr, "Error: mas parametros en la declaracion que en la llamada, Linea %d\n" ,head->lineNo);
			exit(EXIT_FAILURE);
		}
		if  ((pl->next==NULL && pc->next!=NULL)){
			fprintf(stderr, "Error: mas parametros en la llamada que en la declaracion, Linea %d\n" ,head->lineNo);
			exit(EXIT_FAILURE);
		}
		int ret;
		while (pl->next!=NULL && pc->next!=NULL){
			pl = pl->next;
			pc = pc->next;
			ret = evalExpr(pc->param);
			if (pl->content->ret !=ret){
				fprintf(stderr, "Error: error de tipos llamada metodo, Linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
			}
			if  ((pl->next!=NULL && pc->next==NULL)){
				fprintf(stderr, "Error: mas parametros en la declaracion que en la llamada, Linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
			}
			if  ((pl->next==NULL && pc->next!=NULL)){
				fprintf(stderr, "Error: mas parametros en la llamada que en la declaracion, Linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
			}
		}
	}
}

/* Realiza un chequeo de tipos sobre la función pasada como parámetro 
   en base al árbol que la representa. */
void checkTree (node *head, int functionRet, int flag){
  int ret;
  bool hasRet = false;
  if ((head->content)->type == FUNCTION){
    if(flag==5){
        printf("  Checking function %s ...\n",head->content->name);
    }
    checkTree(head->content->function->tree, functionRet, flag);
  }
  if ((head->content)->type == FUNCTION_CALL_NP){
    if(flag==5){
        printf("  Checking params ... \n");
    }
    checkParams(head);
    if(flag==5){
        printf("  Checking function %s ... \n",head->content->name);
    }
    checkTree(head->content->function->tree, functionRet, flag);
  }  
  if ((head->content)->type == FUNCTION_CALL_P){
    if(flag==5){
        printf("  Checking params ... \n");
    }
    checkParams(head);
    if(flag==5){
        printf("  Checking function %s ... \n",head->content->name);
    }
    checkTree(head->content->function->tree, functionRet, flag);
  }  
  if ((head->content)->type == IFAUX){
    if(flag==5){
        printf("  Evaluating expression %s ... \n",head->content->name);
    }
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion if debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->right, functionRet, flag);
  }  
  if ((head->content)->type == IF_ELSE){
    if(flag==5){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion if debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->middle, functionRet, flag);
    checkTree(head->right, functionRet, flag);

  }  
  if ((head->content)->type == ASSIGN){
    if(flag==5){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    int retaux = evalExpr (head->right);
    if (retaux != ret){
        fprintf(stderr, "Error: Error de tipos asignacion en asignacion, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE); 
    }
  }  
  if ((head->content)->type == WHILEAUX){
    if(flag==5){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion while debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->right, functionRet, flag);
  }  
  if ((head->content)->type == RETURNAUX){
    if (functionRet!=VOIDAUX){
        fprintf(stderr, "Error: Return no devuelve nada y la funcion devuelve un valor, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
  }  
  if ((head->content)->type == RETURN_EXPR){
    if(flag==5){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    head->content->ret = ret;
    if (functionRet!=ret){
        fprintf(stderr, "Error: Error en de tipo expresion return, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
  }
  if ((head->content)->type == PRINTAUX){
    if(flag==5){			
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
  } 
  if ((head->content)->type == STATEMENTS){
    checkTree(head->left, functionRet, flag);
    if (head->right !=NULL){
      checkTree(head->right, functionRet, flag);
    }
  }
  if ((head->content)->type == BLOCK){
    checkTree(head->left, functionRet, flag);
  }
} 

/* Evalua una expresión (en forma de árbol), llamando a la función 
	 correspondiente basandose en su tipo (operadores aritméticos, lógicos, 
	 llamadas a funciones, variables, etc.). */
int evalExpr (node *head){
	int t1 = head->content->type;
	if (t1 == CONSTANT || t1 == VAR || t1 == PARAMETER){
		return head->content->ret;
	}
	if ((t1==FUNCTION_CALL_NP || t1==FUNCTION_CALL_P)){
		checkParams(head);
		return head->content->ret;
	}
	if (t1==OPER_AR){
		return checkOpAritBin(head);
	}  
	if (t1==OPER_LOG){
		return checkOpLogBin(head);
	}
	if (t1==OPER_AR_UN){
		return checkOpAritUn(head);
	}  
	if (t1==OPER_LOG_UN){
		return checkOpLogUn(head);
	}  
	if (t1==OPER_EQUAL){
		return checkOpEqual(head);
	}  
	if (t1==OPER_REL){
		return checkOpRel(head);
	}
	return VOIDAUX;
}

/* Controla que el operador de una operación aritmética unaria sea de tipo entero. */
int checkOpAritUn(node *head){
		int left = evalExpr(head->left);
		if(left == INTEGERAUX){
			return INTEGERAUX;
		}
		else {
				fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
		}
}

/* Controla que el operador de una operación lógica unaria sea de tipo booleano. */
int checkOpLogUn(node *head){
		int left = evalExpr(head->left);
		if(left == BOOLAUX){
			return BOOLAUX;
		}
		else {
				fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
		}
}

/* Controla que los operadores de una operación aritmética binaria sean de tipo entero. */
int checkOpAritBin(node *head){
		int left = evalExpr(head->left);
		int right = evalExpr(head->right);
		if((left == INTEGERAUX) && (right == INTEGERAUX)){
			return INTEGERAUX;
		}
		else {
				fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
		}
}

/* Controla que los operadores de una operación lógica binaria sean de tipo booleano. */
int checkOpLogBin(node *head){
		int left = evalExpr(head->left);
		int right = evalExpr(head->right);
		if((left == BOOLAUX) && (right == BOOLAUX)){
			return BOOLAUX;
		}
		else {
				fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
		}
}

/* Controla que los operadoradores de una operación relacional sean de tipo entero. */
int checkOpRel(node *head){
		int left = evalExpr(head->left);
		int right = evalExpr(head->right);
		if((left == INTEGERAUX) && (right == INTEGERAUX)){
			return BOOLAUX;
		}
		else {
				fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
		}
}


int checkOpEqual(node *head){
		int left = evalExpr(head->left);
		int right = evalExpr(head->right);
		if(left == right){
			head->content->ret = left; //para despues generar el codigo intermedio saber lo q compara
			return BOOLAUX;
		}
		else {
				fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
				exit(EXIT_FAILURE);
		}
}