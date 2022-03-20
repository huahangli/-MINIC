/****************************************************/
/* File: util.h                                     */
/* The utils implementation for MINIC compiler      */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/


#ifndef UTIL_H
#define UTIL_H
#include<globals.h>
#include<QString>


void printType(Type);

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken( TokenType, const char*);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind);

/* Function newStmtNode creates a new Declare
 * node for syntax tree construction
 */
TreeNode * newDeclareNode (DeclareKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString( char * );

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode *);


#endif // UTIL_H
