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

const int NUMBER_OF_TESTS = 70;

const std::string FILE_NAME_BASE = "./in/input";
const std::string FILE_NAME_BASE_EXTENSION = ".txt";
const int NUMBER_OF_VERSIONS = 1;

const int DEFAULT_WIELKOSC_POPULACJI = 100;
const int DEFAULT_LICZBA_GENERACJI = 10000;
const int DEFAULT_PRAWDOPODOBIENSTWO_MUTACJI = 5;
const int DEFAULT_WERSJA_MUTACJI = 1;

vector<int> czytaniePliku(string sciezka);
pair<long, long> parseResault(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, int result);

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
	cin.tie(NULL);

	//wartosci domyslne
	int wielkoscPopulacji = DEFAULT_WIELKOSC_POPULACJI;
	int liczbaGeneracji = DEFAULT_LICZBA_GENERACJI;
	double prawdopodobienstwoMutacji = DEFAULT_PRAWDOPODOBIENSTWO_MUTACJI;
	int wersjaMutacji = DEFAULT_WERSJA_MUTACJI;
	
	//analiza flag
	for(int i = 1; i < argc; i+=2 )
	{
		if(i+1 < argc)
		{
			switch(argv[i][1])
			{
				case 'p': //populacja
					if(stoi(argv[i+1]) < 5)
					{
						cout << "populacja musi byc nie mniejsza niz 5\n";
						return 0;
					}
					else
					{
						wielkoscPopulacji = stoi(argv[i+1]);
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
						wersjaMutacji = stoi(argv[i+1]);
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

	//miejsce na zapisywanie wynikow
	vector<pair<long, long> > results[NUMBER_OF_TESTS];
	
	//nazwy kolejnych testow
	vector<string> labels;

	labels.push_back("rozwiazanie iteracyjne");
	labels.push_back("rozwiazanie ewolucyjne");

	for( int i = 0; i < NUMBER_OF_TESTS; ++i )
	{
		vector<int> oceny = czytaniePliku(FILE_NAME_BASE + to_string(i) + FILE_NAME_BASE_EXTENSION);

		
		//cout << "rozwiazanie iteracyjne:\n";
		IterativeSolution iSolution;
    	iSolution.setMarks(oceny);
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    	iSolution.runSolution();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		results[i].push_back(parseResault(begin, end, iSolution.getResult()));
		//results[i].push_back(make_pair(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(),iSolution.getResult());
		//iSolution.writeResult();

		//cout << "rozwiazanie ewolucyjne:\n";
		EvolutionarySolution eSolution(wielkoscPopulacji, liczbaGeneracji, prawdopodobienstwoMutacji);
    	eSolution.setOceny(oceny);
		begin = std::chrono::steady_clock::now();
		eSolution.runSolution(wersjaMutacji);
		end = std::chrono::steady_clock::now();
		results[i].push_back(parseResault(begin, end, eSolution.getRezultat()));
		//cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()<< "; " << eSolution.getRezultat() << "\n";
		//eSolution.piszWynik();
			
	}

	//wypisujemy rozwiazanie
	for(string s : labels)
	{
		cout << s << "\t";
	}
	cout << "\n";
	for(vector<pair<long, long> > v : results)
	{
		for(pair<long, long> p : v)
		{
			cout << p.first << "\t" << p.second << "\t";
		}
		cout << "\n";
	}

    //vector<int> oceny = czytaniePliku(FILE_NAME_BASE);



    // EvolutionarySolution eSolution(wielkoscPopulacji, liczbaGeneracji, prawdopodobienstwoMutacji);
    // eSolution.setOceny(oceny);
	// eSolution.runSolution(wersjaMutacji);
	// eSolution.piszWynik();

    // IterativeSolution iSolution;
    // iSolution.setMarks(oceny);
    // iSolution.runSolution();
	// iSolution.writeResult();


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

pair<long, long> parseResault(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end, int result)
{
	return make_pair(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), result);
}