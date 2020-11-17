#include "../include/IterativeSolution.hpp"

using namespace std;

IterativeSolution::IterativeSolution()
{

}

void IterativeSolution::setMarks(std::vector<int> marks)
{
	this->marks = marks;
	this->numberOfStudents = marks.size();
	result = 0;
}

void IterativeSolution::runSolution()
{
	//redukujemy problem do przypadku gdzie oceny sie nie powtarzaja
	vector<pair<int, int> > reducedMarks = reduceProblem();//first - ocena, second - liczba osob z taka ocena

	vector<int> reducedCookies;
	reducedCookies.resize(reducedMarks.size());

	numberOfStudents = reducedMarks.size();

	//jesli numberOfStudents == 0 => 0 ciastek; numberOfStudents == 1 => 1 ciastko
	if( numberOfStudents == 0 )
	{
		result = numberOfStudents;
		return;
	}
	else
	if( numberOfStudents == 1 )
	{
		result = numberOfStudents;
		reducedCookies[0] = 1;
		return;
	}
	
	
	//jesli pierwszy uczen ma ocene nie wieksza niz drugi to dostaje 1 ciastko
	if( reducedMarks[0].first <= reducedMarks[1].first )
	{
		reducedCookies[0] = 1;
	}
	else //zostawiamy pierwsza osobe do pozniejszego rozpatrzenia
	{
		reducedCookies[0] = -1;
	}
	
	for( int i = 1; i < numberOfStudents - 1; ++i )
	{
		//jesli poprzednik ma taka sama ocene to dajemy tyle samo ciastek
		if( reducedMarks[i].first == reducedMarks[i-1].first)
		{
			reducedCookies[i] = reducedCookies[i-1];
		}
		else
		//jesli uczen ma nizsza ocene od obu sasiadow to dostaje 1 ciastko
		if( reducedMarks[i].first < reducedMarks[i+1].first && reducedMarks[i].first < reducedMarks[i-1].first )
		{
			reducedCookies[i] = 1;
		}
		else
		//jesli ocena jest wieksza od poprzednika i niewieksza od nastepnego
		//to uczen dostaje o 1 ciastko wiecej niz poprzednik
		if( reducedMarks[i].first > reducedMarks[i-1].first && reducedMarks[i].first <= reducedMarks[i+1].first )
		{
			if( reducedCookies[i-1] != -1 )
			{
				reducedCookies[i] = reducedCookies[i-1] + 1;
			}
			else
			{
				reducedCookies[i] = -1;
			}	
		}
		else
		{
			reducedCookies[i] = -1;
		}
	}
	
	//jesli ostatni uczen ma wyzsza ocene od poprzednika to dostaje o 1 ciastko wiecej niz on
	if( reducedMarks[numberOfStudents - 1].first > reducedMarks[numberOfStudents - 2].first )
	{
		reducedCookies[numberOfStudents - 1] = reducedCookies[numberOfStudents - 2] + 1;
	}
	else
	{
		reducedCookies[numberOfStudents - 1] = 1;
	}
	
	// for( int i = 0; i < numberOfStudents; ++i )
	// 	cout << reducedCookies[i] << " ";
	// cout << "\n";
	
	for( int i = numberOfStudents - 2; i > 0; --i )
	{
		if( reducedCookies[i] == -1 )
		{
			if( reducedMarks[i].first > reducedMarks[i-1].first && reducedMarks[i].first > reducedMarks[i+1].first )
			{
				reducedCookies[i] = max(reducedCookies[i-1], reducedCookies[i+1]) + 1;
			}
			else
			if( reducedMarks[i].first < reducedMarks[i-1].first && reducedMarks[i].first > reducedMarks[i+1].first )
			{
				reducedCookies[i] = reducedCookies[i+1] + 1;
			}
		}	
	}
	
	if(reducedCookies[0] == -1)
	{
		reducedCookies[0] = reducedCookies[1] + 1;
	}
	
	// for( int i = 0; i < numberOfStudents; ++i )
	// 	cout << reducedCookies[i] << " ";

	for( unsigned int i = 0; i < reducedMarks.size(); ++i )
	{
		pair<int, int> p = reducedMarks[i];
		for( int j = 0; j < p.second; ++j )
		{
			cookiesAmount.push_back(reducedCookies[i]);
			result += reducedCookies[i];
		}
	}

	numberOfStudents = marks.size(); // przywracamy oryginalna liczbe uczniow

	// for( int i = 0; i < numberOfStudents; ++i )
	// 	cout << cookiesAmount[i] << " ";
}

int IterativeSolution::getResult()
{
	return result;
}

void IterativeSolution::writeResult()
{
	cout << "rozklad ciatek:\n";
	for(int i : cookiesAmount)
	{
		cout << i << " ";
	}
	cout << "\nrazem:\n" << result << "\n";
}

std::vector<pair<int, int> > IterativeSolution::reduceProblem()
{
	std::vector<pair<int, int> > reducedProblem;
	for( unsigned int i = 0; i < marks.size(); ++i )
	{
		int counter = 1;
		unsigned int j = i+1;
		while( j < marks.size() && marks[j] == marks[i] )
		{
			counter++;
			j++;
			i++;
		}
		reducedProblem.push_back(make_pair(marks[i], counter));
	}

	// for(pair <int,int> p : reducedProblem)
	// {
	// 	cout << p.first << " " << p.second << " | ";
	// }

	return reducedProblem;
}