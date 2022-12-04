#ifndef CODE_WRITER
#define CODE_WRITER 1

#include <fstream>
#include <string>
#include <exception>

namespace nand2tetris {
namespace vm_translator {

class CodeWriter {
  private:
    ifstream& in_;

  public:
    CodeWriter(ifstream& f): in_(f) {};
    void SetFileName(string file_name);
    void WriteArithmetic(string command);
    void WritePushPop(CommandType command, string segment, int index);
    void Close();
};

}
}

#endif