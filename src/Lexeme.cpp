#include <stdexcept>
#include <include/Lexeme.h>

// funções-membro de ajuda

char Lexeme::Lexeme::advance() {
    if (!check()) return '\0';
    return src.at(current++);
}

bool Lexeme::Lexeme::check() const {
    if (src.size() > current) return true;
    return false;
}

char Lexeme::Lexeme::peek() const {
    if (!check()) return '\0';
    return src.at(current);
}

// Funções de "fazer tipos"

void Lexeme::Lexeme::makeDigit() {
    std::string digitBuffer {};
    while (check() && std::isdigit(peek())) {
        digitBuffer.push_back(advance());
    }
    tokens.emplace_back(digitBuffer, TokenType::DIGIT);
    digitBuffer.clear();
}

void Lexeme::Lexeme::makeIdentifier() {
    std::string identBuffer {};
    while (check() && std::isalpha(peek())) {
        identBuffer.push_back(advance());
    }
    if (identBuffer == "do") tokens.emplace_back("do", TokenType::KEYWORD);
    else if (identBuffer == "end") tokens.emplace_back("end", TokenType::KEYWORD);
    else if (identBuffer == "func") tokens.emplace_back("func", TokenType::KEYWORD);
    else if (identBuffer == "exit") tokens.emplace_back("exit", TokenType::KEYWORD);
    else tokens.emplace_back(identBuffer, TokenType::IDENT);
    identBuffer.clear();
}

void Lexeme::Lexeme::makeString() {
    std::string stringBuffer {};
    advance();
    while (check() && peek() != '"') {
        stringBuffer.push_back(advance());
    }
    if (peek() == '\0') throw std::runtime_error("EOF unexpected in string call.");
    advance();
    tokens.emplace_back(stringBuffer, TokenType::STRING);
    stringBuffer.clear();
}

// Funções-membro principais

std::vector<Lexeme::Token> Lexeme::Lexeme::tokenize() {
    while (check()) {
        switch (const char c = peek()) {
            case '(':
                tokens.emplace_back("(", TokenType::LPAREN); advance();
                break;
            case ')':
                tokens.emplace_back(")", TokenType::RPAREN); advance();
                break;
            case '+':
                tokens.emplace_back("+", TokenType::PLUS); advance();
                break;
            case '-':
                tokens.emplace_back("-", TokenType::MINUS); advance();
                break;
            case '/':
                tokens.emplace_back("/", TokenType::SLASH); advance();
                break;
            case '*':
                tokens.emplace_back("*", TokenType::STAR); advance();
                break;
            default:
                if (std::isspace(c)) advance();
                else if (std::isalpha(c)) makeIdentifier();
                else if (std::isdigit(c)) makeDigit();
                else if (c == '"') makeString();
        }
    }

    tokens.emplace_back("END OF FILE", TokenType::END_OF_FILE);
    return tokens;
}
