#include <include/Lexeme.h>
#include <iostream>
#include <memory>

const std::string code = R"(
func main() do
    exit "cavalo"
end
)";

int main() {
    const auto lexeme = std::make_unique<Lexeme::Lexeme>(code);
    const std::vector<Lexeme::Token> tokens = lexeme->tokenize();

    for (auto& t : tokens) {
        std::cout << static_cast<int>(t.type) << ": "
            << t.value << '\n';
    }

    return 0;
}