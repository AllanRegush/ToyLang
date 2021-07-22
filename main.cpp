#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>

struct Token;


// AST Expermentation
class ASTNode {
public:
    virtual void accept() = 0;
};

class Binary : ASTNode {
public:
    Binary(std::unique_ptr<ASTNode> left, std::unique_ptr<Token> op, std::unique_ptr<ASTNode> right):
        m_left(std::move(left)),
        m_right(std::move(right)),
        m_op(std::move(op)){}
private:
    std::unique_ptr<ASTNode> m_left;
    std::unique_ptr<ASTNode> m_right;
    std::unique_ptr<Token> m_op;
};

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER,
    GREATER_EQUAL, LESS, LESS_EQUAL,

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
    case TokenType::PLUS: out << "PLUS"; break;
    case TokenType::SEMICOLON: out << "SEMICOLON"; break;
    case TokenType::SLASH: out << "SLASH"; break;
    case TokenType::STAR: out << "STAR"; break;
    case TokenType::BANG: out << "BANG"; break;
    case TokenType::BANG_EQUAL: out << "BANG_EQUAL"; break;
    case TokenType::EQUAL: out << "EQUAL"; break;
    case TokenType::EQUAL_EQUAL: out << "EQUAL_EQUAL"; break;
    case TokenType::GREATER: out << "GREATER"; break;
    case TokenType::GREATER_EQUAL: out << "GREATER_EQUAL"; break;
    case TokenType::LESS: out << "LESS"; break;
    case TokenType::LESS_EQUAL: out << "LESS_EQUAL"; break;
    case TokenType::EndOfFile: out << "EOF"; break;
    }
    return out;
}

struct Token {
    TokenType type;
    size_t line;
    std::string lexeme;
    Token(TokenType type, size_t line, std::string lexeme):
        type(type), line(line), lexeme(lexeme) {}
};

class Lexer {
private:
    std::string m_src;
    std::vector<std::unique_ptr<Token>> m_tokens;
    size_t m_start {0};
    size_t m_current {0};
    size_t m_line {1};
    bool is_at_end()
    {
        return m_current >= m_src.length();
    }
    char advance()
    {
        return m_src.at(m_current++);
    }
    void add_token(const TokenType type, const std::string& text)
    {
        m_tokens.emplace_back(std::make_unique<Token>(type, m_line, std::move(text)));
    }
    void add_token(const TokenType type)
    {
        const std::string text = m_src.substr(m_start, m_current);
        m_tokens.emplace_back(std::make_unique<Token>(type, m_line, std::move(text)));
    }
    void scan_token()
    {
        char c = advance();
        switch (c) {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::STAR); break;
        case '!': {
            const TokenType type = match('=') ? TokenType::BANG_EQUAL : TokenType::BANG;
            add_token(type);
        } break;
        case '/': {
            if (match('/')) {
                while(peek() != '\n' && !is_at_end())
                    advance();
            }
            else if (match('*')) {
                while(peek() != '*' && !match('/')) {
                    if(peek() == '\n') m_line++;
                    advance();
                }
                m_current += 2;
            }
            else {
                add_token(TokenType::SLASH);
            }
        } break;
        case '"': {
            string();
        } break;
        default:
            std::cout << "error on line: " << m_line << " Unexpected char\n";
            break;
        }
    }
    bool match(char expect)
    {
        if (is_at_end())
            return false;

        if (m_src.at(m_current) != expect)
            return false;

        m_current++;
        return true;
    }
    char peek()
    {
        if (is_at_end())
            return '\0';

        return m_src.at(m_current);
    }
    void string()
    {
        // TODO (Allan): get string handling working.
        std::cout << "Strings do not work yet!! \n";
        assert(false);
    }
public:
    Lexer(const std::string src): m_src(src) {}
    std::vector<std::unique_ptr<Token>> scan_tokens()
    {
        while (!is_at_end()) {
            m_start = m_current;
            scan_token();
        }
        m_tokens.emplace_back(std::make_unique<Token>(TokenType::EndOfFile, m_line, ""));
        return std::move(m_tokens);
    }
};

static void run(const std::string& src)
{
    Lexer lexer {src};
    const auto tokens = lexer.scan_tokens();

    for (const auto& token: tokens)
        std::cout << "Token Type: " << token->type << " Line: " << token->line << '\n';

}

static void run_repl()
{
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.length() == 0)
            break;

        run(line);
    }
}

int main()
{
   run_repl();

   return 0;
}
