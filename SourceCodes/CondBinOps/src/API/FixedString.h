/*
 * FixedString.h
 * Created by alivasples 2019
 */

#ifndef FIXEDSTRING_H_
#define FIXEDSTRING_H_

#include <string>
#include <cstring>
using namespace std;

const int MAX_SIZE = 500;

/**
 * This fixed size string is created to support index creations for strings types.
 * This is because we need to know the size of a data type to store it in a file.
 * TODO: Search for an alternative if is possible, to store any length of string.
 * */
class FixedString{
	private:
		// ATTRIBUTES
		char data[MAX_SIZE];

	public:
		// METHODS
		/** Default Constructor */
		FixedString(){}

		/** Full Constructor */
		FixedString(string source){
			strcpy(data, source.c_str());
		}

		/** String Getter */
		string str(){
			return string(data);
		}

		/** Char array getter */
		const char* c_str(){
			return data;
		}

		// Comparators
		/** Equals comparator */
		bool operator==(const FixedString& other) const {return strcmp(data, other.data) == 0; }
		/** Lower comparator */
		bool operator<(const FixedString& other) const {return strcmp(data, other.data) < 0; }
		/** Lower or equal comparator */
		bool operator<=(const FixedString& other) const {return strcmp(data, other.data) <= 0; }
		/** Greater comparator */
		bool operator>(const FixedString& other) const {return strcmp(data, other.data) > 0; }
		/** Greater or equal comparator */
		bool operator>=(const FixedString& other) const {return strcmp(data, other.data) >= 0; }


};

#endif /* FIXEDSTRING_H_ */
