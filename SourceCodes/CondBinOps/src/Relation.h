/*
 *  Created by alivasples 2020 - December
 */

#ifndef RELATION_H_
#define RELATION_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <Generics.h>
#include <Attribute.h>
#include <IndexTree.h>
#include <sfp/simple_file_parser.h>

using namespace std;

class Relation{
	private:
		// ATTRIBUTES
		string name; // name of Relation
		string path; // path of Relation
		int nrTuples; // number of tuples in Relation
		map<string, int> attsPositions; // dictionary of attributes names to index positions in the Relation and vector
		vector<Attribute> attributes; // vector of attributes
		simple_file_parser *sfp; // file reader and parser
		bool isIndexed; // true if the Relation has index
		vector<IndexTree*> indexes; // vector of indexes for each attribute

		// PRIVATE METHODS
		/** method to read the header of the relation and load the list of attributes */
		void loadAttributes();

	public:
		// PUBLIC METHODS
		/** Default constructor */
		Relation(){}

		/** Parameterized constructor */
		Relation(string name, string path, bool hasIndex = false);

		/** Destructor */
		~Relation();

		/** Name getter */
		const string getName() const { return name; }

		/** Size getter */
		const int size() const { return nrTuples; }

		/** has index getter */
		const bool hasIndex() const { return isIndexed; }

		/** Go to next line (tuple) in Relation file */
		bool nextLine();

		/** This method returns the current line value of the parameter attribute */
		string getCurrentAttrValue(string attribute);

		/** This method returns the current tuple as a map */
		map<string,string> getCurrentTuple();

		/** This method returns the index query for a given attribute */
		bitset<MAX_TUPLES> indexQuery(string attribute, string cmp, string value);

		/** This method receives a bitset to show the tuples that are set to true
		 * Also, it returns the number of printed tuples */
		int displayTuples(bitset<MAX_TUPLES> ids);
};

// PRIVATE METHODS (HELPERS)
/** method to read the header of the relation and load the list of attributes */
void Relation::loadAttributes(){
	// ________ Just logging a trace ___________
	DEBUG_MSG("Loading attributes\n");

	// Variables definition
	string attName, attType;
	int attSize;

	sfp = new simple_file_parser();
	if(!sfp->open(path)) ERROR_MSG("Cannot read file: " << path);
	// Set the split tokens to read the header attributes
	sfp->set_single_char_tokens(";(),");
	// Read first line of document considering the tokens
	// Then we will have {"ATT1","(","string",",","10",")",";","ATT2","(","float",",","2",")",";"}
	sfp->get_next_line();
	// Save all attributes in our vector
	// Remember that we increment 7 because each attribute is composed by 7 tokens
	int nrAttributes = 0;
	for(int i = 0; i < (int)sfp->get_num_tokens(); i+= 7){
		// Load values
		attName = sfp->get_token(i);
		attType = sfp->get_token(i+2);
		attSize = sfp->get_token_uint(i+4);
		// just in case, convert the attribute name to upper case
		transform(attName.begin(), attName.end(), attName.begin(), ::toupper);
		// Add the new attribute (name, type, size)
		attributes.push_back(Attribute(attName, attType, attSize));
		// Also, map the attribute name to its index position in the relation or vector of attributes
		attsPositions[attName] = nrAttributes;
		// Finally create and add the index tree for the attribute (if we are in index mode)
		if(isIndexed){
			indexes.push_back(new IndexTree(path+"_"+attName, attType));
		}
		// Increment the number of attributes
		nrAttributes++;
	}
	// Set the split tokens for reading tuples
	sfp->set_single_char_tokens(";");

	// ________ Just logging a trace ___________
	DEBUG_MSG(nrAttributes << " Attributes Successfully Loaded \n");
}

// PUBLIC METHODS
/** Parameterized constructor */
Relation::Relation(string name, string path, bool hasIndex){
	// _____ Just writing a trace _______
	DEBUG_MSG("Instancing Relation " + name + "\n");
	// Setting attribute values
	this->name = name;
	this->path = path;
	this->nrTuples = Generics::linesInFile(path) - 1;
	this->isIndexed = hasIndex;
	loadAttributes();
	// _____ Just writing a trace _______
	DEBUG_MSG("Relation Instanced with " << nrTuples << " tuples\n\n");
}

/** Destructor */
Relation::~Relation(){
	// _____ Just writing a trace _______
	DEBUG_MSG("Destructing Relation " + name + " \n");

	while(!indexes.empty()){
		delete indexes.back();
		indexes.pop_back();
	}

	// _____ Just writing a trace _______
	DEBUG_MSG("Relation successfully destructed\n\n");
}

/** Go to next line (tuple) in Relation file */
bool Relation::nextLine(){
	// if before reading the file is ended
	if(sfp->eof()) return false;
	// read the next line
	sfp->get_next_line();
	// if this line is the end of the file
	if(sfp->eof()) return false;
	// if this is not the end of the file
	return true;
}

/** This method returns the current line value of the parameter attribute */
string Relation::getCurrentAttrValue(string attribute){
	if (attsPositions.find(attribute) == attsPositions.end()) ERROR_MSG(attribute + " does not exist in " + name);
	int pos = attsPositions[attribute];
	// We multiply by 2 because one token is the value and the next is the ";"
	return sfp->get_token(pos * 2);
}

/** This method returns the current tuple as a map */
map<string,string> Relation::getCurrentTuple(){
	map<string,string> tuple;
	for(const auto &attNameAndPosition : attsPositions){
		tuple[attNameAndPosition.first] = sfp->get_token(attNameAndPosition.second * 2);
	}
	return tuple;
}


/** This method returns the index query for a given attribute */
bitset<MAX_TUPLES> Relation::indexQuery(string attribute, string cmp, string value){
	// _____ Just writing a trace _______
	DEBUG_MSG("Index query(" << attribute << ", " << cmp << ", " << value << ")\n");

	if (attsPositions.find(attribute) == attsPositions.end()) ERROR_MSG(attribute + " does not exist in " + name);
	int pos = attsPositions[attribute];
	// Return the index query from the index tree
	return indexes[pos]->indexQuery(cmp, value);
}

/** This method receives a bitset to show the tuples that are set to true
 *  Also, it returns the number of valid tuples printed
 *  */
int Relation::displayTuples(bitset<MAX_TUPLES> ids){
	// variables definition
	int currId = 0;
	int nrValids = 0;
	string line;
	// Opening path
	ifstream input(path);
	// Read header
	getline(input, line);
	// Read line by line
	while(getline(input, line)){
		if(ids.test(currId)) {
			cout << line << endl;
			nrValids++;
		}
		if(line != "") currId ++;
	}
	// closing file
	input.close();
	// return the number of valid tuples
	return nrValids;
}

#endif /* RELATION_H_ */
