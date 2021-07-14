/*
 * Generics.h
 *
 *  Created on: 30 de dez de 2020
 *      Author: alivasples
 */

#ifndef GENERICS_H_
#define GENERICS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

#define ERROR_MSG(msg) {cerr << endl << "ERROR [" << __FILE__ << ", " << __FUNCTION__ << ", " << __LINE__ << "]: " << msg << endl; exit(1);}
#define DEBUG_MSG(msg) {if(Generics::isDebugMode) { cout << msg; }}
#define DEBUG_INSTR(instruction) {if(Generics::isDebugMode) { instruction; }}

class Generics{
public:
	// STATIC ATTRIBUTES
	static bool isDebugMode;
	static string savePath;

	// STATIC METHODS
	/** Read a file and returns the number of non-empty lines */
	static int linesInFile(const string &path){
		int nrLines = 0;
		string line;
		ifstream input(path);
		// validating that file exists
		if(!input.is_open()) ERROR_MSG("File " + path + " could not be opened.");
		// just counting next lines
		while(getline(input, line)){
			// Empty lines will not be counted
			if(line != "") nrLines++;
		}
		input.close();
		return nrLines;
	}

	/** Read a file of integers key - values and return it */
	static map<int, int> readIntKeyValues(const string &path){
		int key, val;
		map<int,int> keyVals;
		ifstream input(path);
		// validating that file exists
		if(!input.is_open()) ERROR_MSG("File " + path + " could not be opened.");
		// reading all pairs of values
		while(input >> key >> val){
			keyVals[key] = val;
		}
		input.close();
		return keyVals;
	}

	/** Save a message in a file */
	template <class DType>
	static void saveMessage(const DType & message, const string &extension = ""){
		std::ofstream ofs;
		ofs.open (savePath+"."+extension, std::ofstream::out | std::ofstream::app);
		ofs << message << endl;
		ofs.close();
	}

};


#endif /* GENERICS_H_ */
