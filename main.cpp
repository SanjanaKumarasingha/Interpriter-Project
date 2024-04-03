#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stack>
#include "Lexical_Analyzer/LexicalAnalyzer.cpp"
#include "Tree/Tree.cpp"
#include "Parser/Parser.cpp"

using namespace std;

int main(int argc, char *argv[])
{
	string filename = "Test\\add";
	ifstream f(filename);
	string content((istreambuf_iterator<char>(f)), (istreambuf_iterator<char>()));
	LexicalAnalyzer Lex(content);

	Parser Par(&Lex);
	Par.E();
	cout << "\n\n Abstract Syntaxt Tree \n\n";
	Tree::TreePrint(Par.getTopTree());

	return 0;
}
