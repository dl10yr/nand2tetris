
#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE 1

#include<string>
#include<map>

using namespace std;

namespace nand2tetris {
namespace assembler {

class SymbolTable {
  private:
    map<string, int> _map;

  public:
    SymbolTable() {}
    void AddEntry(string s, int addr);
    bool Contains(string s);
    int GetAddress(string s);
};

}
}

#endif