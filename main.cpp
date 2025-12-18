#include <iostream>
#include <fileinput.h>
#include <lexeme.h>
#include <ast.h>

void KL_PrintExpr(KL_Expr* expr, int indent = 0) {
    std::string ind(indent * 2, ' ');
    if (auto digit = dynamic_cast<KL_DigitNode*>(expr)) {
        std::printf("%sDigitNode: %ld\n", ind.c_str(), digit->value);
    }
    else if (auto var = dynamic_cast<KL_VarCallNode*>(expr)) {
        std::printf("%sVarCallNode: %s\n", ind.c_str(), var->var_name.c_str());
    }
    else if (auto binary = dynamic_cast<KL_BinaryNode*>(expr)) {
        std::printf("%sBinaryNode (op=%d):\n", ind.c_str(), binary->op);
        if (binary->left) KL_PrintExpr(binary->left.get(), indent + 1);
        if (binary->right) KL_PrintExpr(binary->right.get(), indent + 1);
    }
    else if (auto assign = dynamic_cast<KL_VarAssignStmt*>(expr)) {
        std::printf("%sVarAssignStmt: %s =\n", ind.c_str(), assign->var_name.c_str());
        if (assign->value) KL_PrintExpr(assign->value.get(), indent + 1);
    }
}

void KL_PrintStmt(KL_Stmt* stmt, int indent = 0) {
    std::string ind(indent * 2, ' ');
    if (auto var_assign = dynamic_cast<KL_VarAssignStmt*>(stmt)) {
        std::printf("%sVarAssignStmt: %s =\n", ind.c_str(), var_assign->var_name.c_str());
        if (var_assign->value) KL_PrintExpr(var_assign->value.get(), indent + 1);
    }
    else if (auto expr_stmt = dynamic_cast<KL_ExprStmt*>(stmt)) {
        std::printf("%sExprStmt:\n", ind.c_str());
        if (expr_stmt->expr) KL_PrintExpr(expr_stmt->expr.get(), indent + 1);
    }
}

int main() {
    auto f = KL_NewFileInput("teste.kl");
    KL_InitFileInput(f.get());
    auto lexeme = KL_NewLexeme(f.get());
    KL_TokenizeLexeme(lexeme.get());
    auto ast = KL_NewAST(lexeme->tokens);
    KL_AST_ParseProgram(ast.get());

    std::printf("=== TOKENS GERADOS ===\n");
    for(auto& t : lexeme->tokens) {
        std::printf("%d: %s\n", t.type, t.value.c_str());
    }
    
    std::printf("=== STATEMENTS ===\n");
    for (auto& stmt : ast->statements) {
        KL_PrintStmt(stmt.get());
    }

    return 0;
}