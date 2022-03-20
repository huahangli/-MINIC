/****************************************************/
/* File: util.cpp                                   */
/* The utils implementation for MINIC compiler      */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{
    switch (token)
    {
        case IF:
        case ELSE:
        case WHILE:
        case INT:
        case FLOAT:
        case VOID:
        case RETURN:
        case DO:
            fprintf(listing,
                    "reserved word: %s\n",tokenString);
            break;
        case ASSIGN: fprintf(listing,"=\n"); break;
        case MT: fprintf(listing,">\n"); break;
        case LT: fprintf(listing,"<\n"); break;
        case EQ: fprintf(listing,"==\n"); break;
        case LPAREN: fprintf(listing,"(\n"); break;
        case RPAREN: fprintf(listing,")\n"); break;
        case PLUS: fprintf(listing,"+\n"); break;
        case MINUS: fprintf(listing,"-\n"); break;
        case TIMES: fprintf(listing,"*\n"); break;
        case OVER: fprintf(listing,"/\n"); break;
        case MOD: fprintf(listing,"%%\n"); break;
        case ENDFILE: fprintf(listing,"EOF\n"); break;
        case NUM:
            fprintf(listing,
                    "NUM, val= %s\n",tokenString);
            break;
        case ID:
            fprintf(listing,
                    "ID, name= %s\n",tokenString);
            break;
        case ERROR:
            fprintf(listing,
                    "ERROR: %s\n",tokenString);
            break;
        default: /* should never happen */
            fprintf(listing,"Unknown token: %d\n",token);
    }
}


void printType(Type Type)
{
    switch(Type)
    {
        case Integer:fprintf(listing,"Type: int"); break;
        case Float:fprintf(listing,"Type: float"); break;
        case Void:fprintf(listing,"Type: void"); break;
        default: /* should never happen */
            fprintf(listing,"Unknown token: \n");
            break;
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==nullptr)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
    {
        for (i=0;i<MAXCHILDREN;i++) t->child[i] = nullptr;
        t->sibling = nullptr;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
        t->len = 0;
    }
    return t;
}


/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t==nullptr)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else {
        for (i=0;i<MAXCHILDREN;i++) t->child[i] = nullptr;
        t->sibling = nullptr;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
        t->len = 0;
    }
    return t;
}


/* Function newStmtNode creates a new Declare
 * node for syntax tree construction
 */
TreeNode * newDeclareNode(DeclareKind kind){
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == nullptr)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else {
        for (i=0;i<MAXCHILDREN;i++)
            t->child[i] = nullptr;
        t->sibling = nullptr;
        t->nodekind = DeclareK;
        t->kind.def = kind;
        t->lineno = lineno;
        t->len = 0;
    }
    return t;
}



/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
    char * t;
    if (s==NULL) return NULL;
    n = strlen(s)+1;
    t = (char*)malloc(n);
    if (t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else strcpy(t,s);
    return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=4
#define UNINDENT indentno-=4

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
    for (i=0;i<indentno;i++)
        fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
    INDENT;
    while (tree != nullptr) {
        printSpaces();
        if (tree->nodekind==StmtK)
        {
            switch (tree->kind.stmt)
            {
                case ConditionStmt:
                    fprintf(listing,"Condition\n");
                    break;
                case ReturnStmt:
                    fprintf(listing,"Return\n");
                    break;
                case WhileStmt:
                    fprintf(listing,"While\n");
                    break;
                case DoStmt:
                    fprintf(listing,"Do While\n");
                    break;
                case ExpStmt:
                    fprintf(listing,"Expression\n");
                    break;
                case CompundStmt:
                    fprintf(listing,"CompundStmt\n");
                    break;
                default:
                    fprintf(listing,"Unknown ExpNode kind\n");
                    break;
            }
        }
        else if (tree->nodekind==ExpK)
        { switch (tree->kind.exp) {
                case OpK:
                    fprintf(listing,"Op: ");
                    printToken(tree->attr.op,"\0");
                    break;
                case ConstK:
                    fprintf(listing,"Num: %d\n",tree->attr.val);
                    break;
                case IdK:
                    fprintf(listing,"Id: %s\n",tree->attr.name);
                    break;
                case TypeK:
                    fprintf(listing,"Type: %s\n",tree->attr.name);
                    break;
                case CallK:
                    fprintf(listing,"Call: \n");
                    break;
                case VariaK:
                    fprintf(listing,"Varia: \n");
                    break;
                case ArguK:
                    fprintf(listing,"Arguments: \n");
                    break;
                case AssignK:
                    fprintf(listing,"Assignments: \n");
                    break;
                default:
                    fprintf(listing,"Unknown ExpNode kind\n");
                    break;
            }
        }
        else if (tree->nodekind==DeclareK)
        {switch (tree->kind.def) {
            case VariableDef:
                fprintf(listing,"Variabel Declare: \n");
                break;
            case FuncDef:
                fprintf(listing,"Function Declare: \n");
                break;
            case Parameters:
                fprintf(listing,"Parameters Declare: \n");
                break;
            case CompoundK:
                fprintf(listing,"Compound statements:  \n");
                break;
            default:
                fprintf(listing,"Unknown ExpNode kind\n");
                break;
            }

        }
        else fprintf(listing,"Unknown node kind\n");
        for (i=0;i<MAXCHILDREN;i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}
