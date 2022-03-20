/****************************************************/
/* File: scan.h                                     */
/* The scanner implementation for MINIC compiler    */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/


#ifndef SCAN_H
#define SCAN_H
#include<globals.h>

/* MAXTOKENLEN is the maximum size of a token */
#define MAXTOKENLEN 40

/* tokenString array stores the lexeme of each token */
extern char tokenString[MAXTOKENLEN+1];

/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void);



#endif // SCAN_H
