#include "vm_writer.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace nand2tetris::vm_writer;

VMWriter::VMWriter(string file_name)
{
  out_.open(file_name.c_str());
}

void VMWriter::Close()
{
  if (out_.is_open())
  {
    out_.close();
  }
}

void VMWriter::WritePush(vm_writer::Segment segment, int index)
{
  out_ << "push " << vm_writer::Segment2String(segment) << " " << index << endl;
}

void VMWriter::WritePop(vm_writer::Segment segment, int index)
{
  out_ << "pop " << vm_writer::Segment2String(segment) << " " << index << endl;
}

void VMWriter::WriteArithmetic(vm_writer::Command command)
{
  out_ << vm_writer::Command2String(command) << endl;
}

void VMWriter::WriterLabel(string label)
{
  out_ << "label " << label << endl;
}

void VMWriter::WriteGoTo(string label)
{
  out_ << "goto " << label << endl;
}

void VMWriter::WriteIf(string label)
{
  out_ << "if-goto " << label << endl;
}

void VMWriter::WriteCall(string name, int num_of_locals)
{
  out_ << "call " << name << " " << numOfArgs << endl;
}

void VMWriter::WriteFunction(string name, int num_of_locals)
{
  out_ << "function " << name << " " << num_of_locals << endl;
}

void VMWriter::WriteFunction()
{
  out_ << "return" << endl;
}