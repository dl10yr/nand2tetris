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
  codes["-1"] = "111010";
  codes["D"] = "001100";
  codes["A"] = "110000";
  codes["M"] = "110000";
  codes["!D"] = "001101";
  codes["!A"] = "110001";
  codes["!M"] = "110001";
  codes["-D"] = "001111";
  codes["-A"] = "110011";
  codes["-M"] = "110011";
  codes["D+1"] = "011111";
  codes["A+1"] = "110111";
  codes["M+1"] = "110111";
  codes["D-1"] = "001110";
  codes["A-1"] = "110010";
  codes["M-1"] = "110010";
  codes["D+A"] = "000010";
  codes["D+M"] = "000010";
  codes["D-A"] = "010011";
  codes["D-M"] = "010011";
  codes["A-D"] = "000111";
  codes["M-D"] = "000111";
  codes["D&A"] = "000000";
  codes["D&M"] = "000000";
  codes["D|A"] = "010101";
  codes["D|M"] = "010101";
}

JumpTable::JumpTable() {
  codes["null"] = "000";
  codes["JGT"] = "001";
  codes["JEQ"] = "010";
  codes["JGE"] = "011";
  codes["JLT"] = "100";
  codes["JNE"] = "101";
  codes["JLE"] = "110";
  codes["JMP"] = "111";
}