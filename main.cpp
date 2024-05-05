#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "Lexical_Analyzer/LexicalAnalyzer.cpp"
#include "Tree/Tree.cpp"
#include "Parser/Parser.cpp"
#include "Standardizer/Standardizer.cpp"
#include "CSEMachine/CSEMachine.cpp"

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

	cout << "\n\n Stdandardized Tree\n\n";
	Tree::TreePrint(Standardizer::standardizeTree(Par.getTopTree()));

	cout << "\n\n CSE Machine structure \n\n";
	CSEMachine csemachine(Par.getTopTree());

	cout << "Terminated Succesfully!" << endl;

	return 0;
}
