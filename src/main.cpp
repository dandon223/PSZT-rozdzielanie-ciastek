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

using namespace std;

const std::string FILE_NAME = "./in/input.txt";

vector<int> czytaniePliku(string sciezka);

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(0);
	cin.tie(NULL);

	//wartosci domyslne
	int wielkoscPopulacji = 100;
	int liczbaGeneracji = 10000;
	int prawdopodobienstwoMutacji = 5;
	
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
					prawdopodobienstwoMutacji = stoi(argv[i+1]);
					break;
			}
		}
		else
		{
			cout << "bledna liczba parametrow wykonania\n";
			return 0;
		}
	}

    vector<int> oceny = czytaniePliku(FILE_NAME);

    EvolutionarySolution eSolution(wielkoscPopulacji, liczbaGeneracji, prawdopodobienstwoMutacji);
    eSolution.setOceny(oceny);
	eSolution.runSolution();

    IterativeSolution iSolution;
    iSolution.setMarks(oceny);
    iSolution.runSolution();


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