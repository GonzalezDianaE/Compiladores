#include "structures.c"
#include <stdlib.h>

/* Constantes para definir tipo de las instrucciones de codgo intermedio. */
#define IC_ADD 0
#define IC_SUB 1
#define IC_PLUS 2
#define IC_DIV 3
#define IC_MOD 4
#define IC_AND 5
#define IC_OR 6
#define IC_NOT 7
#define IC_EQUALAR 8
#define IC_EQUALLOG 9
#define IC_NEG 10
#define IC_MINOR 11
#define IC_MAJOR 12
#define IC_ASSING 13
#define IC_IF 14
#define IC_WHILE 15
#define IC_LABEL 16
#define IC_JUMP 17
#define IC_RETINT 18
#define IC_RETBOOL 19
#define IC_RETVOID 20
#define IC_PPARAM 21
#define IC_CALL 22
#define IC_LOAD 23
#define IC_BEGIN_FUNCTION 24
#define IC_END_FUNCTION 25
#define IC_LOAD_P 26

/* DECLARACIÓN DE TIPOS */


int stackSize;
/* Define una instrucción de código intermedio.
    instr: Tipo de instrucción.
    oper1: Primer operando.
    oper2: Segundo operando.
    result: Resultado de la operación.
    En algunos casos, no es necesario que los operandos y resultado esten 
    completos (pueden ser null).
*/
typedef struct OpThreeDir{
    int stackSize;
    int instr;
    item *oper1;
    item *oper2;
    item *result;
}OpThreeDir;

/* Define una lista de instrucciones de código intermedio.
  operation: Instrucción de código intermedio.
  next: Siguiente instrucción.
*/
typedef struct ListThreeDir{
    OpThreeDir *operation;
    struct ListThreeDir *next;
}ListThreeDir;

/* Punteros a la cabeza de la lista y al último elemento de ella, respectivamente. */
static ListThreeDir *head;
static ListThreeDir *last;

/* Contadores y variables para generar los nombres de labels y temporales. */
int labels = 0;
char label[32];
int temps = 0;
char tmp[32];

/* Utilizada para indicar si la compilacion se ejecuto en modo debug.*/
bool debug;

/* DECLARACIÓN DE FUNCIONES */

char *generateLabel();
char *generateTemp();
void initListThreeDir(bool deb);
ListThreeDir *getIntermediateCode();
void generate(item *function);
void generateInterCode (node *tree);
void showOperation ();
void showOperation2 (OpThreeDir *operation);
void insertOperation (OpThreeDir *operation);
void generateConstant (node *tree);
void generateAssing (node *tree);
void generateEqual(node *tree);
void generateOpArit(node *tree);
void generateOpAritUn(node *tree);
void generateOpLog(node *tree);
void generateOpLogUn(node *tree);
void generateOpRel(node *tree);
void generateFunctionCall(node *tree);
void generateIf(node *tree);
void generateIfElse(node *tree);
void generateWhile(node *tree);
void generateReturnVoid(node *tree);
void generateReturnExp(node *tree);
item *setVar(node *tree);
void loadParameters(itemFunc *func);


/* IMPLEMENTACION DE FUNCIONES */

/* Genera un nuevo nombre para label distinto a todos los anteriores. */
char *generateLabel(){
    char c[32];
    sprintf(c,"%d",labels);
    strcpy(label,"Label ");
    strcat (label, c);
    labels++;
    return label;
}

/* Genera un nuevo nombre para temporal distinto a todos los anteriores. */
char *generateTemp(){
    char c[32];
    sprintf(c,"%d",temps);
    strcpy(tmp,"Temp ");
    strcat (tmp, c);
    temps++;
    stackSize++;
    return tmp;
}

/* Inicialización de la lista de instrucciones con elemento ficticio.*/
void initListThreeDir(bool deb){
    head =(ListThreeDir *) malloc(sizeof(ListThreeDir));
    head->next = NULL;
    last = head;
    debug = deb;
}

ListThreeDir *getIntermediateCode(){
    return head;
}


/* Método principal, invocado desde yacc para cada función cargada en la tabla de símbolos. 
    Se encarga de generar los labels de inicio y fin de la función, y el código intermedio 
    correspondiente a la misma (utilizando los métodos correspondientees).
*/
void generate(item *func){
    item *funcionName = (item *) malloc(sizeof(item));
    strcpy(funcionName->name, func->name);

    OpThreeDir *beginFunction = (OpThreeDir *) malloc(sizeof(OpThreeDir));
    beginFunction->stackSize = func->function->stackSize;
    stackSize = func->function->stackSize;
    beginFunction->instr = IC_BEGIN_FUNCTION;
    beginFunction->result = funcionName;
    insertOperation(beginFunction);
    loadParameters(func->function);

    generateInterCode(func->function->tree);

    OpThreeDir *endFunction = (OpThreeDir *) malloc(sizeof(OpThreeDir));
    endFunction->instr = IC_END_FUNCTION;
    endFunction->result = funcionName;
    beginFunction->stackSize = stackSize;
    insertOperation(endFunction);
}

/* Método general para la generación de la lista de instrucciones.
    Mediante un case sobre el tipo del nodo corriente, invoca al método
    correspondiente para generar su código intermedio.
*/
void generateInterCode (node *tree){
    if (tree->content != NULL){
        if(debug){
            printf("Contenido de tipo %d (remitirse a structures.c)\n", tree->content->type );
        }
        switch (tree->content->type){
            case VAR:
               //hace funcion separada
            break;

            case CONSTANT :
                generateConstant(tree);
            break;

            case OPER_AR :
                generateOpArit(tree);
            break;

            case OPER_LOG :
                generateOpLog(tree);
            break;

            case OPER_REL :
                generateOpRel(tree);
            break;

            case FUNCTION_CALL_NP :
                generateFunctionCall(tree);
            break;

            case FUNCTION_CALL_P :
                generateFunctionCall(tree);             
            break;

            case PARAMETER :
                //generateLoad(tree);
            break;

            case IFAUX :
                generateIf(tree);
            break;

            case IF_ELSE :
                generateIfElse(tree);
            break;

            case ASSIGN :
                generateAssing(tree);
            break;

            case WHILEAUX :
                generateWhile(tree);
            break;

            case RETURNAUX :
                generateReturnVoid(tree);
            break;

            case RETURN_EXPR :
                generateReturnExp(tree);
            break;

            case STATEMENTS :
                generateInterCode(tree->left);
                generateInterCode(tree->right);
            break;

            case BLOCK :
                generateInterCode(tree->left);
            break;

            case OPER_AR_UN :
                generateOpAritUn(tree);
            break;

            case OPER_LOG_UN :
                generateOpLogUn(tree);
            break;

            case OPER_EQUAL :
                generateEqual(tree);
            break;
        }
    }
}

/* Inserta la operación pasada como parámetro en la lista de instrucciones. */
void insertOperation (OpThreeDir *operation){
    ListThreeDir *aux = (ListThreeDir *) malloc(sizeof(ListThreeDir));
    last->next = aux;
    last = last->next;
    last->next = NULL;
    last->operation = operation;
}

/* Imprime por consola una representación textual de la lista de instrucciones generada. */
void showOperation (){
    ListThreeDir *aux = head;
    if(aux->next!=NULL){
        aux = aux->next;
        while(aux!=NULL){
            OpThreeDir *operation = aux->operation;
            switch (operation->instr) {
                case IC_ADD : {
                    printf("ADD               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_SUB : {
                    printf( "SUB               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_PLUS : {
                    printf("PLUS              ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_DIV : {
                    printf( "DIV               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_MOD : {
                    printf( "MOD               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_AND : {
                    printf( "AND               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_OR : {
                    printf( "OR                ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_NOT : {
                    printf( "NOT               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_EQUALAR : {
                    printf( "EQAR              ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_EQUALLOG : {
                    printf( "EQLOG             ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_NEG : {
                    printf( "NEG               ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_MINOR : {
                    printf( "MINNOR            ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_MAJOR : {
                    printf( "MAJOR             ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_ASSING : {
                    printf( "ASSIGN            ");
                    printf("%s           ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_IF : {
                    printf( "IF                ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_WHILE : {
                    printf( "WHILE             ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_LABEL : {
                    printf( "LABEL             ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_JUMP : {
                    printf( "JUMP              ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_RETINT : {
                    printf( "RETINT            ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_RETBOOL : {
                    printf( "RETBOOL           ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_RETVOID : {
                    printf( "RETVOID           ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_PPARAM : {
                    printf( "PPARAM            ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_CALL : {
                    printf( "CALL              ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_LOAD : {
                    printf( "LOAD              ");
                    if (operation->oper1->type == VAR || operation->oper1->type == PARAMETER){
                        printf("%s              ",operation->oper1->name);
                        printf("%s      ",operation->oper2->name);
                        printf("%s\n",operation->result->name);
                    }
                    else{
                        if(operation->oper1->ret == INTEGERAUX){
                            printf("%d           ",operation->oper1->value);
                            printf("%s      ",operation->oper2->name);
                            printf("%s\n",operation->result->name);
                        }
                        else{
                            printf("%s              ",operation->oper1->value? "true":"false");
                            printf("%s      ",operation->oper2->name);
                            printf("%s\n",operation->result->name);
                        }
                    }
                    break;
                }
                case IC_BEGIN_FUNCTION : {
                    printf("\n");
                    printf( "BEGIN FUNCTION    ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_END_FUNCTION : {
                    printf( "END FUNCTION      ");
                    printf("%s      ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
                case IC_LOAD_P : {
                    printf( "IC_LOAD_P         ");
                    printf("%s         ",operation->oper1->name);
                    printf("%s      ",operation->oper2->name);
                    printf("%s\n",operation->result->name);
                    break;
                }
            }
            aux = aux->next;
        }
    }
}


/* Genera las lineas de código intermedio correspondientes a la carga de variables, constantes, parametros, etc. */
void generateConstant (node *tree){
    OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
    item *result = (item *) malloc(sizeof(item));
    operation->oper1 = tree->content;
    strcpy(result->name,generateTemp());
    result->offSet = stackSize;
    result->value = tree->content->value;
    result->type = CONSTANT;
    result->ret = tree->content->ret;
    result->offSet = stackSize;
    operation->instr = IC_LOAD;
    operation->result = result;
    insertOperation(operation);
}

/* Genera las lineas de código intermedio correspondientes a una asignación. */
void generateAssing (node *tree){
  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  operation->instr = IC_ASSING;

  // Variable
  item *variable = (item *) malloc(sizeof(item));
  strcpy(variable->name,tree->right->content->name);
  operation->oper1 = variable;

  //Expresion
  /*generateInterCode(tree->left);
  operation->result = last->operation->result;*/
  operation->result=setVar(tree->left);

  insertOperation(operation);
}

item *setVar(node *tree){
    if (tree->content!=NULL){
        int type = tree->content->type;
        if (type==VAR || type==PARAMETER){
            return tree->content;
        }
        else{
            generateInterCode(tree);
            return last->operation->result;
        }
    }
    return NULL;
}

/* Genera las lineas de código intermedio correspondientes a una comparación por igual (aritmética o lógica). */
void generateEqual(node *tree){
  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  item *result = (item *) malloc(sizeof(item));
  /*generateInterCode(tree->left);
  operation->oper1 = last->operation->result;

  generateInterCode(tree->right);
  operation->oper2 = last->operation->result;*/
  operation->oper1=setVar(tree->left);
  operation->oper2=setVar(tree->right);
  result->value = 0;
  result->type = VAR;

  if(tree->content->ret == INTEGERAUX){
    operation->instr = IC_EQUALAR;
    strcpy(result->name,generateTemp());
  }
  else{
    operation->instr = IC_EQUALLOG;
    strcpy(result->name,generateTemp());
  }
  result->offSet = stackSize;
  operation->result = result;

  insertOperation(operation);
}

/* Genera las lineas de código intermedio correspondientes a una operación aritmética binaria. */
void generateOpArit(node *tree){
  char name[32];
  strcpy(name,tree->content->name);

  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  item *result = (item *) malloc(sizeof(item));

  /*generateInterCode(tree->left);
  operation->oper1 = last->operation->result;

  generateInterCode(tree->right);
  operation->oper2 = last->operation->result;*/
  operation->oper1=setVar(tree->left);
  operation->oper2=setVar(tree->right);
  result->value = 0;
  result->type = VAR;

  if (strcmp(name,"OP_ADD")==0){
    operation->instr = IC_ADD;
    strcpy(result->name,generateTemp());
  }
  if (strcmp(name,"OP_SUB")==0){
    operation->instr = IC_SUB;
    strcpy(result->name,generateTemp());
  }
  if (strcmp(name,"OP_PROD")==0){
    operation->instr = IC_PLUS;
    strcpy(result->name,generateTemp());
  }
  if (strcmp(name,"OP_DIV")==0){
    operation->instr = IC_DIV;
    strcpy(result->name,generateTemp());
  }
  if (strcmp(name,"OP_MOD")==0){
    operation->instr = IC_MOD;
    strcpy(result->name,generateTemp());
  }
  result->offSet = stackSize;
  operation->result=result;

  insertOperation(operation);
}

/* Genera las lineas de código intermedio correspondientes a una operación aritmética unaria. */
void generateOpAritUn(node *tree){
  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  operation->instr = IC_NEG;
  item *result = (item *) malloc(sizeof(item));
  /*generateInterCode(tree->left);
  operation->oper1 = last->operation->result;*/
  operation->oper1=setVar(tree->left);
  result->value = 0;
  result->type = VAR;
  result->ret=INTEGERAUX;
  strcpy(result->name,generateTemp());
  result->offSet = stackSize;
  operation->result=result;

  insertOperation(operation);

}

/* Genera las lineas de código intermedio correspondientes a una operación lógica binaria. */
void generateOpLog(node *tree){
  char name[32];
  strcpy(name,tree->content->name);

  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  item *result = (item *) malloc(sizeof(item));

  /*generateInterCode(tree->left);
  operation->oper1 = last->operation->result;

  generateInterCode(tree->right);
  operation->oper2 = last->operation->result;*/
  operation->oper1=setVar(tree->left);
  operation->oper2=setVar(tree->right);

  result->ret=BOOLAUX;
  result->value = 0;
  result->type = VAR;

  if (strcmp(name,"OP_AND")==0){
    operation->instr = IC_AND;
    strcpy(result->name,generateTemp());
  }
  if (strcmp(name,"OP_OR")==0){
    operation->instr = IC_OR;
    strcpy(result->name,generateTemp());
  }
  result->offSet = stackSize;
  operation->result=result;

  insertOperation(operation);
}

/* Genera las lineas de código intermedio correspondientes a una operación lógica unaria. */
void generateOpLogUn(node *tree){
  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  operation->instr = IC_NOT;
  item *result = (item *) malloc(sizeof(item));

  /*generateInterCode(tree->left);
  operation->oper1 = last->operation->result;*/
  operation->oper1=setVar(tree->left);

  result->value = 0;
  result->type = VAR;
  result->ret=BOOLAUX;
  strcpy(result->name,generateTemp());
  result->offSet = stackSize;
  operation->result=result;

  insertOperation(operation);
}

/* Genera las lineas de código intermedio correspondientes a una comparación por mayor o menor (int). */
void generateOpRel(node *tree){
  char name[32];
  strcpy(name,tree->content->name);
  OpThreeDir *operation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  item *result = (item *) malloc(sizeof(item));

  /*generateInterCode(tree->left);
  operation->oper1 = last->operation->result;

  generateInterCode(tree->right);
  operation->oper2 = last->operation->result;*/
  operation->oper1=setVar(tree->left);
  operation->oper2=setVar(tree->right);


  result->value = 0;
  result->type = VAR;
  result->ret=BOOLAUX;

  if (strcmp(name,"OP_MINOR")==0){
    operation->instr = IC_MINOR;
    strcpy(result->name,generateTemp());
  }
  if (strcmp(name,"OP_MAJOR")==0){
    operation->instr = IC_MAJOR;
    strcpy(result->name,generateTemp());
  }
  result->offSet = stackSize;
  operation->result=result;

  insertOperation(operation);
}

/* Genera las lineas de código intermedio correspondientes a una llamada a función. */
void generateFunctionCall(node *tree){
  // Load params (if it has)
  if(tree->content->type==FUNCTION_CALL_P){
    paramsCall *currentParam = tree->content->paramsCall;
    if(currentParam->next!=NULL){
      currentParam = currentParam->next;
      int i=0;
      while (currentParam!=NULL){
        i++;
        //generateInterCode(currentParam->param);
        OpThreeDir *loadParam = (OpThreeDir *) malloc(sizeof(OpThreeDir));
        loadParam->instr = IC_PPARAM;
        //loadParam->result = last->operation->result;
        loadParam->oper1 = setVar(currentParam->param);
        item *result = (item *) malloc(sizeof(item));
        result->value = i;
        result->type = VAR;
        result->ret = BOOLAUX;
        switch (i){
          case 1:
            strcpy(result->name,"RDI");
          break;

          case 2:
            strcpy(result->name,"RSI");
          break;

          case 3:
            strcpy(result->name,"RDX");
          break;

          case 4:
            strcpy(result->name,"RCX");
          break;

          case 5:
            strcpy(result->name,"R8");
          break;

          case 6:
            strcpy(result->name,"R9");
          break;

          default:
            strcpy(result->name,"RSP");
          break;
        }
        loadParam->result = result; 
        insertOperation(loadParam);
        currentParam = currentParam->next;
      }
    }
  }
  // Load function call
  OpThreeDir *functionCall = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  functionCall->instr = IC_CALL;

  item *functionName = (item *) malloc(sizeof(item));
  strcpy(functionName->name, tree->content->name);
  functionCall->oper1 = functionName;

  item *result = (item *) malloc(sizeof(item));
  strcpy(result->name, generateTemp());
  result->offSet = stackSize;
  functionCall->result = result;

  insertOperation(functionCall);
}

/* Genera las lineas de código intermedio correspondientes a un if-then. */
void generateIf(node *tree){
  // Labels
  item *labelEnd = (item *) malloc(sizeof(item));
  strcpy(labelEnd->name,generateLabel());

  // Main instruction
  OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  mainOperation->instr = IC_IF;
  mainOperation->oper2 = labelEnd;

  // Condition
  /*generateInterCode(tree->left);
  mainOperation->result = last->operation->result;*/
  mainOperation->result = setVar(tree->left);

  // Insert if instruction
  insertOperation(mainOperation);

  // Then
  generateInterCode(tree->right);

  // Label End
  OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  instrLabelEnd->instr = IC_LABEL;
  instrLabelEnd->result = labelEnd;
  insertOperation(instrLabelEnd);
}

/* Genera las lineas de código intermedio correspondientes a un if-then-else. */
void generateIfElse(node *tree){
  // Labels
  item *labelElse = (item *) malloc(sizeof(item));
  strcpy(labelElse->name,generateLabel());

  item *labelEnd = (item *) malloc(sizeof(item));
  strcpy(labelEnd->name,generateLabel());

  // Main instruction
  OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  mainOperation->instr = IC_IF;
  mainOperation->oper1 = labelElse;
  mainOperation->oper2 = labelEnd;

  // Condition
  /*generateInterCode(tree->left);
  mainOperation->result = last->operation->result;*/
  mainOperation->result = setVar(tree->left);

  // Insert if instruction
  insertOperation(mainOperation);

  // Then
  generateInterCode(tree->middle);

  // Jump
  OpThreeDir *jumpToEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  jumpToEnd->instr = IC_JUMP;
  jumpToEnd->result = labelEnd;
  insertOperation(jumpToEnd);

  // Label Else
  OpThreeDir *instrLabelElse = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  instrLabelElse->instr = IC_LABEL;
  instrLabelElse->result = labelElse;
  insertOperation(instrLabelElse);

  // Else
  generateInterCode(tree->right);

  // Label End
  OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  instrLabelEnd->instr = IC_LABEL;
  instrLabelEnd->result = labelEnd;
  insertOperation(instrLabelEnd);
}

/* Genera las lineas de código intermedio correspondientes a un ciclo while. */
void generateWhile(node *tree){
  // Labels
  item *labelWhile = (item *) malloc(sizeof(item));
  strcpy(labelWhile->name,generateLabel());
  item *labelEnd = (item *) malloc(sizeof(item));
  strcpy(labelEnd->name,generateLabel());

  // Main instruction
  OpThreeDir *mainOperation = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  mainOperation->instr = IC_WHILE;
  mainOperation->oper1 = labelEnd;

  // Label While
  OpThreeDir *instrLabelWhile = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  instrLabelWhile->instr = IC_LABEL;
  instrLabelWhile->result = labelWhile;
  insertOperation(instrLabelWhile);

  // Condition
  /*generateInterCode(tree->left);
  mainOperation->result = last->operation->result;*/
  mainOperation->result = setVar(tree->left);    

  // Insert while instruction
  insertOperation(mainOperation);

  // Block
  generateInterCode(tree->right);

  // Jump
  OpThreeDir *jumpToCond = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  jumpToCond->instr = IC_JUMP;
  jumpToCond->result = labelWhile;
  insertOperation(jumpToCond);

  // Label End
  OpThreeDir *instrLabelEnd = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  instrLabelEnd->instr = IC_LABEL;
  instrLabelEnd->result = labelEnd;
  insertOperation(instrLabelEnd);
}

/* Genera las lineas de código intermedio correspondientes a un return void. */
void generateReturnVoid(node *tree){
  OpThreeDir *returnVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  returnVoid->instr = IC_RETVOID;
  insertOperation(returnVoid);
}

/* Genera las lineas de código intermedio correspondientes a un return de una expresión (int o bool). */
void generateReturnExp(node *tree){
  OpThreeDir *returnNotVoid = (OpThreeDir *) malloc(sizeof(OpThreeDir));
  if(tree->content->ret==INTEGERAUX){
    returnNotVoid->instr = IC_RETINT;
  } else {
    returnNotVoid->instr = IC_RETBOOL;
  }
  /*if(tree->left->content->type==VAR){
    item *variable = (item *) malloc(sizeof(item));
    strcpy(variable->name,tree->left->content->name);
    variable->offSet = stackSize;
    returnNotVoid->result = variable;
    } else {
      generateInterCode(tree->left);
      returnNotVoid->result = last->operation->result;
    }*/
  returnNotVoid->result = setVar(tree->left);
  insertOperation(returnNotVoid);
}


void loadParameters(itemFunc *func){
  symbol *pf= func->params;
  int i = 0;
  if(pf!=NULL){
    pf = pf->next;
    while (pf!=NULL){
      i++;
      OpThreeDir *load = (OpThreeDir *) malloc(sizeof(OpThreeDir));
      load->instr = IC_LOAD_P;
      item *registro = (item *) malloc(sizeof(item));
      registro->value = i;
      registro->type = pf->content->type;
      registro->ret = pf->content->ret;
      switch(i){
        case 1:
          strcpy(registro->name,"RDI");
        break;

        case 2:
          strcpy(registro->name,"RSI");
        break;

        case 3:
          strcpy(registro->name,"RDX");
        break;

        case 4:
          strcpy(registro->name,"RCX");
        break;

        case 5:
          strcpy(registro->name,"R8");
        break;

        case 6:
          strcpy(registro->name,"R9");
        break;

        default:
          strcpy(registro->name,"RSP");
        break;
      }
      stackSize++;
      pf->content->offSet = stackSize;
      load->oper1 = registro;
      load->result = pf->content;
      insertOperation(load);
      pf= pf->next;
    }
  }
}
