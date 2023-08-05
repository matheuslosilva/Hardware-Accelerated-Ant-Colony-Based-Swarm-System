#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>
#include <time.h>
#include <chrono>
#include <climits>
#include <math.h>
#include <random>
#include <iomanip>
#include <queue>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

double TAX_OF_MUTATION = 0.05; // In percentage
#define POPULATION_SIZE 30
#define MAX_GENERATIONS 1000
#define GENS_TO_BALANCE 30
#define NUM_OF_GENES 8
#define NUM_OF_GENERATIONS_FOR_FITNESS_MEAN 1

//	Returns a random integer between INT_MIN and INT_MAX
int randomInt(int lowerBound = INT_MIN, int upperBound = INT_MAX)
{
	return uniform_int_distribution<int>(lowerBound, upperBound)(rng);
}

//	Returns a random number between 0 and 1
float randomPercentage()
{
	return ((double) randomInt() + INT_MAX) / ((double) INT_MAX - INT_MIN); 
}

//	Returns a random sign
int randomSign()
{
	return (randomPercentage() < 0.5) ? -1 : 1;
}

class Individual
{

public : 
	
	//	gene[i] belongs to the interval genesLowerBound[i] to genesUpperBound[i]
	vector<float> genes;
	vector<float> genesLowerBound;
	vector<float> genesUpperBound;
	queue<int> lattestFitnesses;
	int sumOfFitness;

	Individual(	vector<float> _genes = vector<float>(0),
				vector<float> _genesLowerBound = vector<float>(0),
				vector<float> _genesUpperBound = vector<float>(0)) 
	{
		genes = _genes;
		genesLowerBound = _genesLowerBound;
		genesUpperBound = _genesUpperBound;
		lattestFitnesses = queue<int>();
		sumOfFitness = 0;
	}

	void calculateFitnessScore()
	{
		FILE *fpipe;
		char answer[256];

		//	Running Hardware-Accelerated-Ant-Colony-Based simulator
		string command = "./main";
		for(float gene : genes)
			command += " " + to_string(gene);
		if(!(fpipe = (FILE*) popen(command.c_str(), "r")))
		{
			cerr << "Failed to read pipe";
			exit(1);
		}

		//	Reading the return of the simulation 
		fgets(answer, sizeof(answer), fpipe);
		pclose(fpipe);

		int fitness = stof(answer);

		//	Updating the sum of fitness scores of the lattest generations
		if(!lattestFitnesses.empty())
			sumOfFitness -= lattestFitnesses.front();
		sumOfFitness += fitness;

		//	Updating the fitness scores of the lattest generations
		if(!lattestFitnesses.empty())
			lattestFitnesses.pop();
		lattestFitnesses.push(fitness);
	}

	int getLattestFitnessScore()
	{
		if(lattestFitnesses.empty())
			calculateFitnessScore();

		return lattestFitnesses.back();
	}

	float getMeanFitnessScore()
	{
		if(lattestFitnesses.empty())
			calculateFitnessScore();

		return (float) sumOfFitness / lattestFitnesses.size();
	}

	//	Returns the indexes of the genes that need to mutate
	vector<int> getGenesIndexesToMutate()
	{
		int numberOfIndexes = 0;
		float randPercentage = randomPercentage();

		if (randPercentage < 0.7)
			numberOfIndexes = 1;
		else if (randPercentage < 0.95)
			numberOfIndexes = 2;
		else
			numberOfIndexes = 3;

		numberOfIndexes = min(numberOfIndexes, (int) genes.size());

		vector<int> indexes((int) genes.size());
		for(int i = 0; i < (int) genes.size(); i++)
			indexes[i] = i;
		random_shuffle(indexes.begin(), indexes.end());

		while((int) indexes.size() > numberOfIndexes)
			indexes.pop_back();

		return indexes;
	}

	//	Method that mutates genes of the individual
	void mutateGenes()
	{
		vector<int> indexes = getGenesIndexesToMutate();

		for(int i : indexes)
		{
			float lw = genesLowerBound[i];
			float up = genesUpperBound[i];
			genes[i] += (double) randomSign() * randomPercentage() * TAX_OF_MUTATION * (up - lw);
			genes[i] = min(genes[i], up);
			genes[i] = max(genes[i], lw);
		}
	}

	//	Operator that simulates the crossover between two individuals
	Individual operator + (Individual other)
	{
		Individual child = (*this);

		for(int i = 0; i < (int) child.genes.size(); i++)
			child.genes[i] = (((*this).genes[i] + other.genes[i]) / 2.0);

		return child;
	}
};

class GeneticAlgorithm
{

public : 

	vector<Individual> individuals;

	GeneticAlgorithm(vector<Individual> _individuals = vector<Individual>(0))
	{
		individuals = _individuals;
	}

	void initializePopulation(int populationSize, Individual base)
	{
		individuals.clear();
		individuals.resize(populationSize);
		for(int i = 0; i < populationSize; i++)
		{
			individuals[i] = base;
			individuals[i].mutateGenes();

			for(int j = 0; j < (int) base.genes.size(); j++)
			{
				float lw = base.genesLowerBound[j];
				float up = base.genesUpperBound[j];
				double gene = (double) randomPercentage() * (up - lw);
				individuals[i].genes[j] = gene;
				individuals[i].genes[j] = min(individuals[i].genes[j], up);
				individuals[i].genes[j] = max(individuals[i].genes[j], lw);
			}
		}
	}

	//	Return the current index of the best individual
	int getBestIndividualIndex()
	{
		int bestIndex = 0;
		
		for(int i = 1; i < (int) individuals.size(); i++)
			if(individuals[bestIndex].getMeanFitnessScore() < individuals[i].getMeanFitnessScore())
				bestIndex = i;

		return bestIndex;
	}

	//	Best individual becomes the first individual of the vector
	void realocateBestIndividual()
	{
		int index = getBestIndividualIndex();
		swap(individuals[0], individuals[index]);
	}

	void elitism()
	{
		int bestIndex = getBestIndividualIndex();

		for(int i = 0; i < (int) individuals.size(); i++)
		{
			//	Crossing the i-th individual with the best individual
			if(i != bestIndex && individuals[i].getLattestFitnessScore() < individuals[bestIndex].getLattestFitnessScore())
			{
				//	Crossover
				individuals[i] = (individuals[i] + individuals[bestIndex]);
				
				//	Mutation
				individuals[i].mutateGenes();
			}

			individuals[i].calculateFitnessScore();
		}
	}

	void tournamentOfTwo()
	{
		int bestIndex = getBestIndividualIndex();

		for(int i = 0; i < (int) individuals.size(); i++)
		{
			if(i != bestIndex)
			{
				int id1 = randomInt(0, (int) individuals.size() - 1);
				int id2 = randomInt(0, (int) individuals.size() - 1);

				while(id1 == id2)
					id2 = randomInt(0, (int) individuals.size() - 1);
				
				int father = (individuals[id1].getLattestFitnessScore() > individuals[id2].getLattestFitnessScore()) ? id1 : id2;

				id1 = randomInt(0, (int) individuals.size() - 1);
				id2 = randomInt(0, (int) individuals.size() - 1);

				while(id1 == id2)
					id2 = randomInt(0, (int) individuals.size() - 1);

				int mother = (individuals[id1].getLattestFitnessScore() > individuals[id2].getLattestFitnessScore()) ? id1 : id2;
				
				//	Crossover
				individuals[i] = (individuals[father] + individuals[mother]);
				
				//	Mutation
				individuals[i].mutateGenes();
			}

			individuals[i].calculateFitnessScore();
		}
	}
};

void writeIndividualToFile(ofstream &myfile, int generation, Individual &ind)
{
	myfile << fixed;
	myfile << generation << ",";

	for(float gene : ind.genes)
		myfile << fixed << gene << ",";

	myfile << ind.getLattestFitnessScore() << endl;
}

void showIndividualGenes(int generation, Individual &ind)
{
	cout << fixed;
	cout << generation << ",";

	for(float gene : ind.genes)
		cout << fixed << gene << ",";

	cout << ind.getLattestFitnessScore() << endl;
}

int main ()
{
	int experiment = 0;
	cout << "Insert the number of the experiment : ";
	cin >> experiment;

	//	Compiling Hardware-Accelerated-Ant-Colony-Based simulator
	system("make all");

	//	Creating files to store the experiment
	system("mkdir Experiments");
	string csvFilePath = "Experiments/" + to_string(experiment);
	string createFileCommand = "mkdir " + csvFilePath;
	system(createFileCommand.c_str());
	ofstream myfile;
	myfile.open(csvFilePath + "/evolution.csv");
	
	// ----------- Declaring base individual --------- //
	myfile << "Generation,"; 
	Individual base;

	base.genes.resize(NUM_OF_GENES);
	base.genesLowerBound.resize(NUM_OF_GENES);
	base.genesUpperBound.resize(NUM_OF_GENES);

	//	Velocity Gene
	// base.genes[0] = 0.0004;
	// base.genesLowerBound[0] = 0.0002;
	// base.genesUpperBound[0] = 0.002;

	//	RedPheromoneIntensity Gene
	myfile << "RedPheromoneIntensity,"; 
	base.genes[0] = 60;
	base.genesLowerBound[0] = 0;
	base.genesUpperBound[0] = 255;

	//	GreenPheromoneIntensity Gene
	myfile << "GreenPheromoneIntensity,"; 
	base.genes[1] = 60;
	base.genesLowerBound[1] = 0;
	base.genesUpperBound[1] = 255;

	//	TIMEOUT Gene
	myfile << "TIMEOUT,"; 
	base.genes[2] = 400;
	base.genesLowerBound[2] = 0;
	base.genesUpperBound[2] = 10000;

	//	PHEROMONE_EVAPORATION Gene
	myfile << "PHEROMONE_EVAPORATION,";
	base.genes[3] = 15;
	base.genesLowerBound[3] = 1;
	base.genesUpperBound[3] = 100;

	//	CHANGE_TO_EXPLORER Gene
	myfile << "CHANGE_TO_EXPLORER,";
	base.genes[4] = 21475;
	base.genesLowerBound[4] = 0;
	base.genesUpperBound[4] = INT_MAX;

	//	CHANGE_TO_CARRIER Gene
	myfile << "CHANGE_TO_CARRIER,";
	base.genes[5] = 21475;
	base.genesLowerBound[5] = 0;
	base.genesUpperBound[5] = INT_MAX;

	//	CHANGE_TO_NESTCARRIER Gene
	myfile << "CHANGE_TO_NESTCARRIER,";
	base.genes[6] = 21475;
	base.genesLowerBound[6] = 0;
	base.genesUpperBound[6] = INT_MAX;

	//	CHANGE_TO_FOLLOWGREEN Gene
	myfile << "CHANGE_TO_FOLLOWGREEN,";
	base.genes[7] = 21475;
	base.genesLowerBound[7] = 0;
	base.genesUpperBound[7] = INT_MAX;
	// ----------------------------------------------- //

	myfile << "Fitness Score" << endl;

	GeneticAlgorithm population;
	population.initializePopulation(POPULATION_SIZE, base);

	vector<float> bestGenes(NUM_OF_GENES, 0);
	int flag = 0;

	for(int generation = 0; generation < MAX_GENERATIONS; generation++)
	{
		cout << "GENERATION " << generation << " TAX_OF_MUTATION " << TAX_OF_MUTATION << " flag " << flag << endl;

		int bestIndex = population.getBestIndividualIndex();
		writeIndividualToFile(myfile, generation, population.individuals[bestIndex]);
		showIndividualGenes(generation, population.individuals[bestIndex]);

		for(int i = 0; i < (int) population.individuals.size(); i++)
		{
			if(i != bestIndex)
			{
				writeIndividualToFile(myfile, generation, population.individuals[i]);
				showIndividualGenes(generation, population.individuals[i]);
			}
		}

		if(bestGenes == population.individuals[bestIndex].genes)
			flag++;
		else
		{
			flag = 0;
			TAX_OF_MUTATION = 0.05;
			bestGenes = population.individuals[bestIndex].genes;
		}

		if(flag == 5)
			TAX_OF_MUTATION *= 2.0;
		else if(flag == 10)
			TAX_OF_MUTATION *= 2.0;
		else if(flag == 15)
			TAX_OF_MUTATION *= 2.0;
		else if(flag == 20)
			TAX_OF_MUTATION *= 2.0;

		if(flag >= GENS_TO_BALANCE)
			break;

		//population.elitism();
		population.tournamentOfTwo();
	}

	myfile.close();

	if (flag == GENS_TO_BALANCE)
		cout << "Ended by estabilization." << endl;
	else 
		cout << "Ended by max generations."  << endl;

	cout << "The best individual has the following genes:" << endl;
	for(float gene : bestGenes)
		cout << gene << " ";
	cout << endl;

	return 0;
}