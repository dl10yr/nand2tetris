#ifndef PARSER
#define PARSER 1

#include <fstream>
#include <string>
#include <exception>

using namespace std;

namespace nand2tetris {
namespace vm_translator {

enum CommandType {
  C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL
};

class Parser {
  private:
    string current_line_;
    ifstream& in_;

  public:
    Parser(ifstream& f): in_(f) {};
    ~Parser();
    bool HasMoreCommands();
    void Advance();
    CommandType CommandType();
    string Arg1();
    string Arg2();
};

}
}

#endif