#include "./symbol_table.hpp"
#include "./error.hpp"
#include <string>

using namespace std;

namespace nand2tetris {
namespace assembler {

void SymbolTable::AddEntry(string s, int addr) {
  _map[s] = addr;
}

bool SymbolTable::Contains(string s) {
  return _map.count(s) != 0;
}

int SymbolTable::GetAddress(string s) {
  if (!Contains(s)) {
    throw UndefinedSymbolException(string("Undefined symbol: ") + s);
  }
  return _map[s];
}

}
}