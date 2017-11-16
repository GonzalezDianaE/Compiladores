#include "structures.c"
#include <stdlib.h>

void goOver(symbol *head);
void optimizate(node *head);
void optimizateExpr (node *head);
int getEvaluationResult(char *operation,int left, int right);
bool couldOptimizate(char *operation);
void deallocate(node *head);
bool lala;
bool returnFound;

void goOver(symbol *head){
  symbol *aux = head;
  int r;
  while((aux->next)!=NULL){
    aux=aux->next;
    if (aux->content->type==FUNCTION){
      optimizate(aux->content->function->tree);
    }
  }
}

void optimizate(node *head){
  if ((head->content)->type == FUNCTION){
    optimizate(head->content->function->tree);
  }

  if ((head->content)->type == FUNCTION_CALL_NP){
    optimizate(head->content->function->tree);
  }  

  if ((head->content)->type == FUNCTION_CALL_P){
    paramsCall *pc= head->content->paramsCall;
  	if(pc != NULL){
  		while (pc->next!=NULL){
			pc = pc->next;
			if(couldOptimizate(pc->param->content->name)){
				optimizateExpr(pc->param);
			}
		}
  	}
    optimizate(head->content->function->tree);
  }  

  if ((head->content)->type == IFAUX){
  	if(couldOptimizate(head->left->content->name)){
  		optimizateExpr(head->left);
  		if(head->left->content->value==0){
        deallocate(head);
        //free(head);
        head = NULL;
        lala=true;
  		} else {
    		optimizate(head->right);
    	}
  	} else {
  		optimizate(head->right);
  	}
    return ;
  }  

  if ((head->content)->type == IF_ELSE){
  	if(couldOptimizate(head->left->content->name)){
  		optimizateExpr(head->left);
      printf("%d\n",head->left->content->value );
  		if(head->left->content->value==0){
  			optimizate(head->right);
  			node *oldHead = head;
  			head = head->right;
  			deallocate(oldHead);
  		} else {
  			//optimizate(head->middle);
  			//node *oldHead = head;
        deallocate(head);
        printf("LALALALLALALALALALLALA \n");
  			//head = head->middle;
    		//deallocate(oldHead);
        printf("LALALALLALALALALALLALA \n");
        //printf("%s \n", oldHead->content->name);
    	}
  	} else {
  		optimizate(head->middle);
    	optimizate(head->right);
  	}
    return ;
  }

  if ((head->content)->type == ASSIGN){
  	if (couldOptimizate(head->left->content->name)){
  		optimizateExpr(head->left);
  	}
  } 

  if ((head->content)->type == WHILEAUX){
  	if(couldOptimizate(head->left->content->name)){
  		optimizateExpr(head->left);
  		if(head->left==0){
  			deallocate(head);
  		} else {
  			//Ciclo infinito
  		}
  	} else {
  		optimizate(head->right);
  	}
  } 

  if ((head->content)->type == RETURNAUX){
  	returnFound = true;
  	/*if (previous->content->type == STATEMENTS){
  		deallocate(previous->right);
  		previous = head;
  		deallocate(head);
  	}*/
  }

  if ((head->content)->type == RETURN_EXPR){
  	returnFound = true;
  	if(couldOptimizate(head->left->content->name)){
  		optimizateExpr(head->left);
  	} else {
  		//optimizate(head->left);
  	}
  }

  /*if ((head->content)->type == PRINTAUX){
    ret = optimizateExpr (head->left);
  } */

  if ((head->content)->type == STATEMENTS){
  	returnFound = false;
  	if(couldOptimizate(head->left->content->name)){
  		 optimizateExpr(head->left);
  	} else {

  		optimizate(head->left);
  	}
    if(!returnFound){
    	if(couldOptimizate(head->right->content->name)){
    		optimizateExpr(head->right);
    	} else {
        lala=false;
    		optimizate(head->right);
        if (lala==true){
          head  = head->left;
        }
    	}
    } else {
    	node *oldHead = head;
    	head = head->left;
    	deallocate(oldHead);
    }
    printf("%s\n", head->left->content->name );
    printf("%s\n", head->right->content->name );

  }

  if ((head->content)->type == BLOCK){
    optimizate(head->left);
    if lala = true;
    
  }
} 
 /*
void optimizateExpr (node *head){
	int t1 = head->content->type;
	if (t1==OPER_AR || t1==OPER_LOG || t1==OPER_EQUAL || t1==OPER_REL){
		if (head->left->content->type == CONSTANT && head->right->content->type == CONSTANT){
			int rdo = getEvaluationResult(head->content->name,head->left->content->value,head->right->content->value);
			free(head->left);
			free(head->right);
      head->content->ret = INTEGERAUX;
			head->content->type = CONSTANT;
			head->content->value = rdo;
		}
	}  
	if (t1==OPER_AR_UN || t1==OPER_LOG_UN){
		if (head->left->content->type == CONSTANT && head->right->content->type == CONSTANT){
			int rdo = getEvaluationResult(head->content->name,head->left->content->value,0);
			free(head->left);
			head->content->type = CONSTANT;
			head->content->value = rdo;
		}
	}
}*/

void optimizateExpr (node *head){
  int t1 = head->content->type;
  if (t1==OPER_AR || t1==OPER_LOG || t1==OPER_EQUAL || t1==OPER_REL){
    optimizateExpr(head->left);
    optimizateExpr(head->right);
    if (head->left->content->type == CONSTANT && head->right->content->type == CONSTANT){
      int rdo = getEvaluationResult(head->content->name,head->left->content->value,head->right->content->value);
      free(head->left);
      free(head->right);
      head->left = NULL;
      head->right = NULL;
      head->content->type = CONSTANT;
      head->content->value = rdo;
    } 
  }  
  if (t1==OPER_AR_UN || t1==OPER_LOG_UN){
    optimizateExpr(head->left);
    if (head->left->content->type == CONSTANT){
      int rdo = getEvaluationResult(head->content->name,head->left->content->value,0);
      free(head->left);
      head->left = NULL;
      head->content->type = CONSTANT;
      head->content->value = rdo;
    } else {
      if(couldOptimizate(head->left->content->name)){
        optimizateExpr(head->left);
      }
    }
  }
}


int getEvaluationResult(char *operation,int left, int right){	
	if (strcmp(operation,"OP_ADD")==0){
		return left + right;
	}
	if (strcmp(operation,"OP_SUB")==0){
		return left - right;
	}
	if (strcmp(operation,"OP_PROD")==0){
		return left * right;
	}
	if (strcmp(operation,"OP_DIV")==0){
		return left / right;
	}
	if (strcmp(operation,"OP_MOD")==0){
		return left % right;
	}
	if (strcmp(operation,"OP_MINOR")==0){
		return left < right;
	}
	if (strcmp(operation,"OP_MAJOR")==0){
		return left > right;
	}
	if (strcmp(operation,"OP_EQUAL")==0){
		return left == right;
	}
	if (strcmp(operation,"OP_AND")==0){
		return left && right;
	}
	if (strcmp(operation,"OP_OR")==0){
		return left || right;
	}
	if (strcmp(operation,"OP_NOT")==0){
		return !left;
	}
	if (strcmp(operation,"NEG")==0){
		return -left;
	}
	return 0;
    /*switch (operation){
    	case OP_ADD:
    		return left + right;

    	case OP_SUB :
    		return left - right;

    	case OP_PROD :
    		return left * right;

    	case OP_DIV :
    		return left / right;

    	case OP_MOD :
    		return left % right;

    	case OP_MINOR :
    		return left < right;

    	case OP_MAJOR :           
    		return left > right;

    	case OP_EQUAL :
    		return left == right;

    	case OP_AND :
    		return left && right;

    	case OP_OR :
    		return left || right;

    	case OP_NOT :
    		return !left;

    	case NEG :
    		return -left;
    }*/

}

bool couldOptimizate(char *operation){
	if(strcmp(operation,"OP_ADD")==0 || strcmp(operation,"OP_SUB")==0 || strcmp(operation,"OP_PROD")==0 || strcmp(operation,"OP_DIV")==0 || 
		strcmp(operation,"OP_MOD")==0 || strcmp(operation,"OP_MINOR")==0 ||strcmp(operation,"OP_MAJOR")==0 ||strcmp(operation,"OP_EQUAL")==0 ||
		strcmp(operation,"OP_AND")==0 || strcmp(operation,"OP_OR")==0 || strcmp(operation,"OP_NOT")==0 || strcmp(operation,"NEG")==0){
		return true;
	} else {
		return false;
	}
}

void deallocate(node *head){
	if (head != NULL) {
		deallocate(head->left);
		deallocate(head->right);
    deallocate(head->middle);
		free(head);
    head = NULL;
	}
}