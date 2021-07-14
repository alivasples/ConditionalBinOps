/*
 *  Created by alivasples 2020 - December
 */

#ifndef INDEXTREE_H_
#define INDEXTREE_H_

#include <iostream>
#include <string>
#include <bitset>

#include <Generics.h>
#include <API/ArboretumAPI.h>

using namespace std;

enum DATA_TYPE{INTEGER, FLOAT, STRING};
const int MAX_TUPLES = 100000;

/** ==================================================== Index Tree Class =============================================== */
class IndexTree{
	private:
		// ATTRIBUTES
		DATA_TYPE type; // INTEGER, FLOAT, STRING
		bool isComplex = false; // 0 = simple, 1 = complex

		// Page manager for reading the tree from a file
		stPlainDiskPageManager *pageManager;

		// From the next groups attributes, only one will be used, depending on the type of the tree
		// B+ tree attributes - float
		bpFloatTree *simpleFloatTree = nullptr;
		bpFloatResult *simpleFloatResult = nullptr;
		// B+ tree attributes - strings
		bpStringTree *simpleStringTree = nullptr;
		bpStringResult *simpleStringResult = nullptr;
		// Slim tree attributes - float
		stFloatArrayTree *complexFloatTree = nullptr;
		myFloatResult *complexFloatResult = nullptr;

	public:
		// METHODS
		/** Constructor */
		IndexTree(string path, string type, bool isComplex);

		/** Destructor */
		~IndexTree();

		/** indexQuery method:
		 * This method will receive an operator and a constant.
		 * It will return then a bitset with true values for all tuples
		 * of T1 or T2 that satisfy the condition.
		 * @param operator: <, >, <=, >=, =
		 * @param constant: a string that can be casted to float if current tree is of floats
		 * @return a bitset with true values for tuples that satisfy the condition
		 * */
		bitset<MAX_TUPLES> indexQuery(const int& cmp, const string& strOperand);
		bitset<MAX_TUPLES> indexQuery(const string& cmp, const string& strOperand);
};

/** Index Tree constructor. */
IndexTree::IndexTree(string path, string tp, bool isComplex = false){
	// ___ Just a trace message ____
	DEBUG_MSG("Loading Index Tree ");

	// Basic assignment
	transform(tp.begin(), tp.end(), tp.begin(), ::toupper);
	this->type = tp == "INTEGER" ? INTEGER : tp == "FLOAT" ? FLOAT : STRING;
	this->isComplex = isComplex;

	// Create a page manager for the tree file
	try{
		pageManager = new stPlainDiskPageManager(path.c_str());
	}
	catch (...) {
		ERROR_MSG("File '" + path + "' could not be opened to load the index");
		exit(0);
	}

	// Creating the tree according to the complexity and type
	if(isComplex){
		if(type == FLOAT){
			complexFloatTree = new stFloatArrayTree(pageManager);
			// ___ Just a trace message ____
			DEBUG_MSG("(Complex Float Tree)\n");
		}
		else{
			ERROR_MSG("Data type not supported for complex tree.");
		}
	}
	else{
		if (type == FLOAT){
			simpleFloatTree = new bpFloatTree(pageManager);
			// ___ Just a trace message ____
			DEBUG_MSG("(Simple Float Tree)\n");
		}
		else if(type == STRING){
			simpleStringTree = new bpStringTree(pageManager);
			// ___ Just a trace message ____
			DEBUG_MSG("(Simple String Tree)\n");
		}
		else{
			ERROR_MSG("Data type not supported for simple tree.");
		}
	}

	// ___ Just a trace message ____
	DEBUG_MSG("Index Tree Loaded\n\n");
}

/** Destructor */
IndexTree::~IndexTree(){
	// ___ Just a trace message ____
	DEBUG_MSG("Destructing Index Tree\n");

	// Delete the simple float tree
	if(simpleFloatTree != nullptr){
		delete simpleFloatTree;
		simpleFloatTree = nullptr;
	}
	// Delete the simple string tree
	if(simpleStringTree != nullptr){
		delete simpleStringTree;
		simpleStringTree = nullptr;
	}
	// Delete the complex float tree
	if(complexFloatTree != nullptr){
		delete complexFloatTree;
		complexFloatTree = nullptr;
	}
	// Delete the page manager
	if(pageManager != nullptr){
		delete pageManager;
		pageManager = nullptr;
	}

	// ___ Just a trace message ____
	DEBUG_MSG("Index Tree Destructed\n\n");
}


/** indexQuery method:
 * This method will receive an operator and a constant.
 * It will return then a bitset with true values for all tuples
 * of T1 or T2 that satisfy the condition.
 * @param operator: <, >, <=, >=, =
 * @param constant: a string that can be casted to float if current tree is of floats
 * @return a bitset with true values for tuples that satisfy the condition
 * */
bitset<MAX_TUPLES> IndexTree::indexQuery(const int& cmp, const string& strOperand){
	// ___ Just a trace message ____
	DEBUG_MSG("Performing Index Query\n");

	// Variables definition
	bitset<MAX_TUPLES> resultIds;

	// If complex tree
	if(isComplex){
		ERROR_MSG("***TO DO***: Complex Queries not supported yet.");
	}
	// If simple tree
	else{
		// If our tree is a simple float tree
		if(type == FLOAT){
			// Convert the given string to a float value and query according to the logical operator (comparison)
			float floatOperand = stof(strOperand);
			switch(cmp){
				case EQ: simpleFloatResult = simpleFloatTree->QueryEqual(floatOperand); break;
				case LT: simpleFloatResult = simpleFloatTree->QueryLessThan(floatOperand); break;
				case LEQ: simpleFloatResult = simpleFloatTree->QueryLessThanOrEqual(floatOperand); break;
				case GT: simpleFloatResult = simpleFloatTree->QueryGreaterThan(floatOperand); break;
				case GEQ: simpleFloatResult = simpleFloatTree->QueryGreaterThanOrEqual(floatOperand); break;
				default: ERROR_MSG("Comparator Not supported for simple float tree.");
			}
			// Set all ids of the valid answers as true into the bitset answer
			for(int i = 0; i < (int) simpleFloatResult->GetNumOfEntries(); i++){
				int idValid = simpleFloatResult->GetPair(i)->GetObject()->GetOID();
				resultIds.set(idValid);
			}
		}
		// If our tree is a simple string tree
		else if(type == STRING){
			switch(cmp){
				// Convert the given string to a float value and query according to the logical operator (comparison)
				case EQ: simpleStringResult = simpleStringTree->QueryEqual(FixedString(strOperand)); break;
				default: ERROR_MSG("Comparator Not supported for simple string tree.");
			}
			// Set all ids of the valid answers as true into the bitset answer
			for(int i = 0; i < (int) simpleStringResult->GetNumOfEntries(); i++){
				int idValid = simpleStringResult->GetPair(i)->GetObject()->GetOID();
				resultIds.set(idValid);
			}
		}
		else{
			ERROR_MSG("Simple types different from Floats or Strings are not supported.");
		}
	}

	// ___ Just a trace message ____
	DEBUG_MSG("Index Query Performed with results: {" << resultIds << "}\n\n");

	// Return the results
	return resultIds;
}

/** indexQuery method:
 * This method will receive an operator and a constant.
 * It will return then a bitset with true values for all tuples
 * of T1 or T2 that satisfy the condition.
 * @param operator: <, >, <=, >=, =
 * @param constant: a string that can be casted to float if current tree is of floats
 * @return a bitset with true values for tuples that satisfy the condition
 * */
bitset<MAX_TUPLES> IndexTree::indexQuery(const string& cmp, const string& strOperand){
	return indexQuery(indexOf(cmp, LST_OPERATORS, NR_OPERATORS), strOperand);
}

#endif /* INDEXTREE_H_ */
