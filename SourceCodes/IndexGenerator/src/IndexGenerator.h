//
// 2019 Adapted from gonzaga 2016.
//

#ifndef INDEXGENERATOR_H_
#define INDEXGENERATOR_H_

#include <iostream>
#include <string>
#include <sstream>

#include <API/ArboretumAPI.h>
#include <sfp/simple_file_parser.h>

using namespace std;

class IndexGenerator
{
	public:
    	static void createIndex(string filename, int attributeIndex, string indexType);
};



#endif /* INDEXGENERATOR_H_ */
