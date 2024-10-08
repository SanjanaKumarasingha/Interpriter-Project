#pragma once
#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>
#include <stack>
#include <cstdlib>
#include "../Tree/Tree.h"
#include "Parser.h"

Parser::Parser(LexicalAnalyzer *mLexNew)
{
	isDebug = false;
	mLex = mLexNew;
	tokenType = mLex->processNext();
	tokenValue = mLex->lexeme;
	while (tokenType == COMMENT || tokenType == ERROR)
	{
		tokenType = mLex->processNext();
		tokenValue = mLex->lexeme;
	}
}

Tree Parser::makeTree(string mVal, int popCount)
{

	Tree *newTree = new Tree(mVal, GRAMMAR_RULE, NULL, NULL);
	if (popCount == 0)
	{
		treeStack.push(*newTree);
		return *newTree;
	}

	stack<Tree> tempStack;
	for (int i = 0; i < popCount; i++)
	{
		Tree tmp = treeStack.top();
		treeStack.pop();
		tempStack.push(tmp);
	}
	Tree anotherTemp = tempStack.top();
	Tree *tempBigSib = new Tree(&anotherTemp);
	newTree->childNode = tempBigSib;

	tempStack.pop();
	for (int i = 1; i < popCount; i++)
	{
		anotherTemp = tempStack.top();
		tempBigSib->siblingNode = new Tree(&anotherTemp);
		tempBigSib = tempBigSib->siblingNode;
		tempStack.pop();
	}
	treeStack.push(*newTree);
	return *newTree;
}

Tree *Parser::getTopTree()
{
	Tree *val = &(treeStack.top());
	return val;
}

void Parser::Read(string mToken)
{
	if (tokenType == STOP)
	{
		cout << "Input terminated unexpectedly" << endl;
		cin >> mToken;
		exit(0);
	}

	while (tokenType == COMMENT)
	{
		tokenType = mLex->processNext();
		tokenValue = mLex->lexeme;
	}

	if (mToken == "<ID>" || mToken == "<INT>" || mToken == "<STR>")
	{
		if (isKeyword(tokenValue) && mToken != "<STR>")
		{
			cout << "Expected " << mToken << " found KEYWORD " << tokenValue << endl;
			cin >> mToken;
			exit(0);
		}
		else
		{
			if (mToken == "<ID>" && tokenType != IDENTIFIER)
			{
				cout << "ERROR:\t Wanted IDENTIFIER, found " << mLex->getString(tokenType) << endl;
				cin >> mToken;
				exit(0);
			}
			if (mToken == "<INT>" && tokenType != INT_LITERAL)
			{
				cout << "ERROR:\t Wanted INTEGER, found " << mLex->getString(tokenType) << endl;
				cin >> mToken;
				exit(0);
			}
			if (mToken == "<STR>" && tokenType != STRING)
			{
				cout << "ERROR:\t Wanted STRING, found " << mLex->getString(tokenType) << endl;
				cin >> mToken;
				exit(0);
			}

			if (mToken == "<ID>")
				treeStack.push(Tree(tokenValue, ID_NAME, nullptr, nullptr));
			else if (mToken == "<INT>")
				treeStack.push(Tree(tokenValue, INTEGER_VALUE, nullptr, nullptr));
			else if (mToken == "<STR>")
				treeStack.push(Tree(tokenValue, STRING_VALUE, nullptr, nullptr));
		}
	}
	else
	{
		if (mToken != tokenValue)
		{
			cout << "KEYWORD MISMATCH:\t Wanted " << mToken << " , found " << tokenValue << endl;
			cin >> mToken;
			exit(0);
		}
	}

	tokenType = mLex->processNext();
	tokenValue = mLex->lexeme;

	while (tokenType == COMMENT)
	{
		tokenType = mLex->processNext();
		tokenValue = mLex->lexeme;
	}
}

bool Parser::isKeyword(string mTok)
{

	string reserved_keys[] = {"let", "fn", "in", ".", "where",
							  ",", "aug", "->", "|",
							  "or", "&", "not", "gr", ">", "ge", ">=", "ls", "<", "le", "<=", "eq", "ne",
							  "+", "-", "*", "/", "**", "@",
							  "true", "false", "nil", "(", ")", "dummy",
							  "within", "and", "rec", "="};

	for (int i = 0; i < 38; i++)
	{
		if (reserved_keys[i] == mTok)
			return true;
	}
	return false;
}

void Parser::E()
{
	if (isDebug)
	{
		cout << "Entering E()............stack size:" << treeStack.size() << "\n";
	}
	if (tokenValue == "let")
	{
		Read("let");
		D();
		Read("in");
		E();
		makeTree("let", 2);
	}
	else if (tokenValue == "fn")
	{
		Read("fn");
		int N = 0;
		do
		{
			Vb();
			N++;
		} while (tokenType == IDENTIFIER || tokenType == OPENING);
		Read(".");
		E();
		makeTree("lambda", N + 1);
	}
	else
		Ew();

	if (isDebug)
		cout << ".........Returning E()...stack size:" << treeStack.size() << "\n";
}

void Parser::Ew()
{
	if (isDebug)
	{
		cout << "Entering Ew()............stack size:" << treeStack.size() << "\n";
	}
	T();
	if (tokenValue == "where")
	{
		Read("where");
		Dr();
		makeTree("where", 2);
	}

	if (isDebug)
		cout << ".........Returning Ew()...stack size:" << treeStack.size() << "\n";
}

void Parser::T()
{
	if (isDebug)
	{
		cout << "Entering T()............stack size:" << treeStack.size() << "\n";
	}
	Ta();
	int N = 1;
	while (tokenValue == ",")
	{
		Read(",");
		Ta();
		N = N + 1;
	}
	if (N > 1)
		makeTree("tau", N);

	if (isDebug)
		cout << ".........Returning T()...stack size:" << treeStack.size() << "\n";
}

void Parser::Ta()
{
	if (isDebug)
	{
		cout << "Entering Ta()............stack size:" << treeStack.size() << "\n";
	}
	Tc();
	while (tokenValue == "aug")
	{
		Read("aug");
		Tc();
		makeTree("aug", 2);
	}

	if (isDebug)
		cout << ".........Returning Ta()...stack size:" << treeStack.size() << "\n";
}

void Parser::Tc()
{
	if (isDebug)
	{
		cout << "Entering Tc()............stack size:" << treeStack.size() << "\n";
	}
	B();
	if (tokenValue == "->")
	{
		Read("->");
		Tc();
		Read("|");
		Tc();
		makeTree("->", 3);
	}
	if (isDebug)
		cout << ".........Returning Tc()...stack size:" << treeStack.size() << "\n";
}

void Parser::B()
{
	if (isDebug)
	{
		cout << "Entering B()............stack size:" << treeStack.size() << "\n";
	}
	Bt();
	while (tokenValue == "or")
	{
		Read("or");
		Bt();
		makeTree("or", 2);
	}
	if (isDebug)
		cout << ".........Returning B()...stack size:" << treeStack.size() << "\n";
}

void Parser::Bt()
{
	if (isDebug)
	{
		cout << "Entering Bt()............stack size:" << treeStack.size() << "\n";
	}
	Bs();
	while (tokenValue == "&")
	{
		Read("&");
		Bs();
		makeTree("&", 2);
	}
	if (isDebug)
		cout << ".........Returning Bt()...stack size:" << treeStack.size() << "\n";
}

void Parser::Bs()
{
	if (isDebug)
	{
		cout << "Entering Bs()............stack size:" << treeStack.size() << "\n";
	}
	if (tokenValue == "not")
	{
		Read("not");
		Bp();
		makeTree("not", 1);
	}
	else
		Bp();

	if (isDebug)
		cout << ".........Returning Bs()...stack size:" << treeStack.size() << "\n";
}

void Parser::Bp()
{
	if (isDebug)
	{
		cout << "Entering Bp()............stack size:" << treeStack.size() << "\n";
	}
	A();
	if (tokenValue == "gr")
	{
		Read("gr");
		A();
		makeTree("gr", 2);
	}
	else if (tokenValue == ">")
	{
		Read(">");
		A();
		makeTree("gr", 2);
	}
	else if (tokenValue == "ge")
	{
		Read("ge");
		A();
		makeTree("ge", 2);
	}
	else if (tokenValue == ">=")
	{
		Read(">=");
		A();
		makeTree("ge", 2);
	}
	else if (tokenValue == "ls")
	{
		Read("ls");
		A();
		makeTree("ls", 2);
	}
	else if (tokenValue == "<")
	{
		Read("<");
		A();
		makeTree("ls", 2);
	}
	else if (tokenValue == "le")
	{
		Read("le");
		A();
		makeTree("le", 2);
	}
	else if (tokenValue == "<=")
	{
		Read("<=");
		A();
		makeTree("le", 2);
	}
	else if (tokenValue == "eq")
	{
		Read("eq");
		A();
		makeTree("eq", 2);
	}
	else if (tokenValue == "ne")
	{
		Read("ne");
		A();
		makeTree("ne", 2);
	}
	if (isDebug)
		cout << ".........Returning Bp()...stack size:" << treeStack.size() << "\n";
}

void Parser::A()
{
	if (isDebug)
	{
		cout << "Entering A()............stack size:" << treeStack.size() << "\n";
	}
	if (tokenValue == "-" && tokenType != STRING)
	{
		Read("-");
		At();
		makeTree("neg", 1);
	}
	else if (tokenValue == "+" && tokenType != STRING)
	{
		Read("+");
		At();
	}
	else
		At();

	while ((tokenValue == "-" || tokenValue == "+") && tokenType != STRING)
	{
		string tempToken = tokenValue;
		Read(tokenValue);
		At();
		makeTree(tempToken, 2);
	}
	if (isDebug)
		cout << ".........Returning A()...stack size:" << treeStack.size() << "\n";
}

void Parser::At()
{
	if (isDebug)
	{
		cout << "Entering At()............stack size:" << treeStack.size() << "\n";
	}
	Af();
	while (tokenValue == "*" || tokenValue == "/")
	{
		if (tokenValue == "*")
		{
			Read("*");
			Af();
			makeTree("*", 2);
		}
		else if (tokenValue == "/")
		{
			Read("/");
			Af();
			makeTree("/", 2);
		}
	}

	if (isDebug)
		cout << ".........Returning At()...stack size:" << treeStack.size() << "\n";
}

void Parser::Af()
{
	if (isDebug)
	{
		cout << "Entering Af()............stack size:" << treeStack.size() << "\n";
	}
	Ap();
	if (tokenValue == "**")
	{
		Read("**");
		Af();
		makeTree("**", 2);
	}

	if (isDebug)
		cout << ".........Returning Af()...stack size:" << treeStack.size() << "\n";
}

void Parser::Ap()
{
	if (isDebug)
	{
		cout << "Entering Ap()............stack size:" << treeStack.size() << "\n";
	}
	R();
	while (tokenValue == "@")
	{
		Read("@");
		Read("<ID>");
		R();
		makeTree("@", 3);
	}

	if (isDebug)
		cout << ".........Returning Ap()...stack size:" << treeStack.size() << "\n";
}

void Parser::R()
{
	if (isDebug)
	{
		cout << "Entering R()............stack size:" << treeStack.size() << "\n";
	}
	Rn();
	while (tokenValue == "true" ||
		   tokenValue == "false" ||
		   tokenValue == "nil" ||
		   tokenValue == "dummy" ||
		   (tokenType == IDENTIFIER && !(isKeyword(tokenValue))) ||
		   tokenType == OPENING ||
		   tokenType == INT_LITERAL ||
		   tokenType == STRING)
	{
		Rn();
		makeTree("gamma", 2);
	}

	if (isDebug)
		cout << ".........Returning R()...stack size:" << treeStack.size() << "\n";
}

void Parser::Rn()
{
	if (isDebug)
	{
		cout << "Entering Rn()............stack size:" << treeStack.size() << "\n";
	}
	if (isKeyword(tokenValue) && tokenType != STRING)
	{
		if (tokenValue == "true")
		{
			Read("true");
			makeTree("<true>", 0);
		}
		else if (tokenValue == "false")
		{
			Read("false");
			makeTree("<false>", 0);
		}
		else if (tokenValue == "nil")
		{
			Read("nil");
			makeTree("<nil>", 0);
		}
		else if (tokenValue == "dummy")
		{
			Read("dummy");
			makeTree("<dummy>", 0);
		}
		else if (tokenValue == "(")
		{
			if (tokenType == OPENING)
			{
				Read("(");
				E();
				Read(")");
			}
			else if (tokenType == STRING)
			{
				Read("<STR>");
			}
		}
	}
	else
	{
		switch (tokenType)
		{
		case IDENTIFIER:
			Read("<ID>");
			break;
		case INT_LITERAL:
			Read("<INT>");
			break;
		case STRING:
			Read("<STR>");
			break;
		}
	}

	if (isDebug)
		cout << ".........Returning Rn()...stack size:" << treeStack.size() << "\n";
}

void Parser::D()
{
	if (isDebug)
	{
		cout << "Entering D()............stack size:" << treeStack.size() << "\n";
	}
	Da();
	if (tokenValue == "within")
	{
		Read("within");
		D();
		makeTree("within", 2);
	}

	if (isDebug)
		cout << ".........Returning D()...stack size:" << treeStack.size() << "\n";
}

void Parser::Da()
{
	if (isDebug)
	{
		cout << "Entering Da()............stack size:" << treeStack.size() << "\n";
	}
	Dr();
	int N = 1;
	while (tokenValue == "and")
	{
		Read("and");
		Dr();
		N = N + 1;
	}

	if (N > 1)
		makeTree("and", N);

	if (isDebug)
		cout << ".........Returning Da()...stack size:" << treeStack.size() << "\n";
}

void Parser::Dr()
{
	if (isDebug)
	{
		cout << "Entering Dr()............stack size:" << treeStack.size() << "\n";
	}
	if (tokenValue == "rec")
	{
		Read("rec");
		Db();
		makeTree("rec", 1);
	}
	else
		Db();

	if (isDebug)
		cout << ".........Returning Dr()...stack size:" << treeStack.size() << "\n";
}

void Parser::Db()
{
	if (isDebug)
	{
		cout << "Entering Db()............stack size:" << treeStack.size() << "\n";
	}
	if (tokenValue == "(")
	{
		Read("(");
		D();
		Read(")");
	}
	else if (tokenType == IDENTIFIER && !isKeyword(tokenValue))
	{
		Read("<ID>");
		int childrenCt = 1;
		if (tokenValue == "," || tokenValue == "=")
		{
			while (tokenValue == ",")
			{
				Read(",");
				Read("<ID>");
				childrenCt++;
			}
			if (childrenCt > 1)
				makeTree(",", childrenCt);

			Read("=");
			E();
			makeTree("=", 2);
		}
		else
		{
			do
			{
				Vb();
				childrenCt++;
			} while (tokenType == OPENING || (tokenType == IDENTIFIER && !isKeyword(tokenValue)));
			Read("=");
			E();
			makeTree("function_form", childrenCt + 1);
		}
	}

	if (isDebug)
		cout << ".........Returning Db()...stack size:" << treeStack.size() << "\n";
}

void Parser::Vb()
{
	if (isDebug)
	{
		cout << "Entering Vb()............stack size:" << treeStack.size() << "\n";
	}
	if (tokenType == IDENTIFIER && !isKeyword(tokenValue))
		Read("<ID>");
	else if (tokenType == OPENING)
	{
		Read("(");
		if (tokenType == CLOSING)
		{
			Read(")");
			makeTree("()", 0);
		}
		else
		{
			Vl();
			Read(")");
		}
	}
	if (isDebug)
		cout << ".........Returning Vb()...stack size:" << treeStack.size() << "\n";
}

void Parser::Vl()
{
	if (isDebug)
	{
		cout << "Entering Vl()............stack size:" << treeStack.size() << "\n";
	}
	Read("<ID>");
	int N = 1;
	while (tokenValue == ",")
	{
		Read(",");
		Read("<ID>");
		N++;
	}
	if (N > 1)
		makeTree(",", N);

	if (isDebug)
		cout << ".........Returning Vl()...stack size:" << treeStack.size() << "\n";
}
