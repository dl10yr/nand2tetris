#ifndef ERROR
#define ERROR 1

#include <string>

using namespace std;

namespace nand2tetris {
namespace assembler {

class UndefinedSymbolException {
  private:
    string _message;
  public:
    UndefinedSymbolException(string s): _message(s) {}

};

class SyntaxErrorException {
  private:
    int line_num_;
    int pos_;
    string code_;
    string expected_;
  public:
    SyntaxErrorException(int line_num, int pos, string code, string expected)
      : line_num_(line_num_), pos_(pos), code_(code), expected_(expected) {}

};

class IllegalCallException {
  private:
    string message_;

  public:
    IllegalCallException(string s): message_(s) {}

};

}
}

#endif