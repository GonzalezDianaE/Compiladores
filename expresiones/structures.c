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
#define OPER 1
#define CONSTANT 2

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
/*
punteros al primer y ultimo simbolo de la tabla de simbolos
*/
symbol *first,*last;

/*
puntero a la cabeza de un arbol
*/
node *head;

////////DECLARACION DE FUNCIONES

/*
Inserta un elemento en la tabla de simbolos
*/
void insertList(char n[32], int v, int t);

/*
Muestra la tabla de simbolos
*/
void showList();

/*
Busca un elemento en la tabla de symbolos
*/
item * findList(char n[32]);

/*
Crea un elemento de tipo arbol con sus hijos NULL, 
si el elemento es de tipo Var busca sus datos en la tabla de Simbolos
*/
node * insertTree (char n[32], int v, int t);

/*
agrega el arbol son como hijo izquierdo del arbol father
*/
void concatLeft (node *father, node *son);

/*
agrega el arbol son como hijo derecho del arbol father
*/
void concatRight (node *father, node *son);

/*
Muestra la estructura del arbol de modo InOrder
*/
void showTree(node *aux);

/*
Evalua el arbol
*/
int evalTree(node *aux);

/*
Semantica de la operacion Suma +
*/
int semanticSum(node *sum);

/*
Semantica de la operacion Producto *
*/
int semanticProd(node *prod);
/*
Semantica de la operacion Parentesis ()
*/
int semanticParenthesis(node *parenthesis);


///////IMPLEMENTACION DE FUNCIONES/////////

/*
Inserta un elemento en la tabla de simbolos
*/
void insertList(char n[32], int v, int t){
    if (findList(n)){
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
        if(first==NULL){
            first = element;
            last = element;
        } else {
            last->next = element;
            last = element;
        }
    }   
}

/*
Muestra la tabla de simbolos
*/
void showList(){
    symbol *aux = first;
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
item * findList(char n[32]){
    symbol *aux = first;
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
        content = findList(n);
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
		if((aux->content)->type == OPER){
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

