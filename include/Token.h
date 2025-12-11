#ifndef KEYLANG_TOKEN_H
#define KEYLANG_TOKEN_H
#include <string>
#include <utility>

#include "Token.h"

namespace Lexeme {
    enum class TokenType {
        // Literals
        IDENT, DIGIT,
        STRING, KEYWORD,

        // Não literals
        LPAREN, RPAREN,
        PLUS, MINUS, SLASH, STAR,
        END_OF_FILE
    };

    struct Token {
        std::string value;
        TokenType type;
        Token(std::string v, const TokenType t) : value(std::move(v)), type(t) {}
    };
}

#endif //KEYLANG_TOKEN_H