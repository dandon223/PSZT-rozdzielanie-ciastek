#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include <tuple>
#include <cfloat>
#include <climits>

#include "EvolutionarySolution.hpp"
#include "IterativeSolution.hpp"
#include "TestGenerator.hpp"

using namespace std;

const int NUMBER_OF_TESTS = 15;
const int NUMBER_OF_SEEDS = 5;

const std::string FILE_NAME_BASE = "./in/input";
const std::string FILE_NAME_BASE_EXTENSION = ".txt";
const int NUMBER_OF_VERSIONS = 3;

const int DEFAULT_POPULATION_SIZE = 15;
const double DEFAULT_MUTATION_FACTOR = 0.05;
const int DEFAULT_MUTATION_VERSION = 1;
const int DEFAULT_PERIOD = 10000;
const int DEFAULT_TIMES = 35;

const int NUMBER_OF_MUTATION_FACTOR_TESTS = 8;
const int NUMBER_OF_POPULATION_TESTS = 3;

//najlepsze parametry
const int BEST_VERSION = 1;
const double BEST_MUTATION_FACTOR = 1.6;
const int BEST_POPULATION_SIZE = 5;

void tests(int times, int propabilityBase, int populationBase, vector<int>versions);
void run();

vector<int> readFile(string fileName);
vector<int> readStdin();
pair<long long, long long> parseResault(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, int result);

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
	cin.tie(NULL);

	int times = DEFAULT_TIMES;
	int propabilityBase = DEFAULT_MUTATION_FACTOR;
	int populationBase = DEFAULT_POPULATION_SIZE;

	vector<int>versions;

	bool gMode = false; //czy uruchomic tryb generowania testow
	bool tMode = false; //czy uruchomic tryb testowania
	bool rMode = false; //czy uruchomic tryb uruchamiania najlepszego rozwiazania
	
	//analiza flag
	for(int i = 1; i < argc; i+=2 )
	{
		if(i+1 < argc)
		{
			switch(argv[i][1])
			{
				case 'g'://wlaczenie generowania testow
					//generujemy testy
					gMode = true;
					--i;
					break;
				case 't': //tests - tryb uruchomienia testowania paramertow
					tMode = true;
					--i;
					break;
				case 'r': //run - tryb uruchomienia z ustawionymi przez nas najlepszymi parametrami
					rMode = true;
					break;
				case 'p': //period - liczba okresow w ktorych zbieramy wyniki wynikow
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
				case 'm': //wspolczynnik mutacji
					if(stof(argv[i+1]) <= 0)
					{
						cout << "wspolczynnik mutacji musi byc wiekszy od 0\n";
						return 0;
					}
					else
					{
						propabilityBase = stof(argv[i+1]);
					}
					break;
				case 'v': //wersja mutacji - wersje mutacji z ktorymi wykonywac testy
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
		if(argv[i][1] == 'g') //wlaczenie generowania testow
		{
			//generujemy testy
			gMode = true;
			--i;
		}
		else
		if(argv[i][1] == 't') //tests - tryb uruchomienia testowania paramertow
		{
			tMode = true;
			--i;
		}
		else
		if(argv[i][1] == 'r') //run - tryb uruchomienia z ustawionymi przez nas najlepszymi parametrami
		{
			rMode = true;
		}
		else
		{
			cout << "bledna liczba parametrow wykonania\n";
			return 0;
		}
	}

	if(gMode)
	{
		TestGenerator::generateTests();
	}
	if(tMode)
	{
		tests(times, propabilityBase, populationBase, versions);
	}
	if(rMode)
	{
		run();
	}

	

}

//uruchomienie testow
void tests(int times, int propabilityBase, int populationBase, vector<int>versions)
{
	int period = DEFAULT_PERIOD;
	//wywoloanie iteracyjne
	pair<long long, long long> iterationResults[NUMBER_OF_TESTS];
	for( int i = 0; i < NUMBER_OF_TESTS; ++i )
	{
		//wczytanie pliku z danymi testowymi
		vector<int> marks = readFile(FILE_NAME_BASE + to_string(i) + FILE_NAME_BASE_EXTENSION);

		//znalezienie rozwiazania dla wywolania iteracyjnego
		IterativeSolution iSolution(marks);
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
	vector< vector<MileStone> > results[NUMBER_OF_TESTS * NUMBER_OF_SEEDS]; 
	
	//nazwy kolejnych plikow
	vector<string> fileNames;

	//labels
	vector<string> labels;

	for( int i = 0; i < NUMBER_OF_TESTS; ++i )
	{
		//wczytanie pliku z danymi testowymi
		vector<int> marks = readFile(FILE_NAME_BASE + to_string(i) + FILE_NAME_BASE_EXTENSION);
		
		propabilityBase = DEFAULT_MUTATION_FACTOR;
		for(int j = 1; j <= NUMBER_OF_MUTATION_FACTOR_TESTS; ++j )
		{
			populationBase = DEFAULT_POPULATION_SIZE;
			for(int k = 1; k <= NUMBER_OF_POPULATION_TESTS; ++k)
			{
				for(int version : versions)
				{
					if( i == 0 ) // dodajemy kolejna nazwe pliku
					{
						fileNames.push_back("./out/outV" + to_string(version) + "m" + to_string(propabilityBase) + "p" + to_string(populationBase) + ".txt" );
						labels.push_back("wersja nr " + to_string(version) + ";wspolczynnik mutacji " + to_string(propabilityBase) + 
						";wielkosc populacji " + to_string(populationBase) );
					
					}
					for( int l = 0; l < NUMBER_OF_SEEDS; ++l ) // zmienne seedy
					{
						//uruchamiamy rozwizanie ewolucyjne
						EvolutionarySolution eSolution(marks, populationBase, propabilityBase);
						std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
						eSolution.runSolution(version, l, begin, period, times);
						results[NUMBER_OF_SEEDS * i + l].push_back(eSolution.getMilestones());
					}
				}
				populationBase *= 2;
			}
			propabilityBase *= 2;
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
			for(int j = 0; j < NUMBER_OF_TESTS * NUMBER_OF_SEEDS; ++j)
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

//uruchamiamy rozwiazanie z najlepszymi parametrami
void run()
{
	//wczytujemy oceny ze standardowego wejscia
	vector<int> marks = readStdin();

	EvolutionarySolution eSolution(marks, BEST_POPULATION_SIZE, BEST_MUTATION_FACTOR);
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	eSolution.runSolution(BEST_VERSION, 0, begin, DEFAULT_PERIOD, DEFAULT_TIMES);
	eSolution.writeResult();
}

//wczytujemy wejscie z pliku
vector<int> readFile(string fileName)
{
    vector<int> v;
    ifstream file;
    file.open (fileName);
    int word;
    while (file >> word)
    {
        v.push_back(word);
    }
    return v;
}

//wczytujemy wejscie ze standardowego wejscia
vector<int> readStdin()
{
    vector<int> v;
    int number;
	string s;
	
	getline(std::cin, s);

	std::stringstream iss( s );

    while ( iss >> number )
	{
  		v.push_back( number );
	}
	return v;
}

pair<long long, long long> parseResault(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, int result)
{
	return make_pair(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), result);
}