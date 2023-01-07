#include "jack_tokenizer.hpp"
#include "jack_tokenizer.cpp"
#include "compilation_engine.hpp"
#include "compilation_engine.cpp"
#include "symbol_table.hpp"
#include "symbol_table.cpp"
#include "vm_writer.hpp"
#include "vm_writer.cpp"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    cerr << "error no files provided \n";
  }
  else
  {
    for (int i = 1; i < argc; i++)
    {
      string file_name = argv[i];
      int dotjack = file_name.find(".jack", 0);
      if (dotjack == -1)
      {
        cout << "please provide not dir but file\n";
        return (1);
      }
      else
      {
        JackTokenizer tokenizer(file_name);
        string output_file = file_name.substr(0, dotjack) + ".vm";
        VMWriter vm_writer(output_file);
        SymbolTable symbol_table;
        CompilationEngine compilation_engine(&tokenizer, &vm_writer, &symbol_table);
      }
    }
  }
}