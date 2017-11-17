#include "structures.c"
#include <stdlib.h>

void goOver(symbol *head);
void optimizate(node *head);
void optimizateExpr (node *head);
int getEvaluationResult(char *operation,int left, int right);
bool couldOptimizate(char *operation);
void deallocate(node *head);
bool returnFound;
bool deadCode;

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
  if (head!=NULL){
    printf("%s\n", head->content->name );
    switch (head->content->type){

      case FUNCTION_CALL_P :
        {paramsCall *pc = head->content->paramsCall;
        if(pc != NULL){
          while (pc->next!=NULL){
            pc = pc->next;
            printf("entro\n");
            optimizateExpr(pc->param);
          }
        }
      }
      break;

      case IFAUX :
        optimizateExpr(head->left);
        //si dsp de optimizar queda una constante y es falsa es todo codigo muerto
        if (head->left->content->type == CONSTANT && (head->left->content->value==0)){
          deadCode = true;
        }else{
          optimizate(head->right);
          //si la condicion es constante y es siempre true siempre va a realizar el codigo
          //entonces cambio el tipo por if por statement y dejo elimino la condicion
          if (head->left->content->type == CONSTANT && (head->left->content->value==1)){
            head->content->type = STATEMENTS;
            deallocate (head->left);
            head->left = NULL;
          }
        }
      break;

      case IF_ELSE :
        optimizateExpr(head->left);
        //si dsp de optimizar queda una constante y es falsa siempre se ejecuta el else
        //entonces cambio el tipo por statement y optimizo el hijo derecho (then) saco los hijos izquierdos (condicion)
        // y del medio (else)
        if (head->left->content->type == CONSTANT && (head->left->content->value==0)){
          optimizate(head->right);
          head->content->type = STATEMENTS;
          deallocate (head->left);
          head->left = NULL;
          deallocate (head->middle);
          head->middle = NULL;
        }else{
          //si dsp de optimizar queda una constante y es falsa siempre se ejecuta el else
        //entonces cambio el tipo por statement y saco la condicion y el hijo del medio
          if (head->left->content->type == CONSTANT && (head->left->content->value==1)){
            optimizate(head->middle);
            head->content->type = STATEMENTS;
            deallocate (head->left);
            head->left = head->middle;
            deallocate (head->right);
            head->right = NULL;
            head->middle = NULL;
          }
          else{
            optimizate(head->right);
            optimizate(head->middle);
          }
        }
      break;

      case ASSIGN :
      //optimizo la expresion
        optimizateExpr(head->left);
      break;

      case WHILEAUX :
        // parecido a if
        optimizateExpr(head->left);
        if (head->left->content->type == CONSTANT && (head->left->content->value==0)){
          deadCode = true;
        }else{
          optimizate(head->right);
        } 
        break;

      case RETURNAUX :
        returnFound = true;
      break;

      case RETURN_EXPR :
        returnFound = true;
        optimizateExpr(head->left);  
      break;

      case STATEMENTS :
        returnFound = false;
        deadCode = false;
        optimizate(head->left);
        if (returnFound){
          deallocate (head->right);
          head->right = NULL;
        }
        //si codigo muerto es verdadero se elimina el hijo de statement
        if (deadCode){
          deallocate (head->left);
          head->left = NULL;
        }
        deadCode = false;
        //si es codigo muerto es verdadero se elimina el hijo de statement
        optimizate(head->right);
        if (deadCode){
          deallocate (head->right);
          head->right = NULL;
        }
      break;

      case BLOCK :
        optimizate(head->left);
      break;

      case PRINTAUX :
        optimizateExpr(head->left);      
      break;

      default:
      break;
    }
  }
}

//FALTARIA SETEAR EL TIPO DE RETORNO??
void optimizateExpr (node *head){
  int t1 = head->content->type;
  if (t1==FUNCTION_CALL_P){
    optimizate(head);
  }
  if (t1==OPER_AR || t1==OPER_LOG || t1==OPER_EQUAL || t1==OPER_REL){
    optimizateExpr(head->left);
    optimizateExpr(head->right);
    if (head->left->content->type == CONSTANT && head->right->content->type == CONSTANT){
      int rdo = getEvaluationResult(head->content->name,head->left->content->value,head->right->content->value);
      head->content->ret = head->left->content->ret;// no se si esta bien
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
      head->content->ret = head->left->content->ret; //no se si esta bien
      free(head->left);
      head->left = NULL;
      head->content->type = CONSTANT;
      head->content->value = rdo;
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