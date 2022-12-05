#include "parser.hpp"
#include "code_writer.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace nand2tetris::vm_translator;

int main(int argc, char** argv) {
  string file_name;

  if (argc != 2) {
    cout << "error expected one argument(file name)\n";
    cout << "enter the filename: ";
    cin >> file_name;
  } else {
    file_name = argv[1];
  }

  ifstream file(file_name.c_str());
  if (!file) {
    cout << "cant find this file\n";
    return 1;
  }

  Parser parser(file);

  ofstream output_file("main.asm");
  CodeWriter code_writer(output_file);

  
}