#include "../include/IterativeSolution.hpp"

using namespace std;

IterativeSolution::IterativeSolution()
{

}

void IterativeSolution::setMarks(std::vector<int> marks)
{
	this->marks = marks;
	this->numberOfStudents = marks.size();
}

void IterativeSolution::runSolution()
{

	//jesli numberOfStudents == 0 => 0 ciastek; numberOfStudents == 1 => 1 ciastko
	if( numberOfStudents <= 1 )
	{
		cout << numberOfStudents;
		return;
	}
	
	//jesli pierwszy uczen ma ocene nie wieksza niz drugi to dostaje 1 ciastko
	if( marks[0] <= marks[1] )
	{
		cakesAmount[0] = 1;
	}
	else //zostawiamy pierwsza osobe do pozniejszego rozpatrzenia
	{
		cakesAmount[0] = -1;
	}
	
	for( int i = 1; i < numberOfStudents - 1; ++i )
	{
		//jesli uczen ma nizsza ocene od obu sasiadow to dostaje 1 ciastko
		if( marks[i] < marks[i+1] && marks[i] < marks[i-1] )
		{
			cakesAmount[i] = 1;
		}
		else
		//jesli ocena jest wieksza od poprzednika i niewieksza od nastepnego
		//to uczen dostaje o 1 ciastko wiecej niz poprzednik
		if( marks[i] > marks[i-1] && marks[i] <= marks[i+1] )
		{
			if( cakesAmount[i-1] != -1 )
			{
				cakesAmount[i] = cakesAmount[i-1] + 1;
			}
			else
			{
				cakesAmount[i] = -1;
			}	
		}
		else
		{
			cakesAmount[i] = -1;
		}
	}
	
	//jesli ostatni uczen ma wyzsza ocene od poprzednika to dostaje o 1 ciastko wiecej niz on
	if( marks[numberOfStudents - 1] > marks[numberOfStudents - 2] )
	{
		cakesAmount[numberOfStudents - 1] = cakesAmount[numberOfStudents - 2] + 1;
	}
	else
	{
		cakesAmount[numberOfStudents - 1] = 1;
	}
	
	for( int i = 0; i < numberOfStudents; ++i )
		cout << cakesAmount[i] << " ";
	cout << "\n";
	
	for( int i = numberOfStudents - 2; i > 0; --i )
	{
		if( cakesAmount[i] == -1 )
		{
			if( marks[i] > marks[i-1] && marks[i] > marks[i+1] )
			{
				cakesAmount[i] = max(cakesAmount[i-1], cakesAmount[i+1]) + 1;
			}
			else
			if( marks[i] < marks[i-1] && marks[i] > marks[i+1] )
			{
				cakesAmount[i] = cakesAmount[i+1] + 1;
			}
		}	
	}
	
	if(cakesAmount[0] == -1)
	{
		cakesAmount[0] = cakesAmount[1] + 1;
	}
	
	for( int i = 0; i < numberOfStudents; ++i )
		cout << cakesAmount[i] << " ";
}