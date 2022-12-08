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
    int start_;
    string filename_;

    void PopD();
    void IncSP();
    void DecSP();

  public:
    CodeWriter(): {};
    void SetFileName(string filename);
    void WriteArithmetic(string command);
    void WritePushPop(CommandType command, string segment, int index);
    void Close();
};

}
}

#endif