#ifndef LEXEME_H
#define LEXEME_H

#include <fileinput.h>
#include <string>
#include <vector>

typedef enum {
    KL_String,
    KL_Digit,
    KL_Identifier,
    KL_Keyword,

    KL_LPAREN, KL_RPAREN,
    KL_PLUS, KL_MINUS,
    KL_STAR, KL_SLASH,
    KL_ASSIGN, KL_LKEY,
    KL_RKEY
} KL_TokenType;

typedef struct {
    std::string value;
    KL_TokenType type;
} KL_Token;

typedef struct {
    KL_FileInput* f;
    size_t position;
    std::vector<KL_Token> tokens;
    std::string buffer;
} KL_Lexeme;

std::unique_ptr<KL_Lexeme> KL_NewLexeme(KL_FileInput* fileInput);
void KL_AddTokenLexeme(KL_Lexeme* lexeme, std::string value, KL_TokenType type);
void KL_TokenizeLexeme(KL_Lexeme* lexeme);

#endif