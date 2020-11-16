#include "../include/IterativeSolution.hpp"

using namespace std;

void IterativeSolution::runSolution()
{
    int n;
	cin >> n;
	
	for( int i = 0; i < n; ++i )
	{
		cin >> marks[i];
	}
	
	//jesli n == 0 => 0 ciastek; n == 1 => 1 ciastko
	if( n <= 1 )
	{
		cout << n;
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
	
	for( int i = 1; i < n - 1; ++i )
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
	if( marks[n-1] > marks[n-2] )
	{
		cakesAmount[n-1] = cakesAmount[n-2] + 1;
	}
	else
	{
		cakesAmount[n-1] = 1;
	}
	
	for( int i = 0; i < n; ++i )
		cout << cakesAmount[i] << " ";
	cout << "\n";
	
	for( int i = n-2; i > 0; --i )
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
	
	for( int i = 0; i < n; ++i )
		cout << cakesAmount[i] << " ";
}