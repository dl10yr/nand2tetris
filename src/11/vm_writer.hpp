#ifndef VM_WRITER
#define VM_WRITER 1

#include <string>
#include <fstream>
using namespace std;

namespace nand2tetris {
namespace vm_writer {

enum Segment { CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP };
enum Command { ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT };

string Segment2String(Segment segment) {
  switch(seg) {
    case CONST:
      return "constant";
    case ARG:
      return "argument";
    case LOCAL:
      return "local";
    case STATIC:
      return "static";
    case THIS:
      return "this";
    case THAT:
      return "that";
    case POINTER:
      return "pointer";
    case TEMP:
      return "temp";
  }
}

string Command2String(Command command) {
  switch(command) {
    case ADD:
      return "add";
    case SUB:
      return "sub";
    case NEG:
      return "neg";
    case EQ:
      return "eq";
    case GT:
      return "gt";
    case LT:
      return "lt";
    case AND:
      return "and";
    case OR:
      return "or";
    case NOT:
      return "not";
  }
}

class VMWriter {
  private:
    ofstream out_;

  public:
    VMWriter(string file_name);

    void WritePush(Segment segment, int index);
    void WritePop(segment int index);
    void WriteArithmetic(Command command);
    void WriteLabel(string label);
    void WriteGoTo(string label);
    void WriteIf(string label);
    void WriteCall(string name, int n_of_args);
    void WriteFunction(string name, int n_of_locals);
    void WriteReturn();
    void Close();
};

}
}

#endif