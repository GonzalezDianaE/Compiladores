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
#define INTEGER 4
#define BOOL 5
#define VOID 6
#define ERROR 7
#define INDETERMINATE 8


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
    struct tree *left;
    struct tree *right;
} node;

//////VARIABLES GLOBALES/////

int MAXSIZE = 20;
symbol *levels[20];
int top = -1;

/*
puntero a la cabeza de un arbol
*/
node *head;

////////DECLARACION DE FUNCIONES
int isEmpty();
int isFull();
void openLevel();
void closeLevel();
item * searchVar(char n[32]);
void insertVar(char n[32], int v, int t);
void insertList(symbol *head, char n[32], int v, int t);
void showList(symbol *head);
item * findList(symbol *head, char n[32]);
node * insertTree (char n[32], int v, int t);
void concatLeft (node *father, node *son);
void concatRight (node *father, node *son);
void showTree(node *aux);
int evalTree(node *aux);
int semanticSum(node *sum);
int semanticProd(node *prod);
int semanticParenthesis(node *parenthesis);


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

item * searchVar(char n[32]){
  int i = top;
  item *aux;
  while (i>=0){
      aux = findList(levels[top],n);
      if(aux != NULL){
        return aux;
      }
      i = i - 1;
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
		if((aux->content)->type == OPER_AR || (aux->content)->type == OPER_LOG){
			if(!strcmp((aux->content)->name,"()")){
				printf (" (");
				showTree (aux->left);
				printf (" )");
			}else{
                showTree (aux->left);
                if(!strcmp((aux->content)->name,"+")){
                    printf (" + ");
                } else {
                    printf (" * ");
                }
				showTree (aux->right);
			}
		}
    }else{
        printf("Empty Tree.\n");
    }
}



/*
Evalua el arbol
*/
int evalTree(node *aux){
	if((aux->content)->type == VAR || (aux->content)->type==CONSTANT){
		return (aux->content)->value;
	}
	else{
		if(!strcmp((aux->content)->name,"()")){
			return semanticParenthesis(aux);
		}else{
			if(!strcmp((aux->content)->name,"+")){
				return semanticSum(aux);
			}else{
				return (semanticProd(aux));
			}
		}
	}
}

/*
Semantica de la operacion Suma +
*/
int semanticSum(node *sum){
    return (evalTree(sum->left) + evalTree(sum->right));
}

/*
Semantica de la operacion Producto *
*/
int semanticProd(node *prod){
    return (evalTree(prod->left) * evalTree(prod->right));
}

/*
Semantica de la operacion Parentesis ()
*/
int semanticParenthesis(node *parenthesis){
    return evalTree(parenthesis->left);
}
