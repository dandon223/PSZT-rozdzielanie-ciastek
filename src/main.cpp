#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include<tuple>
#include <cfloat>
#include <climits>

#include "EvolutionarySolution.hpp"
#include "IterativeSolution.hpp"
#include "TestGenerator.hpp"

using namespace std;

const int NUMBER_OF_TESTS = 150;

const std::string FILE_NAME_BASE = "./in/input";
const std::string FILE_NAME_BASE_EXTENSION = ".txt";
const int NUMBER_OF_VERSIONS = 3;

const int DEFAULT_WIELKOSC_POPULACJI = 100;
const int DEFAULT_LICZBA_GENERACJI = 10000;
const int DEFAULT_PRAWDOPODOBIENSTWO_MUTACJI = 5;
const int DEFAULT_WERSJA_MUTACJI = 1;
const int DEFAULT_PERIOD = 100000;
const int DEFAULT_TIMES = 30;

const int NUMBER_OF_MUTATION_FACTOR_TESTS = 10;
const int NUMBER_OF_POPULATION_TESTS = 7;

vector<int> czytaniePliku(string sciezka);
pair<long long, long long> parseResault(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, int result);

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
	cin.tie(NULL);

	//wartosci domyslne
	int wielkoscPopulacji = DEFAULT_WIELKOSC_POPULACJI;
	int liczbaGeneracji = DEFAULT_LICZBA_GENERACJI;
	double prawdopodobienstwoMutacji = DEFAULT_PRAWDOPODOBIENSTWO_MUTACJI;
	int wersjaMutacji = DEFAULT_WERSJA_MUTACJI;

	int period = DEFAULT_PERIOD;
	int times = DEFAULT_TIMES;

	vector<int>versions;
	
	//analiza flag
	for(int i = 1; i < argc; i+=2 )
	{
		if(i+1 < argc)
		{
			switch(argv[i][1])
			{
				case 'p': //period - okres zbieranie wynikow
					if(stof(argv[i+1]) <= 0)
					{
						cout << "okres musi byc wiekszy od 0\n";
						return 0;
					}
					else
					{
						period = stof(argv[i+1]);
					}
					break;
				case 't': //times - ile razy wykonujemy okres
					if(stoi(argv[i+1]) <= 0)
					{
						cout << "ilosc razy musi byc wiekszy od 0\n";
						return 0;
					}
					else
					{
						times = stoi(argv[i+1]);
					}
					break;
				case 'g': //liczba generacji
					liczbaGeneracji = stoi(argv[i+1]);
					break;
				case 'm': //prawdopodobienstwo mutacji
					prawdopodobienstwoMutacji = stof(argv[i+1]);
					break;
				case 'v': //wersja mutacji
					if(stoi(argv[i+1]) > NUMBER_OF_VERSIONS || stoi(argv[i+1]) < 0)
					{
						cout << "nie ma takiej wersji mutacji\n";
						return 0;
					}
					else
					{
						versions.push_back(stoi(argv[i+1]));
					}
					break;
			}
		}
		else
		{
			cout << "bledna liczba parametrow wykonania\n";
			return 0;
		}
	}

	//generujemy testy
	//TestGenerator::generateTests();

	//wywoloanie iteracyjne
	pair<long long, long long> iterationResults[NUMBER_OF_TESTS];
	for( int i = 0; i < NUMBER_OF_TESTS; ++i )
	{
		vector<int> oceny = czytaniePliku(FILE_NAME_BASE + to_string(i) + FILE_NAME_BASE_EXTENSION);

		IterativeSolution iSolution;
    	iSolution.setMarks(oceny);
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    	iSolution.runSolution();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		iterationResults[i] = parseResault(begin, end, iSolution.getResult());
	}

	/***************************************************
	 * rozwiazania ewolucyjne
	***************************************************/

	//miejsce na zapisywanie wynikow ewolucyjnych

	//dla kazdego pliku [] przechowujemy wektor roznych caseow testowania
	//w kazdym vectorze - vector wynikow
	vector< vector<MileStone> > results[NUMBER_OF_TESTS]; 
	
	//nazwy kolejnych plikow
	vector<string> fileNames;

	//labels
	vector<string> labels;

	for( int i = 0; i < NUMBER_OF_TESTS; ++i )
	{
		vector<int> oceny = czytaniePliku(FILE_NAME_BASE + to_string(i) + FILE_NAME_BASE_EXTENSION);

		int populationBase = 15;
		double propabilityBase = 0.3;
		int generationsBase = 500;

		for(int j = 1; j <= NUMBER_OF_MUTATION_FACTOR_TESTS; ++j )
		{
			for(int k = 1; k <= NUMBER_OF_POPULATION_TESTS; ++k)
			{
				for(int version : versions)
				{
					if( i == 0 ) // dodajemy kolejna nazwe pliku
					{
						fileNames.push_back("./out/outV" + to_string(version) + "m" + to_string(j* propabilityBase) + "p" + to_string(k*populationBase) + ".txt" );
						labels.push_back("versja nr " + to_string(version) + ";wspolczynnik mutacji " + to_string(j* propabilityBase) + 
						";wielkosc populacji " + to_string(k*populationBase) );
					
					}
					EvolutionarySolution eSolution( k * populationBase, generationsBase, j* propabilityBase);
					eSolution.setOceny(oceny);
					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
					eSolution.runSolution(version, 0, begin, period, times);
					results[i].push_back(eSolution.getMilestones());
				}
			}
		}
			
	}

	//wypisujemy rozwiazanie iteracyjne
	fstream itFile;
	itFile.open("./out/outIt.txt", ios::out);
	if(itFile.good())
	{
		itFile << "iteracyjne wynik\tczas\n";
		for(pair<long long, long long> p : iterationResults)
		{
			itFile << p.first << "\t" << p.second <<"\n";
		}
	}

	//wypisujemy rozwiazania ewolucyjne
	for(unsigned int i = 0; i < fileNames.size(); ++i )
	{
		fstream evFile;
		evFile.open(fileNames[i], ios::out);
		if(evFile.good())
		{
			evFile << labels[i] << "\n";
			for(int j = 0; j < NUMBER_OF_TESTS; ++j)
			{
				for(unsigned int k = 0; k < results[j][i].size(); ++k )
				{
					evFile << results[j][i][k].generations << "\t" << results[j][i][k].time << "\t" << results[j][i][k].result <<"\t";
				}
				evFile << "\n";
				
			}
		}
	}

}

vector<int> czytaniePliku(string sciezka)
{
    vector<int> v;
    ifstream file;
    file.open (sciezka);
    int word;
    while (file >> word)
    {
        v.push_back(word);
    }
    return v;
}

pair<long long, long long> parseResault(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, int result)
{
	return make_pair(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), result);
}