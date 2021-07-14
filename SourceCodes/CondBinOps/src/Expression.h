#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <iostream>
#include <algorithm>
#include <regex>
#include <cctype>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>

#include <Generics.h>

using namespace std;

// Global variables
const string LST_OPERATORS[] = {"*","/","+","-","<=",">=","<",">","=","NOT","AND","OR"};
enum EN_OPERATORS{MULT, DIVIDE, PLUS, DIFF, LEQ, GEQ, LT, GT, EQ, NOT, AND, OR};
const map<string, string> REV_OPERATORS {{"=","="}, {"<",">"}, {"<=",">="}, {">","<"}, {">=","<="}};
const int NR_OPERATORS = 12;

/** Remember: -1: higher precedence, 0: same precedence, 1: lower precedence */
const int PRECEDENCES[12][12] = {
	{ 0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
	{ 0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1},
	{-1, -1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1},
	{-1, -1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1},
	{-1, -1, -1, -1,  0,  0,  0,  0,  0,  1,  1,  1},
	{-1, -1, -1, -1,  0,  0,  0,  0,  0,  1,  1,  1},
	{-1, -1, -1, -1,  0,  0,  0,  0,  0,  1,  1,  1},
	{-1, -1, -1, -1,  0,  0,  0,  0,  0,  1,  1,  1},
	{-1, -1, -1, -1,  0,  0,  0,  0,  0,  1,  1,  1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0}
};

const string ST_OPERATOR_ARITH = "\\*|/|\\+|-";
const string ST_OPERATOR_LOGIC = "<=|>=|<|>|=";
const string ST_OPERATOR_NEGAT = "NOT";
const string ST_OPERATOR_CONNE = "AND|OR";
const string ST_OPERATOR = ST_OPERATOR_ARITH + "|" + ST_OPERATOR_LOGIC + "|" + ST_OPERATOR_NEGAT + "|" + ST_OPERATOR_CONNE;
const string ST_BRACKETS = "\\(|\\)";
const string ST_OPERAND = "\\d+(\\.\\d+)?|\\w+\\.\\w+";
const string ST_TOKENS = ST_OPERATOR + "|" + ST_BRACKETS + "|" + ST_OPERAND;

const regex REG_OPERATOR_ARITH(ST_OPERATOR_ARITH);
const regex REG_OPERATOR_LOGIC(ST_OPERATOR_LOGIC);
const regex REG_OPERATOR_NEGAT(ST_OPERATOR_NEGAT);
const regex REG_OPERATOR_CONNE(ST_OPERATOR_CONNE);
const regex REG_OPERATOR(ST_OPERATOR);
const regex REG_BRACKETS(ST_BRACKETS);
const regex REG_OPERAND(ST_OPERAND);
const regex REG_TOKENS(ST_TOKENS);

/** This global method returns the index of an element in an array */
int indexOf(const string &item, const string array[], const int &size);

/** This is our class Expression which will be able to convert an infix expression
  * to a postfix form */
class Expression{
private:
	//ATTRIBUTES
	string infixStr;
	vector<string> infixVtr;
	vector<string> postfixVtr;

public:
	// METHODS
	// Constructors
	Expression(){};
	Expression(string infixStr);

	// Getters
	string getInfixStr();
	vector<string> getInfixVtr();
	vector<string> getPostfixVtr();

	// Setters
	void setInfixStr(string inStr);

	/** This method compare two operators precedence.
	  * It returns:
	  *    1 if first operator has greater precedence
	  *    0 if both operators has same precedence
	  *   -1 if first operator has lower precedence
	  *   -2 if op1 or op2 are not operators
	  */
	int cmpPrecedence(const string &op1, const string &op2);

	/** This method receives a string and a regular expression.
	  * Its objective is to return a vector of all tokens
	  * that match the regular expression.
	  */
	static vector<string> tokenize(const string &str, string reg);

	/** This method receives a list of tokens for an infix expression
	  * and transforms it into a postfix expression.
	  */
	void computePostfixExpression();

	/** This method displays the expression in postfix mode */
	void displayPostfix();
};


#endif
