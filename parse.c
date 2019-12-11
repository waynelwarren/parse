// parse a simple expression with single-digit numbers, single character identifiers,
// with parenthesis, *, /, + and - as operators.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {none, expr, trm, fctr, lparen, rparen, ident, number, times, slash, plus, minus} Symbol;

Symbol sym;
int val;
FILE* fp;
void nextsym(void);
char *name(Symbol);

int expect(Symbol s) {
    if (sym == s) {
        nextsym();
        return 1;
    }
    fprintf(stderr, "expect: unexpected symbol %s, %d(%c)\n", name(sym), val, val);
    exit(1); // return 0;
}

typedef struct node {
    Symbol nodeType;
    int    nodeValue;
    struct node* nodeParent;
    struct node* nodeFirst;
    struct node* nodeLast;
    struct node* nodeNext;
    struct node* nodePrev;
} node_t;

node_t* newNode(void) {
    node_t* nn = (node_t*)malloc(sizeof(node_t));
    nn->nodeType   = none;
    nn->nodeValue  = 0;
    nn->nodeParent = NULL;
    nn->nodeFirst  = NULL;
    nn->nodeFirst  = NULL;
    nn->nodeNext   = NULL;
    nn->nodePrev   = NULL;
    return nn;
}

void addNodeLast(node_t* parent, node_t* child) {
    child->nodeParent = parent;
    if (parent->nodeFirst == NULL) {
        parent->nodeFirst = child;
        parent->nodeLast =  child;
    }
    else {
        parent->nodeLast->nodeNext = child;
        parent->nodeLast = child;
        child->nodePrev  = parent->nodeLast;
    }
}

void printTree(node_t* node, int level) {
    if (node == NULL)
        return;
    for (int i=0; i<level; i++)
        printf("   ");
    while (node->nodeType == expr || node->nodeType == none)
        node = node->nodeFirst;
    printf("%s = %c\n", name(node->nodeType), node->nodeValue);
    for (node_t* child=node->nodeFirst; child!=NULL; child=child->nodeNext)
        printTree(child, level+1);
}

int symbol(Symbol sym);

void printPrefix(node_t* node) {
    if (node == NULL)
        return;
    if (node->nodeType> rparen) {
        if (node->nodeType == ident || node->nodeType == number)
            printf("%c", node->nodeValue);
        else
            printf("%c", symbol(node->nodeType));
    }
    for (node_t* child=node->nodeFirst; child!=NULL; child=child->nodeNext)
        printPrefix(child);
}

void printPostfix(node_t* node) {
    if (node == NULL)
        return;
    for (node_t* child=node->nodeFirst; child!=NULL; child=child->nodeNext)
        printPostfix(child);
    if (node->nodeType> rparen) {
        if (node->nodeType == ident || node->nodeType == number)
            printf("%c", node->nodeValue);
        else
            printf("%c", symbol(node->nodeType));
    }
}

void printInfix(node_t* node) {
    if (node == NULL)
        return;
    while (node->nodeType == expr || node->nodeType == none)
        node = node->nodeFirst;
    printInfix(node->nodeFirst);
    if (node->nodeType> rparen) {
        if (node->nodeType == ident || node->nodeType == number)
            printf("%c", node->nodeValue);
        else
            printf("%c", symbol(node->nodeType));
    }
    printInfix(node->nodeLast);
}

//  expression = term | term [+-] expression
 
//  term = factor | factor [*/] term
 
//  factor = ident | number | ( expression )

node_t* expression(void);

node_t* factor() {
    // fprintf(stderr, "factor %s,%c\n", name(sym), val);
    node_t* factNode = newNode();
    if (sym==ident) {
        factNode->nodeValue = val;
        factNode->nodeType = ident;
        nextsym();
    } else if (sym==number) {
        factNode->nodeValue = val;
        factNode->nodeType = number;
        nextsym();
    } else if (sym==lparen) {
        nextsym();
        node_t* exprNode = expression();
        addNodeLast(factNode, exprNode);
        expect(rparen);
    } else {
        fprintf(stderr, "factor: syntax error?\n");
        nextsym();
    }
    return factNode;
}

node_t* term() {
    // printf("term %s,%c\n", name(sym), val);
    node_t* termNode = newNode();
    node_t* factNode = factor();
    addNodeLast(termNode, factNode);
    while (sym==times || sym==slash) {
        termNode->nodeType = sym;
        nextsym();
        factNode = term();
        addNodeLast(termNode, factNode);
    }
    return termNode;
}

node_t* expression() {
    // printf("expression %s,%c\n", name(sym), val);
    node_t* exprNode = newNode();
    node_t* termNode = term();
    addNodeLast(exprNode, termNode);
    exprNode->nodeType = expr;
    exprNode->nodeValue = 0;
    while (sym==plus || sym==minus) {
        exprNode->nodeType = sym;
        nextsym();
        termNode = expression();
        addNodeLast(exprNode, termNode);
    }
    return exprNode;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        fp = stdin;
    else
        fp = fopen(argv[1], "r");
    nextsym();
    node_t* exprNode = expression();
    putchar('\n');
    printf("original tree:\n");
    printTree(exprNode, 0);
    printPrefix(exprNode);
    putchar('\n');
    printPostfix(exprNode);
    putchar('\n');
    printInfix(exprNode);
    putchar('\n');
    if (fp != stdin)
        fclose(fp);
}

void nextsym(void) {
    int c;
    c=fgetc(fp);
    if (c != -1)
        putchar(c);
    val = c;
    if (c=='(')
        sym = lparen;
    else if (c==')')
        sym = rparen;
    else if (c=='*')
        sym = times;
    else if (c=='/')
        sym = slash;
    else if (c=='+')
        sym = plus;
    else if (c=='-')
        sym = minus;
    else if (c>='A' && c<='Z' || c>='a' && c<='z')
        sym = ident;
    else if (c>='0' && c<= '9')
        sym = number;
    else if (c=='\n')
        nextsym();
    else if (c == -1)
        printf("End of input\n");
}

int symbol(Symbol sym) {
    switch (sym) {
        case times:
            return '*';
        case slash:
            return '/';
        case plus:
            return '+';
        case minus:
            return '-';
        default:
            return ' ';
    }

}
char* name(Symbol sym) {
    switch (sym) {
        case none:
            return "none";
        case ident:
            return "ident";
        case number:
            return "number";
        case lparen:
            return "lparen";
        case rparen:
            return "rparen";
        case times:
            return "times";
        case slash:
            return "slash";
        case plus:
            return "plus";
        case minus:
            return "minus";
        case expr:
            return "expr";
        case trm:
            return "trm";
        case fctr:
            return "fctr";
    }
    return "";
}
