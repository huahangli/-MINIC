/****************************************************/
/* File: minic.cpp                                  */
/* The main function of MINIC compiler              */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/


#include"MINIC.h"
#include<QDebug>
#include"scan.h"


void mini_c(char* files)
{

    TreeNode * syntaxTree;
    /* source code file name */
    char pgm[120];
    strcpy(pgm,files) ;
    if (strchr (pgm, '.') == NULL)
    {
        strcat(pgm, ".tny");
    }
    source = fopen(pgm,"r");
    if (source==NULL)
    {
        fprintf(stderr,"File %s not found\n",pgm);
        exit(1);
    }
    /* send listing to screen */

    char outfile[]="out.txt";
    FILE* fileID = fopen(outfile,"w");
    listing = fileID;
    syntaxTree = parse();

    QString res;
    if (TraceParse) {
        fprintf(listing,"Syntax tree:\n\n");
        printTree(syntaxTree);

    }
    fclose(source);
    fclose(fileID);
}

