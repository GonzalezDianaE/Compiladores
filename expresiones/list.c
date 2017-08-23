#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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


int main(){
    show();
    insert("X",1,2);
    show();
    insert("Y",3,4);
    show();
    item *aux = find("Y",4);
    printf("%d\n", aux->value);
    return 0;
}