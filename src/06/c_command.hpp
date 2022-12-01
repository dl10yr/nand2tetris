
#ifndef C_Commands
#define C_Commands 1

#include <string>
#include <map>

using namespace std;

namespace nand2tetris {
namespace assembler {

class DestTable {
  private:
    map<string, string> codes;

  public:
    DestTable();
    string GetBinaryCode(string m);
};

class JumpTable {
  private:
    map<string, string> codes;
  
  public:
    JumpTable();
    string GetBinaryCode(string m);
};

class CompTable {
  private:
    map<string, string> codes;
  
  public:
    CompTable();
    string GetBinaryCode(string m);
};


}
}

#endif