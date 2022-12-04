#include "./parser.hpp"
#include "./c_command.hpp"
#include "./error.hpp"
#include "./symbol_table.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace nand2tetris::assembler;
using namespace std;

int main(int argc, char** argv) {
  string file_name;

  if (argc != 2) {
    cout << "error expected one argument (file name)\n";
    cout << "enter the filename:";
    cin >> file_name;
  } else {
    file_name = argv[1];
  }

  ifstream file(file_name.c_str());
  if (!file) {
    cout << "cant find this file\n";
    return 1;
  }

  DestTable dest_table;
  CompTable comp_table;
  JumpTable jump_table;

  Parser parser(file, dest_table, comp_table, jump_table);
  SymbolTable symbol_table;

  int instruction_count = 0;
  // romアドレスとシンボルの対応づけ
  try {
    while (parser.HasMoreCommands()) {
      parser.Advance();
      switch (parser.CommandType()) {
        case L_COMMAND:
          symbol_table.AddEntry(parser.Symbol(), instruction_count);
          break;
        default:
          instruction_count++;
      }
    }
  }
  catch(IllegalCallException& e) {
    cout << "illeagal\n";
    return 1;
  }
  catch(...) {
    cout << "unknown error\n";
    return 1;
  }

  file.close();
  file.open(file_name.c_str());
  Parser parser2(file, dest_table, comp_table, jump_table);
  ofstream output_file("test.hack");
  string symbol;
  int reg;
  int RAM_running_address = 16;


  symbol_table.AddEntry("SP", 0);
  symbol_table.AddEntry("LCL", 1);
  symbol_table.AddEntry("ARG", 2);
  symbol_table.AddEntry("THIS", 3);
  symbol_table.AddEntry("THAT", 4);
  symbol_table.AddEntry("RO", 0);
  symbol_table.AddEntry("R1", 1);
  symbol_table.AddEntry("R2", 2);
  symbol_table.AddEntry("R3", 3);
  symbol_table.AddEntry("R4", 4);
  symbol_table.AddEntry("R5", 5);
  symbol_table.AddEntry("R6", 6);
  symbol_table.AddEntry("R7", 7);
  symbol_table.AddEntry("R8", 8);
  symbol_table.AddEntry("R9", 9);
  symbol_table.AddEntry("R10", 10);
  symbol_table.AddEntry("R11", 11);
  symbol_table.AddEntry("R12", 12);
  symbol_table.AddEntry("R13", 13);
  symbol_table.AddEntry("R14", 14);
  symbol_table.AddEntry("R15", 15);
  symbol_table.AddEntry("SCREEN", 16384);
  symbol_table.AddEntry("KBD", 24576);

  try {
    while(parser2.HasMoreCommands()) {
      parser2.Advance();
      switch(parser2.CommandType()) {
        case A_COMMAND:
          symbol = parser2.Symbol();
          if (!isdigit(symbol[0])) {
            if (symbol_table.Contains(symbol)) {
              output_file << "0"
                  << symbol_table.GetAddress(symbol);
                  << "\n";
            } else {
              symbol_table.AddEntry(sym, RAM_running_address);
              out << "0"
                  << symbol_table.FormatAsBin(RAM_running_address++)
                  << 
            }
          } else {
            int constatnt = 0;
            for(int i = 0; i < symbol.size(); i++) {
              constant 
            }
          }

          break;

        case C_COMMAND:
          reg = parser2.Comp().find("M") != string::npos ? 1 : 0;
          output_file << "111"
              << reg
              << comp_table.GetBinaryCode(parser2.Comp())
              << dest_table.GetBinaryCode(parser2.Dest())
              << jump_table.GetBinaryCode(parser2.Jump())
              << "\n";
          break;

        case L_COMMAND:
          break;
      }
    }
  }
  catch (...) {
    cout << "unknown error";
    return 1;
  }

  file.close();
  output_file.close();
  cout << "done";  

  return 0;
}