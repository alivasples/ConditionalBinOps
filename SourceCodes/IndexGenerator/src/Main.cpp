//============================================================================
// Name        : IndexGenerator.cpp
// Author      : alivasples
// Version     : 2.0 (Adapted from V1.0 of Gonzaga 2016)
// Description : Index Generator for simple and complex data
//============================================================================

#include <iostream>

#include <IndexGenerator.h>

using namespace std;

// ./CreateIndex [RELATION] filename [ATTRIBUTE] attributeOrder [TYPE] simple/complex
int main(int argc, char* argv[])
{

	if(argc != 4) cout<<"Please follow the format below: \n./CreateIndex [RELATION] filename [ATTRIBUTE] attributeOrder [TYPE] simple/complex";
	else IndexGenerator::createIndex(argv[1], atoi(argv[2]), argv[3]);

    return 0;
}
