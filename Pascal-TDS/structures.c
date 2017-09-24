/*
Libreria que contiene los tipos y funciones necesarias para generar la tabla
de simbolos y el arbol sintactico de la gramatica para el PreProyecto. Ademas contiene
la semantica de las funciones suma producto y parentesis
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*constantes para definir tipo de valor en los items de lista
y tablas (si son variables ,constantes o operaciones)*/
#define VAR 0
#define CONSTANT 1
#define OPER_AR 2
#define OPER_LOG 3
#define INTEGERAUX 4
#define BOOLAUX 5
#define VOIDAUX 6
#define ERROR 7
#define INDETERMINATE 8
#define FUNCTION 9


////////DECLARACION DE TIPOS

typedef struct tree node;
typedef struct itemsFunc itemFunc;
typedef struct items item;
typedef struct symbolTable symbol;

/*
Define un item adeicionales para las funciones del arbol o tabla de simbolos
ret: define el tipo de retorno de la funcion
value: lista de parametros de la funcion
type: arbol de sentencias de la funcion
*/
typedef struct itemsFunc{
    int ret;
    int params[10];
    node *tree;
} itemFunc;

/*
Define un item del arbol o tabla de simbolos
name: Si es una variable define su nombe,
    en caso de ser una operacion define el tipo de operacion (*,+,())
value: define el valor de la constante o variable
type: define el tipo de item (operacion, variable o constante)
*/
typedef struct items{
    char name[32];
    int value;
    int type;
    itemFunc *function;
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
} node;

int isEmpty();
int isFull();
void openLevel();
void closeLevel();
item * searchVar(char n[32]);
int typeLastVar();
void insertVar(char n[32], int v, int t);
void insertList(symbol *head, char n[32], int v, int t);
void showList(symbol *head);
item * findList(symbol *head, char n[32]);
node * insertTree (char n[32], int v, int t);
void concatLeft (node *father, node *son);
void concatRight (node *father, node *son);
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
  if(!isFull()){
    top = top + 1;
    symbol *head;
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

/*
Busca un elemento en la tabla de symbolos
*/
item * findList(symbol *head,char n[32]){
    symbol *aux = head;
    if(aux!=NULL){
        while(aux !=  NULL && strcmp((aux->content)->name,n)){
            aux = aux->next;
        }
        if(aux == NULL){
            return NULL;
        } else {
            return aux->content;
        }
    }else{
        printf("Empty list.\n");
    }
    return  NULL;
}

item * searchVar(char n[32]){
  int i = top;
  item *aux;
  while (i>=0){
      aux = findList(levels[i],n);
      if(aux != NULL){
        return aux;
      }
      i = i - 1;
  }
  return NULL;
}

int typeLastVar(){
  symbol *aux= levels[top];
  if (aux!=NULL){
    while (aux->next!=NULL)
      aux = aux->next;
  }
  return (aux->content)->type;
}

item * searchFunction (char n[32]){
  item *aux;
  aux = findList(levels[0],n);
  if(aux != NULL){
        return aux;
  }
  return NULL;
}

void insertVar(char n[32], int v, int t){
  insertList(levels[top],n,v,t);
}

/*
Inserta un elemento en la tabla de simbolos
*/
void insertList(symbol *head,char n[32], int v, int t){
    if (findList(head,n)){
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
        element->content = content;
        element->next = NULL;
        if(head->next==NULL){
            head->next = element;
        }else {
            symbol *aux = head->next;
            while(aux->next != NULL){
              aux = aux->next;
            }
            aux->next = element;
        }
    }
}

void insertFunction(symbol *head,char n[32], int v, int t, int r, int p[10]){
    if (searchFunction(n)){
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
        int loop;
        for (loop=1; loop<sizeof(int[10]); loop++){
          contentFunc->params[loop]=p[loop];
        }
        contentFunc->ret=r;
        content->function= contentFunc;
        element->content = content;
        element->next = NULL;
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
Muestra la tabla de simbolos
*/
void showList(symbol *head){
    symbol *aux = head;
    if(aux!=NULL){
        printf("Showing list\n");
        while(aux !=  NULL){
            printf("Name: %s, value %d\n",(aux->content)->name,(aux->content)->value);
            aux = aux->next;
        }
    }else{
        printf("Empty list.\n");
    }
}


/*
Crea un elemento de tipo arbol con sus hijos NULL,
si el elemento es de tipo Var busca sus datos en la tabla de Simbolos
*/
node * insertTree (char n[32], int v, int t){
    item *content;
    if (t == VAR){
        content = searchVar(n);
    }else{
        content = (item *) malloc(sizeof(item));
        strcpy(content->name,n);
        content->value = v;
        content->type = t;
    }
    node *element;
    element = (node *) malloc(sizeof(node));
    element->content = content;
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

/*
Muestra la estructura del arbol de modo InOrder
*/
void showTree(node *aux){
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
}



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