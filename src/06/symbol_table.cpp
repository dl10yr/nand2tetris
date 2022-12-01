#include "./symbol_table.hpp"
#include <string>

using namespace std;

namespace nand2tetris {
namespace assembler {

void SymbolTable::AddEntry(string s, int addr) {
  _map[s] = addr;
}

}
}