#include <ast.h>
#include <cassert>

std::unique_ptr<KL_AST> KL_NewAST(std::vector<KL_Token> tokens) {
    auto ast = std::make_unique<KL_AST>();
    ast->tokens = tokens;
    ast->current = 0;
    return ast;
}

std::unique_ptr<KL_Expr> KL_AST_ParseUnary(KL_AST* ast) {
    if (KL_AST_Match(ast, KL_MINUS)) {
        KL_TokenType op = KL_AST_Previous(ast).type;
        auto right = KL_AST_ParseUnary(ast);
        if (!right) return nullptr;
        return std::make_unique<KL_UnaryNode>(op, std::move(right));
    }
    return KL_AST_ParseFactor(ast);
}

std::unique_ptr<KL_Expr> KL_AST_ParseTerm(KL_AST* ast) {
    auto left = KL_AST_ParseUnary(ast);
    while (KL_AST_Match(ast, KL_STAR) || KL_AST_Match(ast, KL_SLASH)) {
        KL_TokenType op = KL_AST_Previous(ast).type;
        auto right = KL_AST_ParseUnary(ast);
        if (!right) return nullptr;
        left = std::make_unique<KL_BinaryNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<KL_Expr> KL_AST_ParseFactor(KL_AST* ast) {
    if (KL_AST_Match(ast, KL_Digit)) {
        auto e = std::make_unique<KL_DigitNode>();
        e->value = std::stoll(KL_AST_Previous(ast).value);
        return e;
    }
    else if (KL_AST_Match(ast, KL_Identifier)) {
        auto e = std::make_unique<KL_VarCallNode>();
        e->var_name = KL_AST_Previous(ast).value;
        return e;
    }
    else if (KL_AST_Match(ast, KL_LPAREN)) {
        auto expr = KL_AST_ParseExpression(ast);
        if (!KL_AST_Match(ast, KL_RPAREN)) return nullptr;
        return expr;
    }
    else return nullptr;
}

std::unique_ptr<KL_Expr> KL_AST_ParseExpression(KL_AST* ast) {
    auto left = KL_AST_ParseTerm(ast);
    while (KL_AST_Match(ast, KL_PLUS) || KL_AST_Match(ast, KL_MINUS)) {
        KL_TokenType op = KL_AST_Previous(ast).type;
        auto right = KL_AST_ParseTerm(ast);
        if (!right) return nullptr;
        left = std::make_unique<KL_BinaryNode>(op, std::move(left), std::move(right));
    }
    return left;
}

KL_Token KL_AST_Advance(KL_AST* ast) {
    if (ast->current < ast->tokens.size()) {
        return ast->tokens[ast->current++];
    }
    return ast->tokens.back();
}

bool KL_AST_Match(KL_AST* ast, KL_TokenType type) {
    if (ast->current >= ast->tokens.size()) return false;
    if (ast->tokens[ast->current].type == type) {
        KL_AST_Advance(ast);
        return true;
    }
    return false;
}

KL_Token KL_AST_Previous(KL_AST* ast) {
    assert(ast->current > 0);
    return ast->tokens[ast->current-1];
}

std::unique_ptr<KL_Stmt> KL_AST_ParseStatement(KL_AST* ast) {
    if (KL_AST_Match(ast, KL_Identifier)) {
        std::string name = KL_AST_Previous(ast).value;

        if (ast->current + 1 >= ast->tokens.size()) return nullptr;
        if (KL_AST_Match(ast, KL_ASSIGN)) {
            ast->current--;
            return nullptr;
        }
        auto expr = KL_AST_ParseExpression(ast);
        if (expr) return std::make_unique<KL_VarAssignStmt>(name, std::move(expr));
        return nullptr;
    }
    
    auto expr = KL_AST_ParseExpression(ast);
    if (expr) return std::make_unique<KL_ExprStmt>(std::move(expr));
    return nullptr;
}

void KL_AST_ParseProgram(KL_AST* ast) {
    while (ast->current < ast->tokens.size()) {
        auto stmt = KL_AST_ParseStatement(ast);
        if (stmt) {
            ast->statements.push_back(std::move(stmt));
        } else break;
    }
}