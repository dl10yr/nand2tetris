#ifndef JACK_TOKENIZER
#define JACK_TOKENIZER 1

#include <fstream>
#include <string>
#include <exception>

namespace nand2tetris {
namespace jack_tokenizer {

enum JackTokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };

enum JackKeyWord { CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, NULL, THIS }

class JackTokenizer {
  private:
    ifstream in_;
    string current_token_;

    void Advance();

  public:
    JackTokenizer(string input_file): {};
    ~JackTokenizer();

    bool HasMoreTokens();
    JackTokenType TokenType();
    JackKeyWord KeyWord();
    string Symbol();
    string Identifier();
    int IntVal();
    string StringVal();

    string GetCurrentToken();
    void UpdateCurrentToken();
};

}
}

#endif