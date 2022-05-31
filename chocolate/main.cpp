#include <iostream>
#include <any>

enum class SyntaxKind {
    Number,
    WhitespaceToken,
    PlusToken,
    MinusToken,
    StarToken,
    SlashToken,
    OpenParenthesisToken,
    CloseParenthesisToken,
    BadToken,
    EndofFileToken
};

union object {
    int a;
};

class SyntaxToken {
    SyntaxKind kind;
    int position;
    std::string text;
    std::any value;

public:
    SyntaxToken(SyntaxKind kind, int position, std::string text, std::any value) {
        kind = kind;
        position = position;
        text = text;
        value = value;
    }

    SyntaxKind Kind() {
        return kind;
    }

    int Position() {
        return position;
    }

    std::string Text() {
        return text;
    }

    std::any Value() {
        return value;
    }
};

class Lexer {
    std::string _text;
    int _position;

    char Current() {
        if (_position >= _text.length())
            return '\0';

        return _text[_position];
    }

    void Next() {
        _position++;
    }

public:
    Lexer(std::string text) {
        this->_text = text;
    }

    SyntaxToken NextToken() {
        if (_position >= _text.length())
            return SyntaxToken(SyntaxKind::EndofFileToken, _position, "\0", NULL);

        if (std::isdigit(Current())) {
            int start = _position;

            while (std::isdigit(Current()))
                Next();

            int length = _position - start;
            std::string text = _text.substr(start, length);
            
            return SyntaxToken(SyntaxKind::Number, start, text, std::stoi(text));
        }

        if (std::isspace(Current())) {
            int start = _position;

            while (std::isspace(Current()))
                Next();

            int length = _position - start;
            std::string text = _text.substr(start, length);
            return SyntaxToken(SyntaxKind::WhitespaceToken, start, text, std::stoi(text));
        }

        if (Current() == '+')
            return SyntaxToken(SyntaxKind::PlusToken, _position++, "+", NULL);
        else if (Current() == '-')
            return SyntaxToken(SyntaxKind::MinusToken, _position++, "-", NULL);
        else if (Current() == '*')
            return SyntaxToken(SyntaxKind::StarToken, _position++, "*", NULL);
        else if (Current() == '/')
            return SyntaxToken(SyntaxKind::SlashToken, _position++, "/", NULL);
        else if (Current() == '(')
            return SyntaxToken(SyntaxKind::OpenParenthesisToken, _position++, "(", NULL);
        else if (Current() == ')')
            return SyntaxToken(SyntaxKind::CloseParenthesisToken, _position++, ")", NULL);
        
        return SyntaxToken(SyntaxKind::BadToken, _position++, _text.substr(_position - 1, 1), NULL);
    }
};

int main() {
    while (true) {
        std::cout << "> ";

        std::string line;
        std::getline(std::cin, line);

        if (line.empty() || line == "exit")
            break;

        Lexer* lexer = new Lexer(line);
        while (true) {
            SyntaxToken token = lexer->NextToken();
            if (token.Kind() == SyntaxKind::EndofFileToken)
                break;

            printf("%s: '%s'", token.Kind(), token.Text());
            if (token.Value() != NULL)
                printf(" %s", token.Value());
            
            printf("\n");
        }
    }
}
