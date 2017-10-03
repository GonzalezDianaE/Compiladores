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
#define OPER_EQUAL 23



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
item * findVar(char n[32],int type);
item * findFunction(char n[32]);
void insertTable(char n[32], int v, int t, int r);

// List
item * findInList(symbol *head, char n[32],int type);
void insertList(symbol *head, char n[32], int v, int t ,int r);
void insertFunction(char n[32], int v, int t, int r, symbol *p, node *tree);
symbol * initParamCall();
void addParamCall(paramsCall *l, node *p);

// Tree
node * insertTree (char n[32], int v, int t, int r, int lineNo);
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
void checkTree (node *head, int functionRet);
void checkParams (node *head);
void checks (symbol *head);

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

item * findVar(char n[32],int type){
  int i = top;
  item *aux = (item *) malloc(sizeof(item));
  while (i>=0){
    aux = findInList(levels[i],n,type);
    if(aux != NULL){
      return aux;
    }
    i = i - 1;
  }
  return NULL;
}

item * findFunction (char n[32]){
  item *aux = (item *) malloc(sizeof(item));
  aux = findInList(levels[0],n,10);
  if(aux != NULL){
    return aux;
  }
  return NULL;
}

void insertTable(char n[32], int v, int t, int r){
  printf("insert %s (type %d)\n",n,t);
  insertList(levels[top],n,v,t,r);
}

/*
Busca un elemento en la tabla de simbolos
*/
item * findInList(symbol *head,char n[32],int type){
  symbol *aux = head;
  if((aux->next)!=NULL){
    aux=aux->next;
    int auxiliarType = (aux->content)->type;
    printf("Searching for %s (type %d)\n",n,type);
    //while(aux!=NULL && strcmp((aux->content)->name,n)){
    //while(aux!=NULL && !(strcmp((aux->content)->name,n)==0 && type==auxiliarType)){
    //while(aux!=NULL && !(strcmp((aux->content)->name,n)==0 && ((auxiliarType==PARAMETER)?(type==PARAMETER||type==VAR||type==ASSIGN):type==FUNCTION))){
    while(aux!=NULL && !(strcmp((aux->content)->name,n)==0 && ((type==VAR||type==ASSIGN)?(auxiliarType==PARAMETER||auxiliarType==VAR):auxiliarType==FUNCTION))){
      printf("  compare %s (type %d) con %s (type %d)\n",n,type,(aux->content)->name,auxiliarType);
      //printf("aux = %d\n",aux!=NULL);
      //printf("second = %d\n",!(strcmp((aux->content)->name,n)==0 && ((auxiliarType==PARAMETER)?(type==PARAMETER||type==VAR):type==FUNCTION)));
      //printf("strcmp = %d\n", strcmp((aux->content)->name,n)==0 );
      //printf("types = %d\n",(auxiliarType==PARAMETER)?(type==PARAMETER||type==VAR):type==FUNCTION);
      aux = aux->next;
    }
    if(aux == NULL){
      //printf("Not found\n");
      return NULL;
    } else {
      //printf("Found\n");
      return aux->content;
    }
  }else{
    //printf("%s\n",n );
    //printf("Empty list.\n");
  }
  return  NULL;
}

/*
Inserta un elemento en la tabla de simbolos
*/
void insertList(symbol *head,char n[32], int v, int t,int r){
  if (findInList(head,n,t)){
    fprintf(stderr, "Error: var %s declared before\n", n);
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

void insertFunction(char n[32], int v, int t, int r, symbol *p, node *tree){
  if (findFunction(n)){
    fprintf(stderr, "Error: FUNCTION %s declared before\n", n);
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
node * insertTree (char n[32], int v, int t, int r, int lineNo){
  //printf("Begin insertTree\n");
  item *content;
  if (t == ASSIGN){
    if (!findVar(n,t)){ //chequeo de variable declarada
      fprintf(stderr, "Error: var %s undeclared.  %d \n", n, lineNo);
      exit(EXIT_FAILURE);
    }else{
      content = (item *) malloc(sizeof(item));
      item *contentAux = findVar(n,t);
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
      content = (item *) malloc(sizeof(item));
      item *contentAux = findVar(n,t);
    if (!contentAux){
        fprintf(stderr, "Error: var %s undeclared  %d \n", n, lineNo);
        exit(EXIT_FAILURE);
    }else{
      strcpy(content->name,contentAux->name);
      content->value =contentAux->value;
      content->type = VAR;
      content->ret = contentAux->ret;
    }
  }else{
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

void addParamCall(paramsCall *head,node *p){
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
  if (pl->next!=NULL && pc->next!=NULL){
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

void checks (symbol *head){
  symbol *aux = head;
  int r;
  while((aux->next)!=NULL){
    aux=aux->next;
    if (aux->content->type==FUNCTION){
      checkTree(aux->content->function->tree, aux->content->ret);
    }
  }
}


void checkTree (node *head, int functionRet){
/*constantes para definir tipo de valor en los items de lista
y tablas (si son variables ,constantes o operaciones)*/
  int ret;
  bool hasRet = false;
  if ((head->content)->type == FUNCTION){
    checkTree(head->content->function->tree, functionRet);
  }
  if ((head->content)->type == FUNCTION_CALL_NP){

    checkParams(head);
    checkTree(head->content->function->tree, functionRet);
  }  
  if ((head->content)->type == FUNCTION_CALL_P){

    checkParams(head);
    checkTree(head->content->function->tree, functionRet);
  }  
  if ((head->content)->type == IFAUX){
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion if debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->middle, functionRet);
    checkTree(head->right, functionRet);
  }  
  if ((head->content)->type == IF_ELSE){
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion if debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->right, functionRet);

  }  
  if ((head->content)->type == ASSIGN){
    ret = evalExpr (head->left);
    if ((head->content)->ret != ret){
        fprintf(stderr, "Error: Error de tipos asignacion en asignacion, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE); 
    }
  }  
  if ((head->content)->type == WHILEAUX){
    ret = evalExpr (head->left);
    if (ret!=BOOLAUX){
        fprintf(stderr, "Error: expresion while debe retornar booleano, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
    checkTree(head->right, functionRet);
  }  
  if ((head->content)->type == RETURNAUX){
    if (functionRet!=BOOLAUX){
        fprintf(stderr, "Error: Return no devuelve nada y la funcion devuelve un valor, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
  }  
  if ((head->content)->type == RETURN_EXPR){
    ret = evalExpr (head->left);
    if (functionRet!=ret){
        fprintf(stderr, "Error: Error en de tipo expresion return, Linea %d\n" ,head->lineNo);
        exit(EXIT_FAILURE);
    }
  }  
  if ((head->content)->type == STATEMENTS){
    checkTree(head->left, functionRet);
    checkTree(head->right, functionRet);
  }
  if ((head->content)->type == BLOCK){
    checkTree(head->left, functionRet);
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

