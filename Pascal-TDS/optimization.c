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

/* Recorre la tabla de simbolos, buscando funciones para oprtimizar su código. */
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

/* Recorre el ast con dfs. Segin el tipo de las operaciones que va encontrando realiza distintas optimizaciones. */
void optimizate(node *head){
  if (head!=NULL){
    printf("%s\n", head->content->name );
    switch (head->content->type){

      /* Llamado a función: Busca optimizar cada uno de sus parámetros. */
      case FUNCTION_CALL_P :
        paramsCall *pc = head->content->paramsCall;
        if(pc != NULL){
          while (pc->next!=NULL){
            pc = pc->next;
            optimizateExpr(pc->param);
          }
        }
      break;

      /* If-Then: Intenta optimizar la condición. Luego, si la misma es falsa constante elimina completamente 
      el código del if; caso contrario, verdadera constante, sube el codigo correspondiente al then a la cabeza 
      actual y elimina la instrucción if. */
      case IFAUX :
        optimizateExpr(head->left);
        if (head->left->content->type == CONSTANT && (head->left->content->value==0)){
          deadCode = true;
        }else{
          optimizate(head->right);
          if (head->left->content->type == CONSTANT && (head->left->content->value==1)){
            head->content->type = STATEMENTS;
            deallocate (head->left);
            head->left = NULL;
          }
        }
      break;

      /* If-Then-Else: Intenta optimizar la condición. Luego, si la misma es falsa constante sube el codigo correspondiente al 
      else a la cabeza actual y elimina la instrucción if; caso contrario, verdadera constante, sube el codigo correspondiente al 
      then a la cabeza actual y elimina la instrucción if. Si la condicion no es constante, intenta optimizar los bloques then y else. */
      case IF_ELSE :
        optimizateExpr(head->left);
        if (head->left->content->type == CONSTANT && (head->left->content->value==0)){
          optimizate(head->right);
          head->content->type = STATEMENTS;
          head->left = NULL;
          deallocate (head->middle);
          head->middle = NULL;
        }else{
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

      /* Asignación: Intenta optimizar la expresión a asignar. */
      case ASSIGN :
        optimizateExpr(head->left);
      break;

      /* While: Intenta optimizar la condición. Luego, si la misma es falsa constante elimina completamente
      el código del ciclo. Caso contrario (sea o no constante la condición), intenta oprtimizar el bloque del ciclo.*/
      case WHILEAUX :
        optimizateExpr(head->left);
        if (head->left->content->type == CONSTANT && (head->left->content->value==0)){
          deadCode = true;
        }else{
          optimizate(head->right);
        } 
        break;

      /* Return Void: Enciende la bandera de return encontrado. */
      case RETURNAUX :
        returnFound = true;
      break;

      /* Return Expression: Enciende la bandera de return encontrado, e intenta optimizar su expresión. */
      case RETURN_EXPR :
        returnFound = true;
        optimizateExpr(head->left);  
      break;

      /* Sentencias: Intenta optimizar su hijo izquierdo, si encuentra un return poda su hijo derecho. Si su hijo izquierdo 
      queda con cédigo muerto, lo poda. Intenta optimizar su hijo derecho y si queda con código muerto también lo poda. */
      case STATEMENTS :
        returnFound = false;
        deadCode = false;
        optimizate(head->left);
        if (returnFound){
          deallocate (head->right);
          head->right = NULL;
        }
        if (deadCode){
          deallocate (head->left);
          head->left = NULL;
        }
        deadCode = false;
        optimizate(head->right);
        if (deadCode){
          deallocate (head->right);
          head->right = NULL;
        }
      break;

      /* Bloque: Intenta optimizar su contenido. */
      case BLOCK :
        optimizate(head->left);
      break;

      /* Print: Intenta optimizar lo que se quiere mostrar. */
      case PRINTAUX :
        optimizateExpr(head->left);      
      break;

      default:
      break;
    }
  }
}

/* Oprimiza una expresión. Para ello controla que la operacion sea de tipos aritmeticos, logicos o relacionales. 
Obtenido el resultado en el caso de una optimización, sube el resultado a la cabeza actual y libera sus hijos. */
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

/* Evalua una expresion de tipo aritmetica, logica o relacional y devuelve su resultado. */
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

/* Libera de forma recursiva la memoria ocupada por el arbol cuya cabeza es pasada como parámetro. */
void deallocate(node *head){
	if (head != NULL) {
		deallocate(head->left);
		deallocate(head->right);
    deallocate(head->middle);
		free(head);
    head = NULL;
	}
}