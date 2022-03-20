/****************************************************/
/* File: scan.cpp                                   */
/* The scanner implementation for MINIC compiler    */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/


#include "globals.h"
#include "util.h"
#include "scan.h"

/* states in scanner DFA */
typedef enum{
    START,INASSIGN,INNUM,INID,DONE,INCOMMENT
}StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

char lineBuf[BUFLEN]; /* holds the current line */
int linepos = 0; /* current position in LineBuf */
int bufsize = 0; /* current size of buffer string */
int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void){
    if (linepos >= bufsize) {
        lineno++;
        if (fgets(lineBuf,BUFLEN-1,source)){
            if (EchoSource) fprintf(listing, "%4d: %s", lineno, lineBuf);
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        }
        else { EOF_flag = TRUE; return EOF;}
    }
    else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* lookup table of reserved words */
static struct
{
    char* str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"if",IF},{"else",ELSE},{"while",WHILE},{"do",DO},
    {"int",INT},{"float",FLOAT},{"void",VOID},{"return",RETURN}
};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup (char * s)
{
    int i;
    for (i=0;i<MAXRESERVED;i++)
        if (!strcmp(s,reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
    int tokenStringIndex = 0;
    /* holds current token to be returned */
    TokenType currentToken;
    /* current state - always begins at START */
    StateType state = START;
    /* flag to indicate save to tokenString */
    int save;
    while (state != DONE)
    {
        int c = getNextChar();
        save = TRUE;
        switch (state)
        {
            case START:
                if (isdigit(c))
                    state = INNUM;
                else if (isalpha(c))
                    state = INID;
                else if ((c == ' ') || (c == '\t') || (c == '\n'))
                    save = FALSE;
                else if (c == '/'){
                    int nextchar = getNextChar();
                    if (nextchar == '*') {state = INCOMMENT;save = FALSE;}
                    else {ungetNextChar(); currentToken = OVER; state = DONE;}
                }
                else if (c== '<'){
                    int nextchar = getNextChar();
                    if (nextchar == '='){currentToken = LEQ; state = DONE;}
                    else {ungetNextChar(); currentToken = LT; state = DONE;}
                }
                else if (c== '>') {
                    int nextchar = getNextChar();
                    if (nextchar == '='){currentToken = MEQ; state = DONE;}
                    else {ungetNextChar(); currentToken = MT; state = DONE;}
                }
                else if (c== '='){
                    int nextchar = getNextChar();
                    if (nextchar == '='){currentToken = EQ;state = DONE;}
                    else {ungetNextChar(); currentToken = ASSIGN; state = DONE;}
                }
                else if (c== '!'){
                    int nextchar = getNextChar();
                    if (nextchar == '='){currentToken = UEQ; state = DONE;}
                }
                else {
                    state = DONE;
                    switch (c)
                    {
                        case EOF:
                            save = FALSE;
                            currentToken = ENDFILE;
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case '(':
                            currentToken = LPAREN;
                            break;
                        case ')':
                            currentToken = RPAREN;
                            break;
                        case '[':
                            currentToken = LBRACKETS;
                            break;
                        case ']':
                            currentToken = RBRACKETS;
                            break;
                        case '{':
                            currentToken = LBRACES;
                            break;
                        case '}':
                            currentToken = RBRACES;
                            break;
                        case ';':
                            currentToken = SMC;
                            break;
                        case '%':
                            currentToken = MOD;
                            break;
                        case ',':
                            currentToken = COMM;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;

            case INCOMMENT:
                save = FALSE;
                if (c == EOF){
                    state = DONE;
                    currentToken = ENDFILE;
                }
                else if (c == '*'){
                    int nextchar = getNextChar();
                    if (nextchar == '/') state = START;
                    else ungetNextChar();
                }
                break;

            case INNUM:
                if (!isdigit(c))
                { /* backup in the input */
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = NUM;
                }
                break;

            case INID:
                if (!isalpha(c))
                { /* backup in the input */
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = ID;
                }
                break;

            case DONE:
            default: /* should never happen */
                fprintf(listing,"Scanner Bug: state= %d\n",state);
                state = DONE;
                currentToken = ERROR;
                break;
        }

        if ((save) && (tokenStringIndex <= MAXTOKENLEN))
            tokenString[tokenStringIndex++] = (char) c;
        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (TraceScan) {
        fprintf(listing,"\t%d: ",lineno);
        printToken(currentToken,tokenString);
    }
    return currentToken;
} /* end getToken */

