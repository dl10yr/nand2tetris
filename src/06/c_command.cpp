#include "./c_command.hpp"
#include <string>

using namespace nand2tetris::assembler;
using namespace std;

DestTable::DestTable() {
  codes["null"] = "000";
  codes["M"] = "000";
  codes["D"] = "000";
  codes["MD"] = "000";
  codes["A"] = "000";
  codes["AM"] = "000";
  codes["AD"] = "000";
  codes["AMD"] = "000";
}

CompTable::CompTable() {
  codes["0"] = "101010";
  codes["1"] = "111111";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
  codes["0"] = "101010";
}