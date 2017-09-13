#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct tokensStr{
    char value[32];
    int line;
} tokenStr;

typedef struct tokensVal{
    int value;
    int line;
} tokenVal;
