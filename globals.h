/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for MINIC compiler         */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/


#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8

typedef enum
{
    /* book-keeping tokens */
    ENDFILE,ERROR,
    /* reserved words */
    IF, ELSE, INT, FLOAT, RETURN, VOID, WHILE,DO,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols */
    ASSIGN,EQ,LT,MT,UEQ,LEQ,MEQ,PLUS,MINUS,TIMES,OVER,MOD,
    LPAREN,RPAREN,SMC,COMM,LBRACKETS,RBRACKETS,LBRACES,RBRACES

} TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */


/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/


typedef enum {StmtK,ExpK,DeclareK} NodeKind;
typedef enum {ExpStmt,CompundStmt,ConditionStmt,WhileStmt,ReturnStmt,DoStmt} StmtKind;
typedef enum {OpK,ConstK,IdK,DefK,RetK,TypeK,AssignK,CallK,ArguK,VariaK} ExpKind;
typedef enum {VariableDef,FuncDef,Parameters,CompoundK} DeclareKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Float} Type;

#define MAXCHILDREN 4

typedef struct treeNode
{
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp;DeclareKind def;} kind;
    union {

        TokenType op;
        int val;
        char * name; } attr;
    int len = 0;
    Type type; /* for type checking of exps */
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;


#endif //GLOBALS_H
