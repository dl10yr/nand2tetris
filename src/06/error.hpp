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

}
}

#endif