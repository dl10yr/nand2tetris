#ifndef CODE_WRITER
#define CODE_WRITER 1

#include <fstream>
#include <string>
#include <exception>

namespace nand2tetris {
namespace vm_translator {

class CodeWriter {
  private:
    ofstream& out_;
    int labelnum_;

    void PopD();
    void IncSP();
    void DecSP();

  public:
    CodeWriter(ofstream& f): out_(f) {};
    void SetFileName(string file_name);
    void WriteArithmetic(string command);
    void WritePushPop(CommandType command, string segment, int index);
    void Close();
};

}
}

#endif