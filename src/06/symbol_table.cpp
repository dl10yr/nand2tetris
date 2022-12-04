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

string SymbolTable::FormatAsBin(int address) {
  string bin(15, '0');
  int n = 14
  while (address) {
    bin[n--] = address & 1 ? '1' : '0';
    address >>= 1;
  }

  return bin;
}

string SymbolTable::GetAddress(string s) {
  if (!Contains(s)) {
    throw UndefinedSymbolException(string("Undefined symbol: ") + s);
  }
  return FormatAsBin(_map[s]);
}

}
}