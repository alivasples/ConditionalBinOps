//
// Created by gonzaga on 1/20/16.
// Updated by alivasples on 10/26/19.
//

#include <IndexGenerator.h>

/*
 * INPUT EXAMPLE
 * ATT1(float,2);ATT2(float,1);ATT3(string,10);
 * 0.292,0.014;0.202;aaa;
 */

using namespace std;


void IndexGenerator::createIndex(string filename, int attributeIndex, string indexType)
{
	// Just a file parser
    simple_file_parser sfp;
    // Vars definition
    string attributeName; // name of attribute we want to index
    string attributeType; // type of attribute we want to index
    int attributeSize; // how many numbers describe the attribute

	// Just opening the file
    sfp.open(filename);
    // Validate if file exists
    sfp.file_exists(filename);

    // Set the tokens separators
    sfp.set_single_char_tokens("(), ;");
    // Read the header line
    sfp.get_next_line();

    // Parse the attribute
    attributeName = sfp.get_token((attributeIndex * 7)); // Example: ATT2
    attributeType = sfp.get_token((attributeIndex * 7) + 2); // Example: float
    attributeSize = sfp.get_token_int((attributeIndex * 7) + 4); // Example: 2 (float coords)
    // convert attribute name to caps
    transform(attributeName.begin(), attributeName.end(), attributeName.begin(), ::toupper);
    // Just showing a message
    cout<<"Creating Index for "<<attributeName<<" of tpye "<<attributeType<<" and size "<<attributeSize<<endl;

    //remove slim tree if already exists
    remove((filename+"_"+attributeName).c_str());

    //create slim tree file. For example file T1.data_ATT2
    stPlainDiskPageManager *pageManager = new stPlainDiskPageManager((filename+"_"+attributeName).c_str(),4096);

    stFloatArrayTree *stFloat = NULL;
    stCharArrayTree *stChar = NULL;
    stGenericMetricTree *stMetric = NULL;
    bpIntTree *bpInt = NULL;
    bpFloatTree *bpFloat = NULL;
    bpStringTree *bpString = NULL;

    //if metric type
    if(attributeSize == -1)
    {
        stMetric = new stGenericMetricTree(pageManager);
    }
    else if(indexType == "complex" and attributeType == "float")
    {
        stFloat = new stFloatArrayTree(pageManager);
    }
    else if(indexType == "complex")
    {
        stChar = new stCharArrayTree(pageManager);
    }
    else if(indexType == "simple" and attributeType == "int")
    {
    	bpInt = new bpIntTree(pageManager, true);
    }
    else if(indexType == "simple" and attributeType == "float")
	{
    	bpFloat = new bpFloatTree(pageManager, true);
	}
    else
	{
    	bpString = new bpStringTree(pageManager, true);
	}

    // =============================== ADDING THE CONTENT TO THE TREE ============================
    //read values and populate index
    sfp.set_single_char_tokens(";");
    string values;

    int rowId = 0;

    while(sfp.get_next_line())
    {
    	//temp values
        float f[attributeSize];
        char *c;

    	// read the current value of tuple[attribute]
    	values = sfp.get_token(attributeIndex*2);

        if(attributeSize == -1)
        {

        }
        else if(indexType == "complex" and attributeType.compare("float") == 0)
        {
            stFloatArrayObject *tmpFloat;
            // alivasples for last arboretum
            vector<float> floatValues;


            //parse values
            std::istringstream iss(values);
            std::string token;
            int i = 0;
            while (std::getline(iss, token, ','))
            {
                f[i] = atof(token.c_str());
                floatValues.push_back(f[i]);
                i++;
            }
            /*
            tmpFloat = new stFloatArrayObject(attributeSize,f);
            tmpFloat->SetOID(rowId);
             */
            tmpFloat = new stFloatArrayObject(rowId,floatValues);

            stFloat->Add(tmpFloat);
        }
        else if(indexType == "complex")
        {
//            stCharArrayObject *tmpChar;
//            // Last arboretum
//            tmpChar = new stCharArrayObject(rowId,vector<char>(values.begin(),values.end()));
//            stChar->Add(tmpChar);
//            if(values=="BLUEBERRY" or values=="Blueberry" or values=="blueberry"){
//            				cout<<values<<" here at line "<<rowId<<endl;
//            }
        }
        else if(indexType == "simple" and attributeType == "int")
		{
        	// The value cast
        	int value = atoi(values.c_str());
        	// Insertion
			bpInt->Insert(value, new stIntArrayObject(rowId, vector<int>()));
		}
		else if(indexType == "simple" and attributeType == "float")
		{
			// The value cast
			float value = atof(values.c_str());
			// Insertion
			bpFloat->Insert(value,new stIntArrayObject(rowId, vector<int>()));
		}
		else
		{
			// Insertion
			bpString->Insert(FixedString(values), new stIntArrayObject(rowId, vector<int>()));
		}
        rowId++;
    }

    // CLEANING SPACE
	if(stMetric!=NULL){
		// Just showing a message for depuration
		cout<<"Slim Tree Successfully Created With:"<<endl;
		cout<<"* "<<stMetric->GetNumberOfObjects()<<" metric objects "<<endl;
		// Cleaning
		delete stMetric;
		stMetric = NULL;
	}

    if(stFloat!=NULL){
    	// Just showing a message for depuration
		cout<<"Slim Tree Successfully Created With:"<<endl;
    	cout<<"* "<<stFloat->GetNumberOfObjects()<<" float objects "<<endl;
    	// Cleaning
    	delete stFloat;
    	stFloat = NULL;
    }

    if(stChar!=NULL){
        	// Just showing a message for depuration
    		cout<<"Slim Tree Successfully Created With:"<<endl;
        	cout<<"* "<<stChar->GetNumberOfObjects()<<" char objects "<<endl;
        	// Cleaning
        	delete stChar;
        	stChar = NULL;
        }

    if(bpInt!=NULL){
    	// Just showing a message for depuration
		cout<<"B+ Tree Successfully Created With:"<<endl;
		cout<<"* "<<bpInt->GetNumberOfObjects()<<" int objects "<<endl;
		// Cleaning
    	delete bpInt;
    	bpInt = NULL;
    }

    if(bpFloat!=NULL){
    	// Just showing a message for depuration
		cout<<"B+ Tree Successfully Created With:"<<endl;
		cout<<"* "<<bpFloat->GetNumberOfObjects()<<" float objects "<<endl;
    	// Cleaning
    	delete bpFloat;
    	bpFloat = NULL;

    }

    if(bpString!=NULL){
    	// Just showing a message for depuration
		cout<<"B+ Tree Successfully Created With:"<<endl;
		cout<<"* "<<bpString->GetNumberOfObjects()<<" string objects "<<endl;
		// Cleaning
    	delete bpString;
    	bpString = NULL;
    }

    if(pageManager!=NULL){
    	delete pageManager;
    	pageManager = NULL;

    	/*
    	stPlainDiskPageManager* pageManager2 = new stPlainDiskPageManager((filename+"_"+attributeName).c_str());
    	bpStringTree* bpString2 = new bpStringTree(pageManager2);
    	cout<<"De novo2: "<<bpString2->GetNumberOfObjects()<<endl;
    	cout<<"Entries2: "<<bpString2->QueryEqual(str)->GetNumOfEntries()<<endl;
    	*/

    }
}
