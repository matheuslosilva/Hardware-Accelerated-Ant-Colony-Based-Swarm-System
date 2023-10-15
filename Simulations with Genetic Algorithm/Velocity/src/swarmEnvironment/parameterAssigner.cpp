#include <parameterAssigner.h>

ParameterAssigner::ParameterAssigner(const char* filePath)
{
	ifstream jsonFile;
	stringstream jsonStream;
	string jsonString;

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
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	Document document;
	document.Parse(jsonString.c_str());

    setGlobalSeed(document["randomSeed"].GetInt());
    //srand(GLOBAL_SEED);
    //A DEFINIÇÃO DO DATA_SIZE ACONTECE ANTES DAQUI NO .H, PRECISA RESOLVER ISSO
    //setScrHeight(2000);
    //setScrWidth(2000);

	environmentParameters.placePheromoneRate = document["environment"]["placePheromoneRate"].GetInt();
   	environmentParameters.pheromoneEvaporationRate = document["environment"]["pheromoneEvaporationRate"].GetInt();
 
   	AnthillParameters* anthillParameterss = (AnthillParameters*) malloc(sizeof(AnthillParameters));
	anthillParameterss->posX = document["anthills"][0]["posX"].GetDouble();
    anthillParameterss->posY = document["anthills"][0]["posY"].GetDouble();
    anthillParameterss->size = document["anthills"][0]["size"].GetDouble();
    anthillParameterss->antEspecification = document["anthills"][0]["antEspecifications"][0].GetInt();
    anthillParameterss->antAmount = document["anthills"][0]["antAmounts"][0].GetInt();
    this->anthillParameters.push_back(anthillParameterss);

    FoodSourceParameters* foodParameterss = (FoodSourceParameters*) malloc(sizeof(FoodSourceParameters));
	foodParameterss->posX = document["foodSources"][0]["posX"].GetDouble();
	foodParameterss->posY = document["foodSources"][0]["posY"].GetDouble();
	foodParameterss->size = document["foodSources"][0]["size"].GetDouble();
	foodParameterss->foodAmount = document["foodSources"][0]["foodAmount"].GetInt();
	this->foodParameters.push_back(foodParameterss);

	FoodSourceParameters* foodParametersss = (FoodSourceParameters*) malloc(sizeof(FoodSourceParameters));
	foodParametersss->posX = document["foodSources"][1]["posX"].GetDouble();
	foodParametersss->posY = document["foodSources"][1]["posY"].GetDouble();
	foodParametersss->size = document["foodSources"][1]["size"].GetDouble();
	foodParametersss->foodAmount = document["foodSources"][1]["foodAmount"].GetInt();
	this->foodParameters.push_back(foodParametersss);

	FoodSourceParameters* foodParameterssss = (FoodSourceParameters*) malloc(sizeof(FoodSourceParameters));
	foodParameterssss->posX = document["foodSources"][2]["posX"].GetDouble();
	foodParameterssss->posY = document["foodSources"][2]["posY"].GetDouble();
	foodParameterssss->size = document["foodSources"][2]["size"].GetDouble();
	foodParameterssss->foodAmount = document["foodSources"][2]["foodAmount"].GetInt();
	this->foodParameters.push_back(foodParameterssss);

	FoodSourceParameters* foodParametersssss = (FoodSourceParameters*) malloc(sizeof(FoodSourceParameters));
	foodParametersssss->posX = document["foodSources"][3]["posX"].GetDouble();
	foodParametersssss->posY = document["foodSources"][3]["posY"].GetDouble();
	foodParametersssss->size = document["foodSources"][3]["size"].GetDouble();
	foodParametersssss->foodAmount = document["foodSources"][3]["foodAmount"].GetInt();
	this->foodParameters.push_back(foodParametersssss);


	AntSensorParameters* antSensorParameters = (AntSensorParameters*) malloc(sizeof(AntSensorParameters));
	antSensorParameters->xCenterAntDistance = 0.004; //document["ants"][0]["antSensorParameters"].GetDouble();
	antSensorParameters->yCenterAntDistance = 0.004;
	antSensorParameters->positionAngle = -45;
	antSensorParameters->sensorPixelRadius = 2;
	antSensorParameters->sensorType = PHEROMONE;

	AntSensorParameters* antSensorParameters2 = (AntSensorParameters*) malloc(sizeof(AntSensorParameters));
	antSensorParameters2->xCenterAntDistance = 0.004;
	antSensorParameters2->yCenterAntDistance = 0.004;
	antSensorParameters2->positionAngle = 45;
	antSensorParameters2->sensorPixelRadius = 2;
	antSensorParameters2->sensorType = PHEROMONE;

	AntParameters* antParameters = (AntParameters*) malloc(sizeof(AntParameters));
	antParameters->nestID = 0;

	antParameters->size = 0.002;
	antParameters->velocity = document["ants"][0]["velocity"].GetDouble();

	antParameters->state = EXPLORER;
	antParameters->pheromoneType = RED;
	antParameters->placePheromoneIntensity = 60;
	antParameters->lifeTime = 1250;
	antParameters->viewFrequency = 1;

	antParameters->antSensorParameters = antSensorParameters;
	antParameters->antSensorParameters2 = antSensorParameters2;

	this->antParameters.push_back(antParameters);
}

/*void ParameterAssigner::setAnthill()
{

}

void ParameterAssigner::setFoodSource()
{

}

void ParameterAssigner::setAnt()
{

}
*/