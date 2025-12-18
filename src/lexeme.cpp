#include <lexeme.h>

std::unique_ptr<KL_Lexeme> KL_NewLexeme(KL_FileInput* fileInput) {
    auto lexeme = std::make_unique<KL_Lexeme>();

    lexeme->f = fileInput;

    return lexeme;
}

void KL_AddTokenLexeme(KL_Lexeme* lexeme, std::string value, KL_TokenType type) {
    lexeme->tokens.push_back({ .value = value, .type = type });
}

void KL_TokenizeLexeme(KL_Lexeme* lexeme) {
    std::string input = lexeme->f->content;
    size_t len = lexeme->f->content.length();
    size_t i = lexeme->position;
    char c;

    while (i < len) {
        c = input[i];

        if (isspace(c)) {
            i++;
            continue;
        }
        else if (c == '-') KL_AddTokenLexeme(lexeme, "-", KL_MINUS);
        else if (c == '+') KL_AddTokenLexeme(lexeme, "+", KL_PLUS);
        else if (c == '/') KL_AddTokenLexeme(lexeme, "/", KL_SLASH);
        else if (c == '*') KL_AddTokenLexeme(lexeme, "*", KL_STAR);
        else if (c == '(') KL_AddTokenLexeme(lexeme, "(", KL_LPAREN);
        else if (c == ')') KL_AddTokenLexeme(lexeme, ")", KL_RPAREN);
        else if (c == '=') KL_AddTokenLexeme(lexeme, "=", KL_ASSIGN);
        else if (c == '{') KL_AddTokenLexeme(lexeme, "{", KL_LKEY);
        else if (c == '}') KL_AddTokenLexeme(lexeme, "}", KL_RKEY);
        else if (isdigit(c)) {
            while (i < len && std::isdigit(input[i])) {
                lexeme->buffer.push_back(input[i]);
                i++;
            }
            KL_AddTokenLexeme(lexeme, lexeme->buffer, KL_Digit);
            lexeme->buffer.clear();
            continue;
        }
        else if (isalpha(c)) {
            while (i < len && std::isalpha(input[i])) {
                lexeme->buffer.push_back(input[i]);
                i++;
            }

            if (lexeme->buffer == "exit") KL_AddTokenLexeme(lexeme, "exit", KL_Keyword);
            else if (lexeme->buffer == "while") KL_AddTokenLexeme(lexeme, "while", KL_Keyword);
            else if (lexeme->buffer == "int8") KL_AddTokenLexeme(lexeme, "int8", KL_Keyword);
            else KL_AddTokenLexeme(lexeme, lexeme->buffer, KL_Identifier);
            lexeme->buffer.clear();
            continue;
        }
        i++;
    }

    lexeme->position = i;
}