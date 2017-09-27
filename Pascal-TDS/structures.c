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
#define PARAMETERS 13
#define IFAUX 14
#define IF_ELSE 15
#define ASSIGN 16
#define WHILEAUX 17
#define RETURNAUX 18
#define RETURN_EXPR 19
#define STATEMENTS 20
#define BLOCK 21







////////DECLARACION DE TIPOS

typedef struct tree node;
typedef struct itemsFunc itemFunc;
typedef struct items item;
typedef struct symbolTable symbol;
typedef struct paramsLists paramsList;
typedef struct paramsCalls paramsCall;
typedef struct parameters parameter;

typedef struct parameters{
  int type;
  char name[32];
} parameter;

typedef struct paramsCalls{
  node *params[10];
  int paramsNo;
} paramsCall;

typedef struct paramsLists{
  parameter params[10];
  int paramsNo;
} paramsList;

/*
Define un item adicional para las funciones del arbol o tabla de simbolos
ret: tipo de retorno de la funcion
value: lista de parametros de la funcion
type: arbol de sentencias de la funcion
*/
typedef struct itemsFunc{
  paramsList params;
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
item * findTable(char n[32]);
item * searchFunction(char n[32]);
void insertTable(char n[32], int v, int t, int r);

// List
item * findList(symbol *head, char n[32]);
void insertList(symbol *head, char n[32], int v, int t ,int r);
void insertFunction(char n[32], int v, int t, int r, paramsList *p, node *tree);
void addParamList(paramsList *l,int type, char name[32]);
void addParamCall(paramsCall *l, node *p);
void showList(symbol *head);

// Tree
node * insertTree (char n[32], int v, int t, int r, int lineNo);
void concatLeft (node *father, node *son);
void concatRight (node *father, node *son);
void concatMiddle (node *father, node *son);
void showTree(node *aux);

/*int evalTree(node *aux);
int semanticSum(node *sum);
int semanticSub(node *sub);
int semanticProd(node *prod);
int semanticParenthesis(node *parenthesis);*/

//////VARIABLES GLOBALES/////

int MAXSIZE = 20;
symbol *levels[20];
int top = -1;

/*
puntero a la cabeza de un arbol
*/
//node *head;


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
  //printf("Begin openLevel\n");
  if(!isFull()){
    top = top + 1;
    symbol *head = (symbol *) malloc(sizeof(symbol));
    head->next = NULL;
    levels[top]=head;
  } else {
    printf("Could not open a new level.\n");
  }
  //printf("End openLevel\n");
}

void closeLevel(){
  //printf("Begin closeLevel\n");
  if(!isEmpty()){
    top = top - 1;
  } else {
    printf("No levels open.\n");
  }
  //printf("End closeLevel\n");
}

int typeLastVar(){
  printf("Begin typeLastVar\n");
  symbol *aux = levels[top];
  if ((aux->next)!=NULL){
    while (aux->next!=NULL)
      aux = aux->next;
  }
  printf("End typeLastVar\n");
  return (aux->content)->type;
}

item * findTable(char n[32]){
  printf("Begin findTable\n");
  int i = top;
  item *aux = (item *) malloc(sizeof(item));
  while (i>=0){
    aux = findList(levels[i],n);
    if(aux != NULL){
      printf("End findTable\n");
      return aux;
    }
    i = i - 1;
  }
  printf("End findTable\n");
  return NULL;
}

item * searchFunction (char n[32]){
  //printf("Begin searchFunction\n");
  item *aux = (item *) malloc(sizeof(item));
  aux = findList(levels[0],n);
  if(aux != NULL){
    //printf("End searchFunction\n");
    return aux;
  }
  //printf("End searchFunction\n");
  return NULL;
}

void insertTable(char n[32], int v, int t, int r){
  insertList(levels[top],n,v,t,r);
}

/*
Busca un elemento en la tabla de simbolos
*/
item * findList(symbol *head,char n[32]){
  //printf("Begin findList\n");
  symbol *aux = head;
  if((aux->next)!=NULL){
    aux=aux->next;
    while(aux!=NULL && strcmp((aux->content)->name,n)){
      aux = aux->next;
    }
    if(aux == NULL){
      return NULL;
    } else {
      //printf("End findList\n");
      return aux->content;
    }
  }else{
    printf("Empty list.\n");
  }
  //printf("End findList\n");
  return  NULL;
}

/*
Inserta un elemento en la tabla de simbolos
*/
void insertList(symbol *head,char n[32], int v, int t,int r){
  printf("Begin insertList\n");
  if (findList(head,n)){
    fprintf(stderr, "Error: var %s declared before\n", n);
    printf("End insertList\n");
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
  printf("End insertList\n");
}

void insertFunction(char n[32], int v, int t, int r, paramsList *p, node *tree){
  //printf("Begin insertFunction\n");
  if (searchFunction(n)){
    fprintf(stderr, "Error: FUNCTION %s declared before\n", n);
    //printf("End insertFunction\n");
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
    int totalParams = p->paramsNo;
    for(int i=0; i<totalParams; i++){
      addParamList(&(contentFunc->params),((p->params)[i]).type,((p->params)[i]).name);
    }
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
  //printf("End insertFunction\n");
}



/*
Muestra la tabla de simbolos
*/
/*
void showList(symbol *head){
  //printf("Begin showList\n");
  int i = top;
  printf("Showing table\n");
  while (i>=0){
    printf("Level %d\n",i);
    symbol *aux = levels[i];
    if((aux->next)!= NULL){
      do{
        aux = aux->next;
        printf("  Name: %s, value %d\n",(aux->content)->name,(aux->content)->value);
      } while (aux->next!=NULL);
    }else{
      printf("  Empty list.\n");
    }
    i = i - 1;
  }
  //printf("End showList\n");
}*/


/*
Crea un elemento de tipo arbol con sus hijos NULL,
si el elemento es de tipo Var busca sus datos en la tabla de Simbolos
*/
node * insertTree (char n[32], int v, int t, int r, int lineNo){
  //printf("Begin insertTree\n");
  item *content;
  if (t == ASSIGN){
    if (!findTable(n)){ //chequeo de variable declarada
      fprintf(stderr, "Error: var %s undeclared. \n", n);
      printf("End insertTree\n");
      exit(EXIT_FAILURE);
    }else{
      content = (item *) malloc(sizeof(item));
      item *contentAux = findTable(n);
      strcpy(content->name,contentAux->name);
      content->value = v;
      content->type = ASSIGN;
      if (contentAux->ret==r){
        content->ret = contentAux->ret;
      }else{
        fprintf(stderr, "Error: Error en tipos de assignacion \n");
        exit(EXIT_FAILURE);
      }
    }
  }
  if (t == VAR){
      content = (item *) malloc(sizeof(item));
      item *contentAux = findTable(n);
    if (!contentAux){
              fprintf(stderr, "Error: var %s undeclared \n", n);
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
  return element;
}

/*
agrega el arbol son como hijo izquierdo del arbol father
*/
void concatLeft (node *father, node *son){
  father->left=son;
  //printf("concatLeft\n");
}

/*
agrega el arbol son como hijo derecho del arbol father
*/
void concatRight (node *father, node *son){
  father->right=son;
  //printf("concatRight\n");
}

/*
agrega el arbol son como hijo del medio del arbol father
*/
void concatMiddle (node *father, node *son){
  father->middle=son;
  //printf("concatMiddle\n");
}

void addParamList(paramsList *l,int type, char name[32]){
  ((l->params)[l->paramsNo]).type=type;
  strcpy(((l->params)[l->paramsNo]).name,name);
  (l->paramsNo) = (l->paramsNo)+1;
}

void addParamCall(paramsCall *l,node *p){
  (l->params)[l->paramsNo]=p;
  (l->paramsNo) = (l->paramsNo)+1;
}

node * insertParamAux (char name[32],int ret,node *expr) {
    insertTable(name, 0, VAR, ret);
    node *assign = insertTree(name,0,ASSIGN, (expr->content)->ret);
    concatLeft (assign,expr);
    return assign;
}

/*
node * insertParams (paramsList l,paramsCall c){
  int size = c.paramsNo;
  node *expr;
  node *aux1 = NULL;
  node *aux2 = NULL;
  int i =0;
  if (i<size){
    aux1 = insertParamAux(l.params[i].name,l.params[i].type,c.params[i]);
    i++;
    while (i<size){
      aux2 = insertTree("PARAMETERS",0,PARAMETERS,INDETERMINATE);
      concatLeft (aux2,aux1);
      aux1 = insertParamAux(l.params[i].name,l.params[i].type,c.params[i]);
      concatRight (aux2, aux1);
      aux1=aux2;
      i++;
    }
  }
  return aux1;
}*/

bool checkParams (paramsList l,paramsCall c){
  int size = c.paramsNo;
  int i = 0;
  bool error = false;
  int t1;
  if(size!=(l.paramsNo)){
    return false;
  }else {
    while (i<size && !error){
      t1 = ((c.params[i])->content)->ret;
      if ((l.params[i]).type != t1){
        error = true;
      }
      i++;
      }
    if (!error){
      return true;
    }
    else{
      return false;
    }
  }
}



/*
Muestra la estructura del arbol de modo InOrder
*/
/*void showTree(node *aux){
  //printf("Begin showTree\n");
  if(aux!=NULL){
    if((aux->content)->type==CONSTANT){
      printf(" %d ",(aux->content)->value);
    }
    if((aux->content)->type == VAR){
    printf(" %s ",(aux->content)->name);
    }
    if((aux->content)->type == OPER_AR){
      if(!strcmp((aux->content)->name,"()")){
        printf (" (");
        showTree (aux->left);
        printf (" )");
      }
      else{
        showTree (aux->left);
        if(!strcmp((aux->content)->name,"+")){
          printf (" + ");
        }
        if(!strcmp((aux->content)->name,"-")){
          printf (" - ");
        }
        if(!strcmp((aux->content)->name,"*")){
          printf (" * ");
        }
        if(!strcmp((aux->content)->name,"/")){   //DIVISION VER SIMBOLO
          printf (" / ");
        }
        if(!strcmp((aux->content)->name,"MOD")){  //MODULO VER SIMBOLO
          printf (" MOD ");
        }
        if(!strcmp((aux->content)->name,"<")){
          printf (" < ");
        }
        else{
          printf(" > ");
        }
        showTree (aux->right);
      }
    }
    if((aux->content)->type == OPER_LOG){
      if(!strcmp((aux->content)->name,"-")){ //OPERADOR SUB VER SIMBOLO
        printf (" - ");
        showTree (aux->right);
      }
      if(!strcmp((aux->content)->name,"!")){ //OPERADOR NOT
        printf (" ! ");
        showTree (aux->right);
      }
      else{
        showTree (aux->left);
        if(!strcmp((aux->content)->name,"=")){
          printf (" = ");
        }
        if(!strcmp((aux->content)->name,"&&")){
          printf (" && ");
        }
        else{
          printf(" || ");
        }
        showTree (aux->right);
      }
    }
  }
  else{
    printf("Empty Tree.\n");
  }
  //printf("End showTree\n");
} */



/*
Evalua el arbol
*/

/*
int evalTree(node *aux){
  if((aux->content)->type == VAR || (aux->content)->type==CONSTANT){
    return (aux->content)->value;
  }
  else{
    if((aux->content)->type == OPER_AR){
      if(!strcmp((aux->content)->name,"()")){
        return semanticParenthesis(aux);
      }
      if(!strcmp((aux->content)->name,"+")){
        return semanticSum(aux);
      }
      if(!strcmp((aux->content)->name,"-")){
        return semanticSub(aux);
      }
      if(!strcmp((aux->content)->name,"*")){
        return semanticProd(aux);
      }
      if(!strcmp((aux->content)->name,"/")){   //DIVISION VER SIMBOLO
        printf (" / ");
      }
      if(!strcmp((aux->content)->name,"MOD")){  //MODULO VER SIMBOLO
        printf (" MOD ");
      }
      if(!strcmp((aux->content)->name,"<")){
        printf (" < ");
      }
      else{
        printf(" > ");
      }
    }
    if((aux->content)->type == OPER_LOG){
      if(!strcmp((aux->content)->name,"-")){ //OPERADOR SUB VER SIMBOLO
        printf (" - ");
      }
      if(!strcmp((aux->content)->name,"!")){ //OPERADOR NOT
        printf (" ! ");
      }
      if(!strcmp((aux->content)->name,"=")){
        printf (" = ");
      }
      if(!strcmp((aux->content)->name,"&&")){
        printf (" && ");
      }
      else{
        printf(" || ");
      }
    }
  return 0;///para que no tire error
  }
}
*/


/*
Semantica de la operacion Suma +
*/

/*
int semanticSum(node *sum){
    return (evalTree(sum->left) + evalTree(sum->right));
}
*/

/*
Semantica de la operacion Sustraccion -
*/

/*
int semanticSub(node *sub){
    return (evalTree(sub->left) - evalTree(sub->right));
}
*/

/*
Semantica de la operacion Producto *
*/
/*
int semanticProd(node *prod){
    return (evalTree(prod->left) * evalTree(prod->right));
}
*/

/*
Semantica de la operacion Parentesis ()
*/
/*
int semanticParenthesis(node *parenthesis){
    return evalTree(parenthesis->left);
}
*/
