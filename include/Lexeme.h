#ifndef KEYLANG_LEXEME_H
#define KEYLANG_LEXEME_H

#include "Token.h"
#include <utility>
#include <vector>

namespace Lexeme {
    class Lexeme {
    private:
        size_t current = 0;
        std::string src;
        std::vector<Token> tokens;
        [[nodiscard]] bool check() const;
        [[nodiscard]] char peek() const;
        void makeDigit();
        void makeString();
        void makeIdentifier();
        char advance();
    public:
        explicit Lexeme(std::string  code) : src(std::move(code)) {}
        std::vector<Token> tokenize();
    };
}

#endif //KEYLANG_LEXEME_H