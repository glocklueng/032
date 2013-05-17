/*****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
June 2010

Copyright 2010 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

Fuse settings:

Compiled with GCC, -O2 optimizations

*****************************************************************************/

#ifndef _PARSE_H
#define _PARSE_H

#include "stdint.h"

#ifndef M_E
#define	M_E		    2.7182818284590452354
#endif

typedef enum {
        NONE    = 0,
        LEVEL0  = 1,
        LEVEL1  = 2,
        LEVEL2  = 3,
        LEVEL3  = 4,
        LEVEL4  = 5,
        LEVEL5  = 6
} PRECEDENCE;

typedef enum {
        KEYWORD     = 1,
        IDENTIFIER  = 2,
        NUMBER      = 3,
        OPERATOR    = 4,
        PUNCTUATION = 5
} TOKENCLASS;

typedef struct {
    char *Token;
    TOKENCLASS Cls;
    PRECEDENCE PrecedenceLevel;
} mcSymbol;

/* First in - First out stack */
struct list {
    struct list *next;
    mcSymbol sym;
};

typedef struct list item;
typedef item* Queue;


int IsNumeric(char expression[]);
int IndexOf(char s[], char c);
char *Substring(char s[], int start, int end);
uint8_t is_function(char token[]);
void evaluate(char expression[]);
void calc_scan(char str[], Queue *symbols);
double identifier(item *symbol);
double calc_function(item *symbol, double *args);
double level0(Queue *tokens);
double level1_prime(Queue *tokens, double result);
double level1(Queue *tokens);
double level2(Queue *tokens);
double level2_prime(Queue *tokens, double result);
double level3(Queue *tokens);
double level3_prime(Queue *tokens, double result);
double level4(Queue *tokens);
double level4_prime(Queue *tokens);
double level5(Queue *tokens);
double level5_prime(Queue *tokens, double result);
double level6(Queue *tokens);
double *arguments(Queue *tokens);

extern uint8_t m_error;
extern double ans,x,y;
/* Error codes */
#define NoNumber    1
#define INVALID     2   /* Invalid expression */
#define NOMEMORY    3

#endif
