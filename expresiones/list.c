#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define VAR 0
#define OPER 1
#define CONSTANT 2


typedef struct items{
    char name[32];
    int value;
    int type;
} item;

typedef struct symbolTable{
    item *content;
    struct symbolTable *next;
} symbol;

typedef struct tree{
    item *content;
    struct tree *left;
    struct tree *right;
} node;

symbol *first, *last;
node *head;

void insert(char n[32], int v, int t){
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

void show(){
    symbol *aux = first;
    if(aux!=NULL){
        printf("Showing list\n");
        while(aux !=  NULL){
            printf("Name: %s, value %d, type: %d\n",(aux->content)->name,(aux->content)->value,(aux->content)->type);
            aux = aux->next;
        }
    }else{
        printf("Empty list.\n");
    }
}

item * find(char n[32], int t){
    symbol *aux = first;
    if(aux!=NULL){
        while(aux !=  NULL && strcmp((aux->content)->name,n) && (aux->content)->type!=t){
            aux = aux->next;
        }
        if(aux == NULL){
            printf("Element not found \n");
            return NULL;
        } else {
            printf("Element found \n");
            return aux->content;
        }
    }else{
        printf("Empty list.\n");
    }
    return  NULL;
}

node * insertTree (char n[32], int v, int t, node *father){
    item *content;
    if (t == VAR){
    	content = find(n, t);
    }else{
		content = (item *) malloc(sizeof(item));
	    strcpy(content->name,n);
	    content->value = v;
	    content->type = t;
	}
    node *element;
    element = (node *) malloc(sizeof(node));
  	element->content = content;
  	element->left = NULL;
  	element->right = NULL;
    if(father==NULL){
    	head = element;
    }else{
    	if (father->left == NULL){
    		father->left = element;
    	}
    	else{
    		father->right = element;
    	}
    }
    return element;
}

void showThree(node *aux){
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
				showThree (aux->left);
				printf (" )");
			}else{
				showThree (aux->left);
				printf (" + ");
				showThree (aux->right);
			}
		}
    }else{
        printf("Empty list.\n");
    }
}

int eval(node *aux){
	if((aux->content)->type == VAR || (aux->content)->type==CONSTANT){
		return (aux->content)->value;
	}
	else{
		if(!strcmp((aux->content)->name,"()")){
			return eval(aux->left);
		}else{
			if(!strcmp((aux->content)->name,"+")){
				return (eval(aux->left) + eval(aux->right));
			}else{
				return (eval(aux->left) * eval(aux->right));
			}
		}
	}
}

int main(){
    show();
    insert("X",1,2);
    show();
    insert("Y",3,VAR);
    show();
    item *aux = find("X",VAR);
    printf("%d\n", aux->value);
    node *element;
    node *elementAux; 
    element = insertTree("+", 0, OPER, head);
    elementAux = insertTree("Y", 0, VAR, element);
    elementAux = insertTree("", 2, CONSTANT, element);
	printf("ecuacion: ");
	showThree(head);
    printf("\nresultado: ");
    printf("%d\n", eval(head));
    return 0;
}