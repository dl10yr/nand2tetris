#include "compilation_engine.hpp"
#include "jack_tokenizer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;
using namespace nand2tetris::compilation_engine;
using namespace nand2tetris::jack_tokenizer;

namespace CEhelper {
  string KeyWord2String(JackKeyWord key) {
    switch (key) {
      case CLASS:
        return "class";
      case METHOD:
        return "method";
      case FUNCTION:
        return "function";
      case CONSTRUCTOR:
        return "constructor";
      case INT:
        return "int";
      case BOOLEAN:
        return "boolean";
      case CHAR:
        return "char";
      case VOID:
        return "void";
      case VAR:
        return "var";
      case STATIC:
        return "static";
      case FIELD:
        return "field";
      case LET:
        return "let";
      case DO:
        return "do";
      case IF:
        return "if";
      case ELSE:
        return "else";
      case WHILE:
        return "while";
      case RETURN:
        return "return";
      case TRUE:
        return "true";
      case FALSE:
        return "false";
      case null:
        return "null";
      case THIS:
        return "this";
    }
  }
}

CompilationEngine::CompilationEngine(JackTokenizer * tokenizer, string output_file) {
  tokenizer_ = tokenizer;
  out_.open(output_file.c_str());
  if (!out_.is_open()) {
    cerr << "Failed to open output file.\n";
  }
  CompileClass();
}

CompilationEngine::~CompilationEngine() {
  if (out_.is_open()) {
    out_.close();
  }
}

void CompilationEngine::CompileClass() {
  out_ << "<class>" << endl;
  tokenizer_->Advance();
  assert(tokenizer_->KeyWord() == CLASS);
  out_ << "<keyword> " << "class" << " </keyword>" << endl;
  tokenizer_->Advance();
  assert(tokenizer_->TokenType() == IDENTIFIER);
  out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
  tokenizer_->Advance();
  assert(tokenizer_->Symbol() == '{');
  out_ << "<symbol> { </symbol>" << endl;
  tokenizer_->Advance();
  while ((tokenizer_->TokenType() == KEYWORD) && (tokenizer_->Keyword() == STATIC
    || tokenizer_->KeyWord() == FIELD || tokenizer_->KeyWord() == CONSTRUCTOR || tokenizer_->KeyWord() == function
    || tokenizer_->KeyWord() == METHOD || tokenizer_->KeyWord() == VOID
  )) {
    switch(tokenizer_->KeyWord()) {
      case STATIC:
      case FIELD:
        CompileClassVarDec();
        break;
      case CONSTRUCTOR:
      case FUNCTION:
      case METHOD:
      case VOID:
        CompileSubroutine();
        break;
      default:
        break;
    }
  }

  assert(tokenizer_->Symbol() == '}');
  out_ << "<symbol> } </symbol>" << endl;
  out_ << "</class>" << endl;
}

void CompilationEngine::CompileClassVarDec() {
  out_ << "<classVarDec>" << endl;
  out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
  tokenizer_->Advance();
  if (tokenizer_->TokenType() == KEYWORD) {
    out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
    tokenizer_->Advance(); 
  } else if (tokenizer_->TokenType() == IDENTIFIER) {
    out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
    tokenizer_->Advance();
  }
  while (tokenizer_->GetCurrentToken() != ";") {
    switch(tokenizer_->TokenType()) {
      case IDENTIFIER:
        out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" < endl;
        break;
      case SYMBOL:
        out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
        tokenizer_->UpdateCurrentToken();
        break;
    }
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    }
  }
  assert(tokenizer_->Symbol() == ';');
  out_ << "<symbol> ; </symbol>" << endl;
  out_ << "</classVarDec>" << endl;
  tokenizer_->Advance();
}

void CompilationEngine::CompileSubroutine() {
  out_ << "<subroutineDec>" << endl;
  out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
  tokenizer_->Advance();
  if (tokenizer_->TokenType() == KEYWORD) {
    out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
    tokenizer_->Advance();
  } else if (tokenizer_->TokenType() == IDENTIFIER) {
    out_ << "<identifier> " << tokenizer->Identifier() << " </identifier>" << endl;
    tokenizer_->Advance();
  }

  if (tokenizer_->TokenType() == IDENTIFIER) {
    out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    } else if (tokenizer_->Symbol() == '(') {
      out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer_->UpdateCurrentToken();
      if (tokenizer_->GetCurrentToken() == "") {
        tokenizer_->Advance();
      }
      CompileParameterList();
    }
    if (tokenizer_->GetCurrentToken() == ")") {
      out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer->UpdateCurrentToken();
    }
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer->Advance();
    }
    if (tokenizer_->GetCurrentToken() == "{") {
      CompileSubroutineBody();
    }
  }

  out_ << "</subroutineDec>" << endl;
  tokenizer_->Advance();
}

void CompilationEngine::CompileParameterList() {
  out_ << "<parameterList>" << endl;
  while(tokenizer_->GetCurrentToken() != ")") {
    switch(tokenizer_->TokenType()) {
      case KEYWORD:
        out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
        tokenizer_->Advance();
        break;
      case IDENTIFIER:
        out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
        break;
      case SYMBOL:
        out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
        tokenizer_->UpdateCurrentToken();
        break;
    }
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    }
  }
  out_ << "</paramterList>" << endl;
}

void CompilationEngine::CompileSubroutineBody() {
  out_ << "<subtourineBody>" << endl;
  out_ << "<symbol>" << "{" << "</symbol>" << endl;
  tokenizer_->UpdateCurrentToken();
  if (tokenizer_->GetCurrentToken() == "") {
    tokenizer_->Advance();
  }
  while (tokenizer_->KeyWord() == VAR) {
    CompileVarDec();
  }
  if (tokenizer_->KeyWord() == LET || tokenizer_->KeyWord() == IF || tokenizer_->KeyWord() == ELSE
    || tokenizer_->KeyWord() == WHILE || tokenizer_->KeyWord() == DO || tokenizer_->KeyWord() == RETURN
  ) {
    CompileStatements();
  }
  if (tokenizer_->Symbol() == '}') {
    out_ << "<symbol>" << tokenizer_->Symbol() << "</symbol>" << endl;
  }
  out_ << "</subroutineBody>" << endl;
}

void CompilationEngine::CompileVarDec() {
  out_ << "<varDec>" << endl;
  out_ << "<keyword>" << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << "</keyword>" << endl;
  tokenizer_->Advance();
  while(tokenizer_->GetCurrentToken() != ";") {
    switch (tokenizer_->TokenType()) {
      case KEYWORD:
        out_ << "<keyword>" << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
        tokenizer_->Advance();
        break;
      case IDENTIFIER:
        out_ << "<identifer>" << tokenizer_->Identifier() << " </identifier>" << endl;
        break;
      case SYMBOL:
        out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
        tokenizer_->UpdateCurrentToken();
    }
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    }
  }
  if (tokenizer_->GetCurrentToken() == ";") {
    out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
    tokenizer_->Advance();
  }
  out_ << "</varDec>" << endl;
}

void CompilationEngine::CompileStatements() {
  out_ << "<statements>" << endl;
  while((tokenizer_->TokenType() == KEYWORD) && (tokenizer_->KeyWord() == LET || tokenizer_->KeyWord() == IF
    || tokenizer_->KeyWord() == ELSE || tokenizer_->KeyWord() == WHILE
    || tokenizer_->KeyWord() == DO || tokenizer_->KeyWord() == RETURN
  )) {
    switch(tokenizer_->KeyWord()) {
      case LET:
        CompileLet();
        break;
      case DO:
        CompileDo();
        break;
      case WHILE:
        CompileWhile();
        break;
      case RETURN:
        CompileReturn();
        break;
      case IF:
        CompileIf();
        break;
    }
  }
  out_ << "</statements>" << endl;
}

void CompilationEngine::CompileDo() {
  out_ << "<doStatement>" << endl;
  out_ << "<keyword>" << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
  tokenizer_->Advance();
  out_ << "<idetifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
  if (tokenizer_->Symbol() == '(') {
    out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
    tokenizer_->UpdateCurrentToken();
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    }
    CompileExpression();
    if (tokenizer_->Symbol() == ')') {
      out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer_->UpdateCurrentToken();
      if (tokenizer_->GetCurrentToken() == "") {
        tokenizer_->Advance();
      }
    }
  } else if (tokenizer_->Symbol() == '.') {
    out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
    tokenizer_->UpdateCurrentToken();
    out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
    if (tokenizer_->Symbol() == '(') {
      out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer_->UpdateCurrentToken();
    }
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    }
    CompileExpressionList();
    if (tokenizer_->Symbol() == ')') {
      out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer_->UpdateCurrentToken();
      if (tokenizer_->GetCurrentToken() == "") {
        tokenizer_->Advance();
      }
    }
    assert(tokenizer_->Symbol() == ';');
    out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
    tokenizer_->Advance();
    out_ << "</doStatement>" << endl;
  }
}

void CompilationEngine::CompileLet() {
  out_ << "<letStatement>" << endl;
  out_ << "<keyword>" << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
  tokenizer_->Advance();
  out_ << "<identifier>" << tokenizer_->Identifier() << " </idnetifier>" << endl;
  if (tokenizer_->GetCurrentToken() == "") {
    tokenizer_->Advance();
  }
  if (tokenizer_->Symbol() == '[') {
    out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
    tokenizer_->UpdateCurrentToken();
    if (tokenizer_->GetCurrentToken() == "") {
      tokenizer_->Advance();
    }
    CompileExpression();
    if (tokenizer_->Symbol() == ']') {
      out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer_->UpdateCurrentToken();
      if (tokenizer_->GetCurrentToken() == "") {
        tokenizer_->Advance();
      }
    }
    if (tokenizer_->Symbol() == '=') {
      out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
      tokenizer_->UpdateCurrentToken();
      if (tokenizer_->GetCurrentToken() == "") {
        tokenizer_->Advance();
      }
      CompileExpression();
    }
    if (tokenizer_->Symbol() == ';') {
      out_ << "<symbol>" << tokenizer_->Symbol() << " </symbol>" << endl;
    }
    tokenizer_->Advance();
    out_ << "</letStatement>" << endl;
  }
}

void CompilationEngine::CompileWhile() {
  out_ << "<whileStatement>" << endl;
  out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
  tokenizer_->UpdateCurrentToken();
  if (tokenizer_->GetCurrentToken().substr(0, 1) != "(") {
    tokenizer_->Advance();
  }
	assert(tokenizer_->Symbol() == '(');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	CompileExpression();
	assert(tokenizer_->Symbol() == ')');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	assert(tokenizer_->Symbol() == '{');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	CompileStatements();
	assert(tokenizer_->Symbol() == '}');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	out_ << "</whileStatement>" << endl;
}

void CompilationEngine::CompileReturn() {
  out_ << "<returnStatement>" << endl;
  out_ << "<keyword>" << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
  if (tokenizer_->GetCurrentToken() == "return") {
    tokenizer_->Advance();
		CompileExpression();
		assert(tokenizer_->Symbol() == ';');
		out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
		tokenizer_->Advance();
	}
	else if(tokenizer_->GetCurrentToken() == "return;")
	{
		out_ << "<symbol> " << ";" << " </symbol>" << endl;
		tokenizer_->Advance();		
	}
	out_ << "</returnStatement>" << endl;
}

void CompilationEngine::CompileIf() {
  out_ << "<ifStatement>" << endl;
  out_ << "<keyword>" < CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
	tokenizer_->UpdateCurrentToken();
	tokenizer_->UpdateCurrentToken(); // done twice since "if" is 2 letters
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	assert(tokenizer_->Symbol() == '(');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	CompileExpression();
	assert(tokenizer_->Symbol() == ')');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	assert(tokenizer_->Symbol() == '{');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	CompileExpression();
	assert(tokenizer_->Symbol() == '}');
	out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
	tokenizer_->UpdateCurrentToken();
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	if(tokenizer_->KeyWord() == ELSE)
	{
		out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
		tokenizer_->UpdateCurrentToken();
		if(tokenizer_->GetCurrentToken() == "")
			tokenizer_->Advance();
		assert(tokenizer_->Symbol() == '{');
		out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
		tokenizer_->UpdateCurrentToken();
		if(tokenizer_->GetCurrentToken() == "")
			tokenizer_->Advance();
		CompileExpression();
		assert(tokenizer_->Symbol() == '}');
		out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
		tokenizer_->UpdateCurrentToken();
		if(tokenizer_->GetCurrentToken() == "")
			tokenizer_->Advance();
	}
	out_ << "</ifStatement>" << endl;
}

void CompilationEngine::CompileTerm() {
	out_ << "<term>" << endl;
	switch(tokenizer_->TokenType())
	{
		case IDENTIFIER:
			out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
			if(tokenizer_->GetCurrentToken() != "")
			{
				if(tokenizer_->Symbol() == '(') // subroutine call
				{
					out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
					tokenizer_->UpdateCurrentToken();
					if(tokenizer_->GetCurrentToken() == "")
						tokenizer_->Advance();
					CompileExpressionList();
					if(tokenizer_->Symbol() == ')')
					{
						out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
						tokenizer_->UpdateCurrentToken();
						if(tokenizer_->GetCurrentToken() == "")
							tokenizer_->Advance();
					}
				}
				else if(tokenizer_->Symbol() == '.')
				{
					out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
					tokenizer_->UpdateCurrentToken();
					out_ << "<identifier> " << tokenizer_->Identifier() << " </identifier>" << endl;
					if(tokenizer_->Symbol() == '(')
					{
						out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
						tokenizer_->UpdateCurrentToken();
					}
					if(tokenizer_->GetCurrentToken() == "")
						tokenizer_->Advance();
					CompileExpressionList();
					if(tokenizer_->Symbol() == ')')
					{
						out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
						tokenizer_->UpdateCurrentToken();
					}
				}
				else if(tokenizer_->Symbol() == '[')
				{
					out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
					tokenizer_->UpdateCurrentToken();
					if(tokenizer_->GetCurrentToken() == "")
						tokenizer_->Advance();
					CompileExpression();
					if(tokenizer_->Symbol() == ']')
					{
						out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
						tokenizer_->UpdateCurrentToken();
					}
				}
			}
			break;
		case INT_CONST:
			out_ << "<integerConstant> " << tokenizer_->IntVal() << " </integerConstant>" << endl;
			break;
		case STRING_CONST:
			out_ << "<stringConstant> " << tokenizer_->StringVal() << " </stringConstant>" << endl;
			break;
		case KEYWORD: // true, false, null, this will automatically update current token when calling keyWord()
			out_ << "<keyword> " << CEhelper::KeyWord2String(tokenizer_->KeyWord()) << " </keyword>" << endl;
			break;
		case SYMBOL:
			if(tokenizer_->Symbol() == '(')
			{
				out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
				tokenizer_->UpdateCurrentToken();
				if(tokenizer_->GetCurrentToken() == "")
					tokenizer_->Advance();
				CompileExpression();
				assert(tokenizer_->Symbol() == ')');
				out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
				tokenizer_->UpdateCurrentToken();
			}
			else //unaryOp term
			{
				out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
				tokenizer_->UpdateCurrentToken();
				if(tokenizer_->GetCurrentToken() == "")
					tokenizer_->Advance();
				CompileTerm();
			}
			break;
	}
	if(tokenizer_->GetCurrentToken() == "")
		tokenizer_->Advance();
	out_ << "</term>" << endl;
}

void CompilationEngine::CompileExpression() {
	out_ << "<expression>" << endl;
	CompileTerm();
	while(tokenizer_->GetCurrentToken().substr(0,1) != ";" && tokenizer_->GetCurrentToken().substr(0,1) != "}" && tokenizer_->GetCurrentToken().substr(0,1) != "]" && tokenizer_->GetCurrentToken().substr(0,1) != ")" && tokenizer_->GetCurrentToken().substr(0,1) != ",")
	{
		switch(tokenizer_->TokenType())
		{
			case SYMBOL:
				if(tokenizer_->Symbol() == '<')
					out_ << "<symbol> " << "&lt;" << " </symbol>" << endl;
				else if(tokenizer_->Symbol() == '>')
					out_ << "<symbol> " << "&gt;" << " </symbol>" << endl;
				else if(tokenizer_->Symbol() == '&')
					out_ << "<symbol> " << "&amp;" << " </symbol>" << endl;
				else
					out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
				tokenizer_->UpdateCurrentToken();
				if(tokenizer_->GetCurrentToken() == "")
					tokenizer_->Advance();
				CompileTerm();
				break;
			default:
				CompileTerm();
				break;
		}
		if(tokenizer_->GetCurrentToken() == "")
			tokenizer_->Advance();
	}
	out_ << "</expression>" << endl;
}

void CompilationEngine::CompileExpressionList() {
	out_ << "<expressionList>" << endl;
	while(tokenizer_->GetCurrentToken().substr(0,1) != ")")
	{
		CompileExpression();
		if(tokenizer_->GetCurrentToken().substr(0,1)==",")
		{
			out_ << "<symbol> " << tokenizer_->Symbol() << " </symbol>" << endl;
			tokenizer_->UpdateCurrentToken();
			if(tokenizer_->GetCurrentToken() == "")
				tokenizer_->Advance();
		}
	}
	out_ << "</expressionList>" << endl;
}