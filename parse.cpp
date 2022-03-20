/****************************************************/
/* File: parse.cpp                                  */
/* The parse implementation for MINIC compiler      */
/* Compiler principle Project                       */
/* Copyright:  Huahang li                           */
/* Created in  03.15.2022  version@_1               */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * declare_sequence();
static TreeNode * declare_stmt();
static TreeNode * type_indicator();
static TreeNode * parameters();
static TreeNode * parameter();
static TreeNode * compound_stmt();
static TreeNode * declare_stmt();
static TreeNode * stmt_sequence();
static TreeNode * statement();
static TreeNode * expression_stmt();
static TreeNode * return_stmt();
static TreeNode * condition_stmt();
static TreeNode * dowhile_stmt();
static TreeNode * while_stmt();
static TreeNode * return_stmt();
static TreeNode * exp();
static TreeNode * variabel();
static TreeNode * simple_exp(TreeNode*);
static TreeNode * addtive_exp(TreeNode*);
static TreeNode * term(TreeNode*);
static TreeNode * simple_exp();
static TreeNode * addtive_exp();
static TreeNode * term();
static TreeNode * factor();
static TreeNode * arguments();


static void syntaxError(char * message)
{
    fprintf(listing,"\n>>> ");
    fprintf(listing,"Syntax error at line %d: %s",lineno,message);
    Error = TRUE;
}

static void match(TokenType expected)
{
    if (token == expected) token = getToken();
    else {
        syntaxError("unexpected token -> ");
        printToken(token,tokenString);
        fprintf(listing,"      ");
    }
}

TreeNode * declare_sequence(){
    if(token==ENDFILE) return nullptr;
    TreeNode * t = declare_stmt();
    TreeNode * p = t;
    while ((token != ENDFILE)){
        TreeNode * q;
        q = declare_stmt();
        if (q!= nullptr){
            if (t == nullptr)
                t = p = q;
            else {
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}

TreeNode * declare_stmt(){
    TreeNode * p = nullptr;           /* the declare Node to return , lazy determind */
    TreeNode * type = type_indicator();  /* holds the current type */
    TreeNode * id = newExpNode(IdK);
    if ((type!=nullptr) && (id!=nullptr))
    {id->attr.name = copyString(tokenString); match(ID);}
    TreeNode * num;
    TreeNode * para;
    TreeNode * stmt;
    switch (token) {
        case SMC:
            p = newDeclareNode(VariableDef);
            p->child[0] = type;
            p->child[1] = id;
            match(SMC);
            break;
        case LBRACKETS:
            p = newDeclareNode(VariableDef);
            match(LBRACKETS);
            p->child[0] = type;
            p->child[1] = id;
            if(token==NUM)
            {   num = newExpNode(ConstK);
                num->attr.val = atoi(tokenString);
                p->child[2] = num;
            }

            match(NUM);
            match(RBRACKETS);
            match(SMC);
            break;
        case LPAREN:
            p = newDeclareNode(FuncDef);
            p->child[0] = type;
            p->child[1] = id;
            match(LPAREN);
            para = parameters();
            match(RPAREN);
            p->child[2] = para;
            stmt = compound_stmt();
            p->child[3] = stmt;
            break;

        default:
            fprintf(listing,"Syntax error at line %d: %s",lineno,tokenString);
            token = getToken();
            break;
    }
    return p;
}

TreeNode * parameters(){
    TreeNode *t = newDeclareNode(Parameters);
    if (token == VOID){
        t->child[0] = newExpNode(TypeK);
        t->child[0]->attr.name = copyString(tokenString);
        match(VOID);
    }
    else {
        t = parameter();
        TreeNode * p = t;
        while(token!=RPAREN){
            TreeNode * q;
            match(COMM);
            q = parameter();
            if(q!=nullptr){
                if (t==nullptr) t = p = q;
                else{
                    p->sibling = q;
                    p = q;
                }
            }
         }
   }
   return t;
}

TreeNode * parameter(){
    TreeNode * para = newDeclareNode(Parameters);
    TreeNode * t = type_indicator();
    TreeNode * id = newExpNode(IdK);
    id->attr.name = copyString(tokenString);
    match(ID);
    if (token == LBRACKETS){
        match(LBRACKETS);
        match(RBRACKETS);
    }

    para->child[0] = t;
    para->child[1] = id;
    return para;
}

TreeNode * compound_stmt(){
    TreeNode * comp_stmt = newDeclareNode(CompoundK);
    TreeNode * t = nullptr;
    TreeNode * p = nullptr;
    TreeNode * q = nullptr;
    TreeNode * stmt = nullptr;
    match(LBRACES);
    if ((token ==INT)||(token==FLOAT))
    {
        t = newDeclareNode(VariableDef);
        t->child[0] = type_indicator();
        t->child[1] = newExpNode(IdK);
        t->child[1]->attr.name = copyString(tokenString);
        match(ID);
        if (token == LBRACKETS)
        {
            match(LBRACKETS);
            t->child[2] = newExpNode(ConstK);
            t->child[2]->attr.val = atoi(tokenString);
            match(NUM);
            match(RBRACKETS);

        }
        match(SMC);
        p = t;

        while((token ==INT)||(token==FLOAT)){

            q = newDeclareNode(VariableDef);
            q->child[0] = type_indicator();
            q->child[1] = newExpNode(IdK);
            q->child[1]->attr.name = copyString(tokenString);
            match(ID);
            if (token == LBRACKETS)
            {
                match(LBRACKETS);
                q->child[2] = newExpNode(ConstK);
                q->child[2]->attr.val = atoi(tokenString);
                match(NUM);
                match(RBRACKETS);
            }
            match(SMC);
            p->sibling = q;
            p = q;

        }
    }

    if (token != RBRACES){
        stmt = stmt_sequence();
    }

    if (t!=nullptr&&stmt!=nullptr){
        comp_stmt->child[0] = t;
        comp_stmt->child[1] = stmt;
    } else if (t!=nullptr) {
        comp_stmt->child[0] = t;
    } else if (stmt!=nullptr) {
        comp_stmt->child[0] = stmt;
    }

    match(RBRACES);
    return comp_stmt;
}

TreeNode * type_indicator(){
    TreeNode * t = nullptr;
    switch (token) {
        case INT :
            t = newExpNode(TypeK);
            if ((t!= nullptr) && (token==INT))
                t->attr.name = copyString(tokenString);
            match(INT);
            break;
        case FLOAT :
            t = newExpNode(TypeK);
            if ((t!= nullptr) && (token==FLOAT))
                t->attr.name = copyString(tokenString);
            match(FLOAT);
            break;
        case VOID :
            t = newExpNode(TypeK);
            if ((t!= nullptr) && (token==VOID))
                t->attr.name = copyString(tokenString);
            match(VOID);
            break;
        default:
            syntaxError("unexpected token -> ");
            printToken(token,tokenString);
            token = getToken();
            break;
    }
    return t;
}

TreeNode * stmt_sequence(){
    TreeNode * t = statement();
    TreeNode * p = t;
    while ((token!=RBRACES) && (token!=ENDFILE)) {
        TreeNode * q;

        q = statement();
        if (q != nullptr) {
            if (t == nullptr) t = p = q;
            else /* now p cannot be nullptr either */
            {
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}

TreeNode * statement(){
    TreeNode * t = nullptr;
    switch (token) {
        case ID:
        case LPAREN:
        case SMC:
        case NUM:
            t = expression_stmt();break;
        case IF : t = condition_stmt(); break;
        case DO:  t = dowhile_stmt(); break;
        case WHILE : t = while_stmt(); break;
        case RETURN : t = return_stmt(); break;
        case LBRACES: t = compound_stmt(); break;
        default : syntaxError("unexpected token -> ");
            printToken(token,tokenString);
            token = getToken();
            break;
    } /* end case */
    return t;
}

TreeNode * expression_stmt(){
    TreeNode * t = newStmtNode(ExpStmt);
    if (token != SMC && t!= nullptr)
        t->child[0] = exp();
    match(SMC);
    return t;
}

TreeNode * condition_stmt()
{ TreeNode * t = newStmtNode(ConditionStmt);
    match(IF);
    match(LPAREN);
    if (t!=nullptr) t->child[0] = exp();
    match(RPAREN);
    if (t!=nullptr) t->child[1] = statement();
    if (token==ELSE) {
        match(ELSE);
        if (t!=nullptr) t->child[2] = statement();
    }
    return t;
}

TreeNode * while_stmt(){
    TreeNode * t = newStmtNode(WhileStmt);
    match(WHILE);
    match(LPAREN);
    if (t!= nullptr) t->child[0] = exp();
    match(RPAREN);
    if (t!= nullptr) t->child[1] = statement();
    return t;
}

TreeNode * dowhile_stmt(){
    TreeNode * t = newStmtNode(DoStmt);
    match(DO);
    if (t!=nullptr) t->child[0] = statement();
    match(WHILE);
    match(LPAREN);
    if (t!= nullptr) t->child[1] = exp();
    match(RPAREN);
    return t;
}

TreeNode * return_stmt(){
    TreeNode * t = newStmtNode(ReturnStmt);
    match(RETURN);
    if (token != SMC && t!=nullptr)
        t->child[0] = exp();
    match(SMC);
    return t;
}

TreeNode * exp()
{   TreeNode * t = nullptr;
    TreeNode * id = nullptr;
    switch(token)
    {
        case LPAREN: case NUM:
            t = simple_exp();
            break;
        case ID:
            id = variabel();
            if (token==ASSIGN){
                t =newExpNode(AssignK);
                t->child[0] = id;
                t->child[1] = newExpNode(OpK);
                t->child[1]->attr.op = token;
                match(ASSIGN);
                t->child[2] = exp();
            } else if(token==LPAREN){
                t = newExpNode(CallK);
                t->child[0] = newExpNode(IdK);
                t->child[0]->attr.name = copyString(id->child[0]->attr.name);
                match(LPAREN);
                t->child[1] = arguments();
                match(RPAREN);
                t = simple_exp(addtive_exp(term(t)));
            } else{
                t = id;
                t = simple_exp(addtive_exp(term(t)));
            }

            break;
        default: syntaxError("unexpected token -> ");
            printToken(token,tokenString);
            token = getToken();
            break;
    }


    return t;
}

TreeNode * simple_exp(treeNode * t){
    while ((token==LT)||(token==MT)||(token==LEQ)||(token==MEQ)||(token==EQ)||(token==UEQ)){
        TreeNode * p = newExpNode(OpK);
        if (p!= nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = addtive_exp();
        }
    }
    return t;
}

TreeNode * simple_exp(){
    TreeNode * t = addtive_exp();
    while ((token==LT)||(token==MT)||(token==LEQ)||(token==MEQ)||(token==EQ)||(token==UEQ)){
        TreeNode * p = newExpNode(OpK);
        if (p!= nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = addtive_exp();
        }
    }
    return t;
}

TreeNode * addtive_exp(TreeNode * t){
    while((token==PLUS)||(token==MINUS))
    {
        TreeNode * p =newExpNode(OpK);
        if(p!=nullptr){
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = term();
        }
    }
    return t;
}

TreeNode * addtive_exp(){
    TreeNode * t = term();
    while((token==PLUS)||(token==MINUS))
    {
        TreeNode * p =newExpNode(OpK);
        if(p!=nullptr){
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = term();
        }
    }
    return t;
}

TreeNode * term(TreeNode * t){
    while ((token==TIMES)||(token==OVER)||(token==MOD)){
        TreeNode * p = newExpNode(OpK);
        if (p!= nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = factor();
        }
    }
    return t;
}

TreeNode * term(void){
    TreeNode * t = factor();
    while ((token==TIMES)||(token==OVER)||(token==MOD)){
        TreeNode * p = newExpNode(OpK);
        if (p!= nullptr) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            p->child[1] = factor();
        }
    }
    return t;
}

TreeNode * factor(){
    TreeNode * t = nullptr;
    TreeNode * q = nullptr;
    switch (token) {
        case NUM :
            t = newExpNode(ConstK);
            if ((t!= nullptr) && (token==NUM))
                t->attr.val = atoi(tokenString);
            match(NUM);
            break;
        case ID :
            t = newExpNode(IdK);
            if ((t!= nullptr) && (token==ID))
                t->attr.name = copyString(tokenString);
            match(ID);
            if (token == LBRACKETS){
                match(LBRACKETS);
                q = newExpNode(VariaK);
                q->child[0] = t;
                q->child[1] = exp();
                match(RBRACKETS);
            } else if (token==LPAREN){
                match(LPAREN);
                q = newExpNode(CallK);
                q->child[0] = t;
                q->child[1] = arguments();
                match(RPAREN);
            } else{
                q = newExpNode(VariaK);
                q->child[0] = t;
            }
            t = q;
            break;   
        case LPAREN :
            match(LPAREN);
            t = exp();
            match(RPAREN);
            break;
        default:
            syntaxError("unexpected token -> ");
            printToken(token,tokenString);
            token = getToken();
            break;
    }
    return t;
}

TreeNode * variabel(){
    TreeNode * t = newExpNode(VariaK);
    if (token == ID){
        t->child[0] = newExpNode(IdK);
        t->child[0]->attr.name = copyString(tokenString);
        match(ID);
    } else{
        syntaxError("unexpected token -> ");
        printToken(token,tokenString);
        token = getToken();
    }

    if (token == LBRACKETS){
        match(LBRACKETS);
        t->child[1] = exp();
        match(RBRACKETS);
    }

    return t;
}

TreeNode * arguments(){
    if (token == RPAREN)
        return nullptr;
    TreeNode * t = exp();
    TreeNode * p =t;
    while (token!=RPAREN){
        TreeNode * q;
        match(COMM);
        q = exp();
        if (q!=nullptr){
            if(t==nullptr) t= p =q;
            else{
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}


/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode * parse(void)
{
    TreeNode * t;
    token = getToken();
    t = declare_sequence();
    if (token!=ENDFILE)
        syntaxError("Code ends before file\n");
    return t;
}
