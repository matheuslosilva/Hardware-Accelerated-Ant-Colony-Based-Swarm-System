#include "include/extern/rapidjson/document.h"
#include "include/extern/rapidjson/writer.h"
#include "include/extern/rapidjson/stringbuffer.h"
#include "include/extern/rapidjson/prettywriter.h" 

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <random>

using namespace rapidjson;
using namespace std;

int main (int argc, char** argv)
{
	srand(time(0));

	//	Parameters of the simulation
	vector<double> parameters(argc - 1);
	for(int i = 1; i < argc; i++)
		parameters[i - 1] = stod(argv[i]);

	//	Creating filePath
	char* filePath = (char *) malloc(50 * sizeof(char));
	strcpy(filePath, "src/swarmEnvironment/experiments/simulation.json");

	ifstream jsonFile;
	stringstream jsonStream;
	string jsonString;

	//	Reading JSON file
	jsonFile.exceptions (ifstream::failbit | ifstream::badbit);
	try
	{
	    jsonFile.open(filePath);
		jsonStream << jsonFile.rdbuf();
		jsonString = jsonStream.str();
		jsonFile.close();
	}
	catch (ifstream::failure& e)
    {
		cout << "ERROR::JSON_FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	//	Parsing JSON file
	Document document;
	document.Parse(jsonString.c_str());

	//	Updating ant parameters
	//document["randomSeed"] = rand();
	document["ants"][0]["velocity"] = parameters[0];
	//document["ants"][0]["placePheromoneIntensity"] = parameters[1];

	//	Stringify JSON
	StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    document.Accept(writer);

    //	Writing updates in JSON file
    string jsonFileString = sb.GetString();
    ofstream file;
    file.open(filePath);
    file << jsonFileString;
    file.close();

    free(filePath);

    return 0;
}
