#ifndef CODE_WRITER
#define CODE_WRITER 1

#include <fstream>
#include <string>
#include <exception>

namespace nand2tetris {
namespace vm_translator2 {

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
    void WritePushPop(VMCommandType command, string segment, int index);
    void WriteInit();
    void WriteLabel(string label);
    void WriteGoTo(string label);
    void WriteIf(string label);
    void WriteCall(string functaion_name, int num_args);
    void WriteReturn();
    void WriteFunction(string function_name, int num_locals);
    void Close();
};

}
}

#endif