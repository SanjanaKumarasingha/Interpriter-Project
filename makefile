all:
	g++ -g -w -Wall -o p1 main.cpp Lexical_Analyzer\LexicalAnalyzer.cpp Parser\Parser.cpp Tree\Tree.cpp Standardizer.cpp CSEMachine.cpp Env.cpp	CSE


