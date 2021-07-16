#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, /* PLUS, SEMICOLON, SLASH, STAR,
    */
    EndOfFile
};

std::ostream& operator<< (std::ostream& out, const TokenType& type)
{
    switch (type) {
    case TokenType::LEFT_PAREN: out << "LEFT_PAREN"; break;
    case TokenType::RIGHT_PAREN: out << "RIGHT_PAREN"; break;
    case TokenType::LEFT_BRACE: out << "LEFT_BRACE"; break;
    case TokenType::RIGHT_BRACE: out << "RIGHT_BRACE"; break;
    case TokenType::COMMA: out << "COMMA"; break;
    case TokenType::DOT: out << "DOT"; break;
    case TokenType::MINUS: out << "MINUS"; break;
    case TokenType::EndOfFile: out << "EOF"; break;
    }
    return out;
}

struct Token {
    TokenType type;
    int line;
    std::string lexeme;
    Token(TokenType type, int line, std::string lexeme):
        type(type), line(line), lexeme(lexeme) {}
};

class Lexer {
private:
    std::string m_src;
    std::vector<std::unique_ptr<Token>> m_tokens;
    size_t m_start = 0;
    size_t m_current = 0;
    size_t m_line = 1;
    bool isAtEnd()
    {
        return m_current >= m_src.length();
    }
    char advance()
    {
        return m_src.at(m_current++);
    }
    void addToken(const TokenType type)
    {
        std::string text = m_src.substr(m_start, m_current);
        m_tokens.emplace_back(std::make_unique<Token>(type, m_line, text));
    }
    void scanToken()
    {
        char c = advance();
        switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        default:
            std::cout << "error on line: " << m_line << " Unexpected char\n";
            break;
        }
    }
public:
    Lexer(const std::string src): m_src(src) {}
    std::vector<std::unique_ptr<Token>> scanTokens()
    {
        while (!isAtEnd()) {
            m_start = m_current;
            scanToken();
        }
        m_tokens.emplace_back(std::make_unique<Token>(TokenType::EndOfFile, m_line, ""));
        return std::move(m_tokens);
    }
};

static void run(const std::string& src)
{
    Lexer lexer {src};
    const auto tokens = lexer.scanTokens();

    for (const auto& token: tokens) {
        std::cout << "Token Type: " << token->type << " Line: " << token->line << '\n';
    }
}

static void runRepl()
{
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.length() == 0) {
            break;
        }
        run(line);
    }
}

int main()
{
   runRepl();

   return 0;
}
