////EN BUSQUEDAS DE VARIABLES Y FUNCIONES FALTA DIFERENCIAR
///FALTA CHEQUEAR QUE POR LLAMADA A FUNCION QUE AL MENOS RETORNE ALGO

/*
Libreria que contiene los tipos y funciones necesarias para generar la tabla
de simbolos y el arbol sintactico de la gramatica para el PreProyecto. Ademas contiene
la semantica de las funciones suma producto y parentesis
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


/*constantes para definir tipo de valor en los items de lista
y tablas (si son variables ,constantes o operaciones)*/
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



////////DECLARACION DE TIPOS

typedef struct tree node;
typedef struct itemsFunc itemFunc;
typedef struct items item;
typedef struct symbolTable symbol;
typedef struct paramsCalls paramsCall;
typedef struct parameters parameter;


typedef struct paramsCalls{
  node *param;
  struct paramsCalls *next;
} paramsCall;

/*
Define un item adicional para las funciones del arbol o tabla de simbolos
ret: tipo de retorno de la funcion
value: lista de parametros de la funcion
type: arbol de sentencias de la funcion
*/
typedef struct itemsFunc{
  symbol *params;
  node *tree;
} itemFunc;

/*
Define un item del arbol o tabla de simbolos
name: Si es una variable define su nombe, en caso de ser una operacion define el tipo de operacion (*,+,())
value: valor de la constante o variable
type: tipo de item (operacion, variable o constante)
*/
typedef struct items{
  char name[32];
  int value;
  int type;
  int ret;
  itemFunc *function;
  paramsCall *paramsCall;
} item;

/*
 Tabla de simbolos del lenguaje implementada mediante una linked list de items
*/
typedef struct symbolTable{
  item *content;
  struct symbolTable *next;
} symbol;

/*
Arbol de evaluacion del lenguaje.
*/
typedef struct tree{
  item *content;
  struct tree *middle;
  struct tree *left;
  struct tree *right;
  int lineNo;
} node;


// Table
int isEmpty();
int isFull();
void openLevel();
void closeLevel();
int typeLastVar();
item * findVar(char n[32],int type,bool debug);
item * findFunction(char n[32],bool debug);
void insertTable(char n[32], int v, int t, int r, bool debug);

// List
item * findInList(symbol *head, char n[32],bool debug);
void insertList(symbol *head, char n[32], int v, int t ,int r, bool debug);
void insertFunction(char n[32], int v, int t, int r, symbol *p, node *tree, bool debug);
symbol * initParamCall();
void addParamCall(paramsCall *l, node *p, bool debug);

// Tree
node * insertTree (char n[32], int v, int t, int r, int lineNo, bool debug);
node * insertVoidNode(int lineNo);
void concatLeft (node *father, node *son);
void concatRight (node *father, node *son);
void concatMiddle (node *father, node *son);

//Checkeo de tipos
int evalExpr (node *head);
int checkOpRel(node *head);
int checkOpAritBin(node *head);
int checkOpAritUn(node *head);
int checkOpLogBin(node *head);
int checkOpLogUn(node *head);
int checkOpEqual(node *head);
int checkOpRel(node *head);
void checkTree (node *head, int functionRet, bool debug);
void checkParams (node *head);
void checks (symbol *head, bool debug);

//////VARIABLES GLOBALES/////

int MAXSIZE = 20;
symbol *levels[20];
int top = -1;


///////IMPLEMENTACION DE FUNCIONES/////////
int isEmpty() {
  if(top == -1)
    return 1;
  else
    return 0;
}

int isFull() {
  if(top == MAXSIZE)
    return 1;
  else
    return 0;
}

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

void closeLevel(){
  if(!isEmpty()){
    top = top - 1;
  } else {
    printf("No levels open.\n");
  }
}

int typeLastVar(){
  symbol *aux = levels[top];
  if ((aux->next)!=NULL){
    while (aux->next!=NULL)
      aux = aux->next;
  }
  return (aux->content)->ret;
}

item * findVar(char n[32],int type,bool debug){
  int i = top;
  item *aux = (item *) malloc(sizeof(item));
  if(debug){
      printf("Searching for variable %s\n",n);
  }
  while (i>=0){
    aux = findInList(levels[i],n,debug);
    if(aux != NULL){
      return aux;
    }
    i = i - 1;
  }
  return NULL;
}

item * findFunction (char n[32], bool debug){
  item *aux = (item *) malloc(sizeof(item));
  if(debug){
      printf("Searching for function %s\n",n);
  }
  aux = findInList(levels[0],n,debug);
  if(aux != NULL){
    return aux;
  }
  return NULL;
}

void insertTable(char n[32], int v, int t, int r,bool debug){
  if(debug){
    printf("Inserting variable %s ... \n",n);
  }
  insertList(levels[top],n,v,t,r,debug);
}

/*
Busca un elemento en la tabla de simbolos
*/
item * findInList(symbol *head,char n[32],bool debug){
  symbol *aux = head;
  if((aux->next)!=NULL){
    aux=aux->next;
    while(aux!=NULL && strcmp((aux->content)->name,n)){
      if(debug){
        printf("    Compare to %s\n",aux->content->name);
      }
      aux = aux->next;
    }
    if(aux == NULL){
      if(debug){
        printf("  Not found\n");
      }
      return NULL;
    } else {
      if(debug){
        printf("  Found\n");
      }
      return aux->content;
    }
  } 
  return  NULL;
}

/*
Inserta un elemento en la tabla de simbolos
*/
void insertList(symbol *head,char n[32], int v, int t,int r, bool debug){
  if(debug){
    printf("  Looking for previous occurrence ... \n");
  }
  if (findInList(head,n,debug)){
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

void insertFunction(char n[32], int v, int t, int r, symbol *p, node *tree, bool debug){
  if (debug){
    printf("Inserting function %s ...\n",n);
  }
  if (findFunction(n,debug)){
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

/*
Crea un elemento de tipo arbol con sus hijos NULL,
si el elemento es de tipo Var busca sus datos en la tabla de Simbolos
*/
node * insertTree (char n[32], int v, int t, int r, int lineNo, bool debug){
  //printf("Begin insertTree\n");
  if(debug){
    printf("Inserting in tree %s ",n);
  }
  item *content;
  if (t == ASSIGN){
    if(debug){
      printf("(assign) ...\n");
    }
    if (!findVar(n,t,debug)){ //chequeo de variable declarada
      fprintf(stderr, "Error: var %s undeclared.  %d \n", n, lineNo);
      exit(EXIT_FAILURE);
    }else{
      content = (item *) malloc(sizeof(item));
      item *contentAux = findVar(n,t,debug);
      strcpy(content->name,contentAux->name);
      content->value = v;
      content->type = ASSIGN;
      //printf("ret %d\n", contentAux->ret);
      //printf("r %d\n", r);
      if (contentAux->ret==r){
        content->ret = contentAux->ret;
      }else{
        fprintf(stderr, "Error: Error en tipos de assignacion %d\n" ,lineNo);
        exit(EXIT_FAILURE);
      }
    }
  }
  if (t == VAR){
      if(debug){
        printf("(variable) ...\n");
      }
      content = (item *) malloc(sizeof(item));
      item *contentAux = findVar(n,t,debug);
    if (!contentAux){
        fprintf(stderr, "Error: %s undeclared  %d \n", n, lineNo);
        exit(EXIT_FAILURE);
    }else{
      strcpy(content->name,contentAux->name);
      content->value =contentAux->value;
      content->type = VAR;
      content->ret = contentAux->ret;
    }
  }else{
    if(debug){
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

/*
agrega el arbol son como hijo izquierdo del arbol father
*/
void concatLeft (node *father, node *son){
  father->left=son;
}

/*
agrega el arbol son como hijo derecho del arbol father
*/
void concatRight (node *father, node *son){
  father->right=son;
}

/*
agrega el arbol son como hijo del medio del arbol father
*/
void concatMiddle (node *father, node *son){
  father->middle=son;
}

symbol * initParamCall(){
  symbol *head = (symbol *) malloc(sizeof(symbol));
  head->next = NULL;
  return head;
}

void addParamCall(paramsCall *head,node *p, bool debug){
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

void checks (symbol *head, bool debug){
  if(debug){
    printf("Checking ...\n");
  }
  symbol *aux = head;
  int r;
  while((aux->next)!=NULL){
    aux=aux->next;
    if (aux->content->type==FUNCTION){
      if(debug){
        printf("Checking function %s ...\n",aux->content->name);
      }
      checkTree(aux->content->function->tree, aux->content->ret, debug);
    }
  }
}


void checkTree (node *head, int functionRet, bool debug){
/*constantes para definir tipo de valor en los items de lista
y tablas (si son variables ,constantes o operaciones)*/
  int ret;
  bool hasRet = false;
  if ((head->content)->type == FUNCTION){
    if(debug){
        printf("  Checking function %s ...\n",head->content->name);
    }
    checkTree(head->content->function->tree, functionRet, debug);
  }
  if ((head->content)->type == FUNCTION_CALL_NP){
    if(debug){
        printf("  Checking params ... \n");
    }
    checkParams(head);
    if(debug){
        printf("  Checking function %s ... \n",head->content->name);
    }
    checkTree(head->content->function->tree, functionRet, debug);
  }  
  if ((head->content)->type == FUNCTION_CALL_P){
    if(debug){
        printf("  Checking params ... \n");
    }
    checkParams(head);
    if(debug){
        printf("  Checking function %s ... \n",head->content->name);
    }
    checkTree(head->content->function->tree, functionRet, debug);
  }  
  if ((head->content)->type == IFAUX){
    if(debug){
        printf("  Evaluating expression %s ... \n",head->content->name);
    }
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion if debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->right, functionRet, debug);
  }  
  if ((head->content)->type == IF_ELSE){
    if(debug){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion if debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->middle, functionRet, debug);
    checkTree(head->right, functionRet, debug);

  }  
  if ((head->content)->type == ASSIGN){
    if(debug){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    if ((head->content)->ret != ret){
        fprintf(stderr, "Error: Error de tipos asignacion en asignacion, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE); 
    }
  }  
  if ((head->content)->type == WHILEAUX){
    if(debug){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion while debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->right, functionRet, debug);
  }  
  if ((head->content)->type == RETURNAUX){
    if (functionRet!=VOIDAUX){
        fprintf(stderr, "Error: Return no devuelve nada y la funcion devuelve un valor, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
  }  
  if ((head->content)->type == RETURN_EXPR){
    if(debug){
        printf("  Evaluating expression ... \n");
    }
    ret = evalExpr (head->left);
    if (functionRet!=ret){
        fprintf(stderr, "Error: Error en de tipo expresion return, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
  }  
  if ((head->content)->type == STATEMENTS){
    checkTree(head->left, functionRet, debug);
    checkTree(head->right, functionRet, debug);
  }
  if ((head->content)->type == BLOCK){
    checkTree(head->left, functionRet, debug);
  }
} 

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

int checkOpAritUn(node *head){
    int left = evalExpr(head->left);
    if(left == BOOLAUX){
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
      return BOOLAUX;
    }
    else {
        fprintf(stderr, "Error: Error en tipos de expresion en linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
}

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

