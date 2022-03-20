/****************************************************/
/* File: main.cpp                                   */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/

#include "mainwindow.h"
#include "MINIC.h"
#include <QApplication>


FILE * source;
FILE * listing;
FILE * code;

int lineno=0;
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;

#define BUFLEN 256


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
