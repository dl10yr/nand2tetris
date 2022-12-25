#include "jack_tokenizer.hpp"
#include "jack_tokenizer.cpp"
#include "compilation_engine.hpp"
#include "compilation_engine.cpp"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "error: no files or directories provided\n";
  } else {
    for (int i = 1; i < argc; i++) {
      string filename = argv[i];
      int dot_jack = filename.find(".jack", 0);
      if (dot_jack == -1) {
        cout << "directory support currently not available\n";
        return(1);
      } else {
        JackTokenizer tokenizer(filename);
        string output_file = filename.substr(0, dot_jack) + ".xml";
        CompilationEngine engine(&tokenizer, output_file);
      }
    }
  }
}