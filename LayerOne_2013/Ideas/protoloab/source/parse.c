/*****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
July 2010

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License

www.gabotronics.com
email me at: gabriel@gabotronics.com

*****************************************************************************/

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include "parse.h"

/* GLOBALS */

int m_State[7][7] = {{2, 4, 1, 1, 4, 6, 7},
                     {2, 3, 3, 3, 3, 3, 3},
                     {1, 1, 1, 1, 1, 1, 1},
                     {2, 4, 5, 5, 4, 5, 5},
                     {1, 1, 1, 1, 1, 1, 1},
                     {1, 1, 1, 1, 1, 1, 1},
                     {1, 1, 1, 1, 1, 1, 1}};


#define MAXARGS 8       /* Maximum number of arguments a function can have */

#define NUMFUNCTIONS 20
char *m_funcs[NUMFUNCTIONS] = { /* Keep functions in this order */
    "sin", "cos", "tan", "asin", "acos","atan", "sqrt", "max","min",
    "floor","ceiling", "log", "log10","ln", "round", "abs", "neg",
    "sinh", "cosh", "tanh" };

#define NUMOPERATORS 13
mcSymbol m_operators[NUMOPERATORS] = {
    { "-", OPERATOR, LEVEL1  },
    { "+", OPERATOR, LEVEL1  },
    { "*", OPERATOR, LEVEL2  },
    { "/", OPERATOR, LEVEL2  },
    { "\\", OPERATOR, LEVEL2  },
    { "%", OPERATOR, LEVEL2  },
    { "^", OPERATOR, LEVEL3  },
    { "!", OPERATOR, LEVEL5  },
    { "&", OPERATOR, LEVEL5  },
    { "-", OPERATOR, LEVEL4  },
    { "+", OPERATOR, LEVEL4  },
    { "(", OPERATOR, LEVEL5  },
    { ")", OPERATOR, LEVEL0  }
};

/* All operators */
char m_colstring[] = "\t .()!%&()*++--/\\^";

/* error */
uint8_t m_error=0;

double ans; /* last answer */
double x,y;

/* Stack implementation First In - First Out */

/* Create new empty item */
item *createnew(void) {
    item *pointer;
    pointer = (item *)malloc(sizeof(item));
    if(pointer==NULL) {
        m_error = NOMEMORY;
        return NULL;
    }
    pointer->next = NULL;
    pointer->sym.Token = NULL;
    pointer->sym.Cls = KEYWORD;
    pointer->sym.PrecedenceLevel = NONE;
    return pointer;
}

void del(item *a) {
    free(a->sym.Token);
    free(a);
}

/* Adds item I to the end of the stack */
void Enqueue(Queue *Q, item *I) {
    item *p;
    I->next=NULL;
    if(*Q==NULL) {
        *Q=I;
    }
    else {
        p=*Q;
        while(p->next!=NULL) p=p->next;
        p->next=I;
    }
}

/* Remove item at top of stack */
item *Dequeue(Queue *Q) {
    item *popitem;
    popitem = *Q;
    if(*Q!=NULL) {
        *Q=(*Q)->next;
        popitem->next=NULL;
    }
    return popitem;
}

/* Check if the string is a numeric value */
int IsNumeric(char s[]) {
    uint8_t point=0;
    char c;
    while (*s) {
        c=s[0];
        if(!isdigit(c)) {
            if(c=='.') point++;
            else return 0;
        }
        s++;
    }
    if(point>1) return 0;   /* More than one decimal point */
    return 1;
}

/* Returns the index of the first occurrence of the character in the string */
int IndexOf(char s[], char c) {
    int index=0;
    while (*s) {
        if((*s++)==c) return index;
        index++;
    }
    /* character not found */
    return -1;
}

/* Return a pointer to a new string, initialized to a substring of s */
char *Substring(char s[], int start, int length) {
    char *pointer;

    pointer = (char *)malloc(sizeof(char) * (length+1));
    if(pointer==NULL) {
        m_error = NOMEMORY;
        return 0;
    }
    else {
        strncpy(pointer, s+start, length);
        pointer[length]='\0';
    }
    return pointer;
}

/* 'Main' function, called from upper application */
void evaluate(char expression[]) {
    Queue symbols=NULL; /* New stack */
    m_error = 0;
    if (IsNumeric(expression)) ans=atof(expression);
    else {
        calc_scan(expression, &symbols);    /* Create tokens */
        ans=level0(&symbols);               /* Calculate */
    }
}

uint8_t is_function(char token[]) {
    int i;
    for(i=0; i<NUMFUNCTIONS; i++) {
        if(strcmp(token, m_funcs[i])==0) return 1;
    }
    return 0;
}

void calc_scan(char str[], Queue *symbols) {
    int sp=0;  /* start position marker */
    int cp=0;  /* current position marker */
    int col; /* input column */
    int lex_state=1;
    char cc;

    item *newitem;
    col = strlen(str);  
    str[col+1] = '\0';  /* Functions need to look ahead one character */
    str[col] = ' ';     /* add a space at the end */

    while(cp <= strlen(str)-1) { /* line.Length - 1 */
        cc = str[cp];  /* if cc is not found then IndexOf returns -1 giving col = 2. */
        col = IndexOf(m_colstring,cc) + 3;
        /* set the input column */
        if(col==2) { /* cc wasn't found in the column string */
            if(isalpha(cc)) col=1;
            else if(isdigit(cc)) col=2;
            else col=6; /* everything else assigned to the punctuation column */
        }
        else if(col>5) /* cc was found and is > 5 so must be in operator column */
            col = 7;
                    /* case else ' cc was found - col contains the correct column */
        /* find the new state based on current state and column (determined by input) */
        lex_state = m_State[lex_state - 1][col - 1];
        switch(lex_state) {
            case 3: /* function or variable  end state
                       TODO variables aren't supported but substitution
                            could easily be performed here or after
                            tokenization */
                newitem = createnew();    /* Create new empty item */
                newitem->sym.Token = Substring(str, sp, cp-sp);
                if(is_function(newitem->sym.Token))
                    newitem->sym.Cls = KEYWORD;
                else newitem->sym.Cls = IDENTIFIER;
                Enqueue(symbols, newitem);
                lex_state = 1;
                cp = cp - 1;
            break;
            case 5: /* number end state */
                newitem = createnew();
                newitem->sym.Token = Substring(str, sp, cp-sp);
                newitem->sym.Cls = NUMBER;
                Enqueue(symbols, newitem);
                lex_state = 1;
                cp = cp - 1;
            break;
            case 6: /* punctuation end state */
                newitem = createnew();
                newitem->sym.Token = Substring(str, sp, cp-sp+1);
                newitem->sym.Cls = PUNCTUATION;
                Enqueue(symbols, newitem);
                lex_state = 1;
            break;
            case 7: /* operator end state */
                newitem = createnew();
                newitem->sym.Token = Substring(str, sp, cp-sp+1);
                newitem->sym.Cls = OPERATOR;
                Enqueue(symbols, newitem);
                lex_state = 1;
            break;
        }
        cp += 1;
        if (lex_state == 1) sp = cp;
    }
    str[strlen(str)-1] = '\0';  /* restore to original string */
}

double calc_op(mcSymbol op, double operand1, double operand2) {
    int i;
    double res=1;

    switch(op.Token[0]) {
        case '&':   /* ' sample to show addition of custom operator */
            return 5;
        break;
        case '^': return (pow(operand1,operand2)); break;
        case '+':
            switch(op.PrecedenceLevel) {
                case LEVEL1: return (operand2 + operand1);
                case LEVEL4: return operand1;
            }
        break;
        case '-':
            switch(op.PrecedenceLevel) {
                case LEVEL1: return (operand1 - operand2);
                case LEVEL4: return -1 * operand1;
            }
        break;
        case '*': return (operand2 * operand1);
        case '/': return (operand1 / operand2);
        case '\\': /*return (CLng(operand1) \ CLng(operand2))*/
                    return ((long)(operand1) / (long)(operand2));
        case '%': return ((int)operand1 % (int)operand2);
        case '!':
            if(operand1 > 1) {
                for(i=(int)operand1; i>1; i--) {
                    res = res*i;
                }
            }
            return res;
    }
    return res;
}

double calc_function(item *symbol, double *args) {
    int fn;
    double val=0;
/*    "sin", "cos", "tan", "asin", "acos","atan", "sqrt", "max","min",
      "floor","ceiling", "log", "log10","ln", "round", "abs", "neg",
      "sinh", "cosh", "tanh" */
    for(fn=0; fn<NUMFUNCTIONS; fn++) {
        if(strcmp(m_funcs[fn], symbol->sym.Token)==0) break;
    }
    switch(fn) {
        case  0: val = sin(args[0]); break;
        case  1: val = cos(args[0]); break;
        case  2: val = tan(args[0]); break;
        case  3: val = asin(args[0]); break;
        case  4: val = acos(args[0]); break;
        case  5: val = atan(args[0]); break;
        case  6: val = sqrt(args[0]); break;
        case  7: if(args[0]>args[1]) val = args[0]; else val = args[1]; break;
        case  8: if(args[0]<args[1]) val = args[0]; else val = args[1]; break;
        case  9: val = floor(args[0]); break;
        case 10: val = ceil(args[0]); break;
        case 11: val = log10(args[0]); break;
        case 12: val = log10(args[0]); break;
        case 13: val = log(args[0]); break;
        case 14: val = floor(args[0]); break;     /* ----< verify */
        case 15: val = fabs(args[0]); break;
        case 16: val = -1*args[0]; break;
        case 17: val = sinh(args[0]); break;
        case 18: val = cosh(args[0]); break;
        case 19: val = tanh(args[0]); break;
    }
    del(symbol);
    free(args);
    return val;
}


double identifier(item *symbol) {
    int id;
    char *m_ids[] = { "e", "pi", "x", "y", "ans" };

    for(id=0; id<5; id++) {
        if(strcmp(m_ids[id], symbol->sym.Token)==0) break;
    }
    del(symbol);
    switch(id) {
        case 0: return M_E;
        case 1: return M_PI;
        case 2: return x;
        case 3: return y;
        case 4: return ans;
        /*     more symbols... */
        default: return 0;
    }
}

uint8_t is_operator(char *token, PRECEDENCE level, mcSymbol *op) {
    int ir;

    for(ir=0; ir<NUMOPERATORS; ir++) {
        if((strcmp(m_operators[ir].Token,token)==0) &&
           (level==m_operators[ir].PrecedenceLevel) ) break;
    }
    if(ir<NUMOPERATORS) { /* found operator */
        (*op).Token = m_operators[ir].Token;  /* Copying pointer! */
        (*op).Cls = m_operators[ir].Cls;
        (*op).PrecedenceLevel = m_operators[ir].PrecedenceLevel;
        return 1;
    }
    return 0;
}

/* Recusrsive Descent Parsing Functions */

double level0(Queue *tokens) {
    return level1(tokens);
}

double level1_prime(Queue *tokens, double result) {
    mcSymbol op,symbol;

    if(*tokens!=NULL) { /* copy to symbol by reference */
        symbol.Token = (*tokens)->sym.Token;
        symbol.Cls = (*tokens)->sym.Cls;
        symbol.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else return result;

    /* binary level1 precedence operators....+, - */
    if(is_operator(symbol.Token, LEVEL1, &op)) {
        del(Dequeue(tokens));
        result = calc_op(op, result, level2(tokens));
        result = level1_prime(tokens, result);
    }
    return result;
}

double level1(Queue *tokens) {
    return level1_prime(tokens, level2(tokens));
}

double level2(Queue *tokens) {
    return level2_prime(tokens, level3(tokens));
}

double level2_prime(Queue *tokens, double result) {
    mcSymbol op,symbol;

    if(*tokens!=NULL) { /* copy to symbol by reference */
        symbol.Token = (*tokens)->sym.Token;
        symbol.Cls = (*tokens)->sym.Cls;
        symbol.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else return result;

    /* binary level2 precedence operators....*, /, \, % */
    if(is_operator(symbol.Token, LEVEL2, &op)) {
        del(Dequeue(tokens));
        result = calc_op(op, result, level3(tokens));
        result = level2_prime(tokens, result);
    }
    return result;
}

double level3(Queue *tokens) {
    return level3_prime(tokens, level4(tokens));
}

double level3_prime(Queue *tokens, double result) {
    mcSymbol op,symbol;

    if(*tokens!=NULL) { /* copy to symbol by reference */
        symbol.Token = (*tokens)->sym.Token;
        symbol.Cls = (*tokens)->sym.Cls;
        symbol.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else return result;

    /* binary level3 precedence operators....^ */
    if(is_operator(symbol.Token, LEVEL3, &op)) {
        del(Dequeue(tokens));
        result = calc_op(op, result, level4(tokens));
        result = level3_prime(tokens, result);
    }
    return result;
}

double level4(Queue *tokens) {
    return level4_prime(tokens);
}

double level4_prime(Queue *tokens) {
    mcSymbol op,symbol;

    if(*tokens!=NULL) { /* copy to symbol by reference */
        symbol.Token = (*tokens)->sym.Token;
        symbol.Cls = (*tokens)->sym.Cls;
        symbol.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else {  /* Invalid expression */
        m_error = INVALID;
        return 0;
    }

    /* unary level4 precedence right associative  operators.... +, - */
    if(is_operator(symbol.Token, LEVEL4, &op)) {
        del(Dequeue(tokens));
        return calc_op(op, level5(tokens),0);
    }
    else return level5(tokens);
}

double level5(Queue *tokens) {
    return level5_prime(tokens, level6(tokens));
}

double level5_prime(Queue *tokens, double result) {
    mcSymbol op,symbol;

    if(*tokens!=NULL) { /* copy to symbol by reference */
        symbol.Token = (*tokens)->sym.Token;
        symbol.Cls = (*tokens)->sym.Cls;
        symbol.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else return result;

    /* unary level5 precedence left associative operators.... ! */
    if(is_operator(symbol.Token, LEVEL5, &op)) {
        del(Dequeue(tokens));
        return calc_op(op, result,0);
    }
    else return result;
}

double level6(Queue *tokens) {
    item *symbol;
    double val;
    symbol = createnew(); /* allocate space */

    if(*tokens!=NULL) { /* copy to symbol by reference */
        symbol->sym.Token = (*tokens)->sym.Token;
        symbol->sym.Cls = (*tokens)->sym.Cls;
        symbol->sym.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else {  /* Invalid expression */
        m_error = INVALID;
        del(symbol);
        return 0;
    }

    /* constants, identifiers, keywords, -> expressions */
    if(strcmp(symbol->sym.Token, "(")==0) { /* opening paren of new expression */
        del(Dequeue(tokens));
        free(symbol);
        val = level0(tokens);

        symbol=Dequeue(tokens); /* pop next item */

        if(symbol==NULL) {      /* Invalid expression */
            m_error = INVALID;
            return 0;
        }

        /* closing paren */
        if(strcmp(symbol->sym.Token,")")!=0) {
            m_error = INVALID;
        }
        del(symbol);
        return val;
    }
    else {
        switch(symbol->sym.Cls) {
            case IDENTIFIER:
                free(Dequeue(tokens));
                return identifier(symbol);
            break;
            case KEYWORD:
                free(Dequeue(tokens));
                return calc_function(symbol, arguments(tokens));
            break;
            case NUMBER:
                free(Dequeue(tokens));
                /*m_stack.Push(CDbl(symbol.Token))*/
                val = atof(symbol->sym.Token);
                del(symbol);
                return val;
            break;
            default:
                m_error = INVALID;
                del(symbol);
                return 0;
        }
    }
}

double *arguments(Queue *tokens) {
    item *symbol;
    double *args;
    uint8_t index=0;

    symbol=createnew();

    args = (double *)malloc(sizeof(double) * MAXARGS);

    if(*tokens!=NULL) { /* copy to symbol by reference*/
        symbol->sym.Token = (*tokens)->sym.Token;
        symbol->sym.Cls = (*tokens)->sym.Cls;
        symbol->sym.PrecedenceLevel = (*tokens)->sym.PrecedenceLevel;
    }
    else {  /* Invalid expression*/
        m_error = INVALID;
        del(symbol);
        free(args);
        return NULL;
    }

    if(strcmp(symbol->sym.Token,"(")==0) {
        del(Dequeue(tokens));
        args[index++]=level0(tokens);
        free(symbol);
        symbol=Dequeue(tokens);

        while(strcmp(symbol->sym.Token,")")!=0) {
            if(strcmp(symbol->sym.Token,",")==0) {
                args[index++]=level0(tokens);
            }
            else {
                m_error = INVALID;
                del(symbol);
                free(args);
                return NULL;
            }
            del(symbol);
            symbol = Dequeue(tokens);
        }
        del(symbol);
        return args;
    }
    else {
        m_error = INVALID;
        del(symbol);
        free(args);
        return NULL;
    }
}

