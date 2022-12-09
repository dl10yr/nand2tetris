#include "parser.hpp"
#include "code_writer.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace nand2tetris::vm_translator2;

int main(int argc, char** argv) {
  string file_name;

  if (argc != 2) {
    cout << "error expected one argument(file name)\n";
    cout << "enter the filename: ";
    cin >> file_name;
  } else {
    file_name = argv[1];
  }

  int dot = filename.find();
  file_name = file_name.substr(0, dot);
  Parser parser(file_name);
  CodeWriter writer;
  writer.SetFileName(file_name);

  while(parser.HasMoreCommands()) {
    parser.Advance();
    if (parser.CommandType() == C_ARITHMETIC) {
      writer.WriteArithmetic(parser.Arg1());
    } else if (parser.CommandType() == C_POP) {
      writer.WritePushPop(C_POP, parser.Arg1(), parser.Arg2());
    } else if (parser.CommandType() == C_PUSH) {
      writer.WritePushPop(C_PUSH, parser.Arg1(), parser.Arg2());
    }
  }
}