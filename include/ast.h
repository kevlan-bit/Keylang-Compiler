#ifndef KL_AST_H
#define KL_AST_H

#include <cstdint>
#include <string>
#include <vector>
#include <lexeme.h>
#include <memory>

class KL_Expr {
public:
    virtual ~KL_Expr() = default;
};

class KL_Stmt {
public:
    virtual ~KL_Stmt() = default;
};

// Expressions
class KL_DigitNode : public KL_Expr {
public:
    int64_t value;
};
class KL_StringNode : public KL_Expr {
public:
    std::string value;
};
class KL_VarCallNode : public KL_Expr {
public:
    std::string var_name;
};
class KL_BinaryNode : public KL_Expr {
public:
    KL_TokenType op;
    std::unique_ptr<KL_Expr> left, right;
    KL_BinaryNode(KL_TokenType o, std::unique_ptr<KL_Expr> l, std::unique_ptr<KL_Expr> r) : op(o), left(std::move(l)), right(std::move(r)) {}
};
class KL_UnaryNode : public KL_Expr {
public:
    KL_TokenType op;
    std::unique_ptr<KL_Expr> expr;
    KL_UnaryNode(KL_TokenType o, std::unique_ptr<KL_Expr> e) : op(o), expr(std::move(e)) {}
};
class KL_VarAssignStmt : public KL_Stmt {
public:
    std::string var_name;
    std::unique_ptr<KL_Expr> value;
    KL_VarAssignStmt(std::string name, std::unique_ptr<KL_Expr> val) : var_name(name), value(std::move(val)) {}
};
class KL_ExprStmt : public KL_Stmt {
public:
    std::unique_ptr<KL_Expr> expr;
    KL_ExprStmt(std::unique_ptr<KL_Expr> e) : expr(std::move(e)) {}
};

struct KL_AST {
public:
    std::vector<KL_Token> tokens;
    std::vector<std::unique_ptr<KL_Stmt>> statements;
    size_t current;
};

std::unique_ptr<KL_AST> KL_NewAST(std::vector<KL_Token> tokens);
std::unique_ptr<KL_Expr> KL_AST_ParseUnary(KL_AST* ast);
std::unique_ptr<KL_Expr> KL_AST_ParseTerm(KL_AST* ast);
std::unique_ptr<KL_Expr> KL_AST_ParseFactor(KL_AST* ast);
std::unique_ptr<KL_Expr> KL_AST_ParseExpression(KL_AST* ast);
std::unique_ptr<KL_Stmt> KL_AST_ParseStatement(KL_AST* ast);
void KL_AST_ParseProgram(KL_AST* ast);
KL_Token KL_AST_Advance(KL_AST* ast);
KL_Token KL_AST_Previous(KL_AST* ast);
bool KL_AST_Match(KL_AST* ast, KL_TokenType type);

#endif