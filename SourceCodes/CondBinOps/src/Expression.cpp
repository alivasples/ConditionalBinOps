#include "Expression.h"

/** This global method returns the index of an element in an array */
int indexOf(const string &item, const string array[], const int &size){
	for(int i = 0; i < size; i++){
		if(array[i] == item) return i;
	}
	return -1;
}


// Expression Class

/** Expression Constructor */
Expression::Expression(string inStr){
	setInfixStr(inStr);
}

// Getters
string Expression::getInfixStr(){
	return infixStr;
}

vector<string> Expression::getInfixVtr(){
	return infixVtr;
}

vector<string> Expression::getPostfixVtr(){
	return postfixVtr;
}

// Setters
void Expression::setInfixStr(string inStr){
	infixStr = inStr;
	transform(infixStr.begin(), infixStr.end(), infixStr.begin(), ::toupper);
	infixVtr = tokenize(infixStr, ST_TOKENS);
	computePostfixExpression();
}


/** This method compare two operators precedence.
  * It returns:
  *    1 if first operator has greater precedence
  *    0 if both operators has same precedence
  *   -1 if first operator has lower precedence
  *   -2 if op1 or op2 are not operators
  */
int Expression::cmpPrecedence(const string &op1, const string &op2){
	int idx1 = indexOf(op1, LST_OPERATORS, NR_OPERATORS);
	int idx2 = indexOf(op2, LST_OPERATORS, NR_OPERATORS);
	if(idx1 < 0 or idx2 < 0) return -2;
	return PRECEDENCES[idx1][idx2];
}

/** This method receives a string and a regular expression.
  * Its objective is to return a vector of all tokens
  * that match the regular expression.
  */
vector<string> Expression::tokenize(const string &str, string reg){
	// Variables definition
	vector<string> tokens;
	regex rgx (reg);
	sregex_token_iterator iter(str.begin(), str.end(), rgx, {/*-1, */0});
	sregex_token_iterator end;

	// Add all tokens to the vector of tokens
	while (iter != end)  {
		tokens.push_back(*iter);
		++iter;
	}

	// Return the vector of tokens
	return tokens;
}

/** This method receives a list of tokens for an infix expression
  * and transforms it into a postfix expression.
  */
void Expression::computePostfixExpression(){
	// Variables definition
	vector<string> infixTmp(infixVtr);
	stack<string> pendingOps;
	string nextOp;

	// Part 1: Set default variables
	// 1.1: Clear postfix expression in case it contains past information
	postfixVtr.clear();
	// 1.2: Add "(" to the pending operations stack
	pendingOps.push("(");
	// 1.3: Add ")" to the end of the expression
	infixTmp.push_back(")");

	// Part 2: For each token in the infix expression
	for(const string & token : infixTmp){
		// if token is an operand
		if(regex_match(token, REG_OPERAND)){
			postfixVtr.push_back(token);
		}
		// if token is "("
		else if(token == "("){
			pendingOps.push(token);
		}
		// if token is ")"
		else if(token == ")"){
			// Get next operation from our pending operations
			string nextOp = pendingOps.top();
			pendingOps.pop();
			// Send next operations to the expression until a "(" is found
			while(nextOp != "("){
				postfixVtr.push_back(nextOp);
				// get next operation
				nextOp = pendingOps.top();
				pendingOps.pop();
			}
		}
		// if token is an operator
		else if(regex_match(token, REG_OPERATOR)){
			// send all operations in our stack of next operations to our expression,
			// while they have greater or equal precedence than the current one
			while(!pendingOps.empty() and regex_match(pendingOps.top(), REG_OPERATOR) and cmpPrecedence(pendingOps.top(), token) >= 0){
				// add the top of pending operations to the expression and pop it
				postfixVtr.push_back(pendingOps.top());
				pendingOps.pop();
			}
			// add the current operation to the stack of pending operations
			pendingOps.push(token);
		}
	}
}

/** This method displays the expression in postfix mode */
void Expression::displayPostfix(){
	cout << "Postfix Expression: {";
	for(string s : postfixVtr){
		cout << " " << s ;
	}
	cout << "}" << endl << endl;
}
