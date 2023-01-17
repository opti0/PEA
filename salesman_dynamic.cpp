#pragma once
#include "salesman_dynamic.h"
#include <vector>
#include <iostream>
#include <limits>

#include "city.h"
#include "salesman_utility.h"

//Rekurencyjny element rozwi�zania
int salesman_dynamic(unsigned long long visited, unsigned int current,
	const vector<vector<int>>& distance,
	vector<vector<int>>& dynamic)
{
	unsigned int n = distance.size();
	unsigned long long m = 1ull << n;
	unsigned long long completed = m - 1;
	constexpr int infinity = numeric_limits<int>::max();

	// koniec oblicze� rekurencyjnych
	if (visited == completed)
		return distance[current][0];
	// warto�� zosta�a ju� obliczona, nie ma sensu liczy� jeszcze raz
	if (dynamic[visited][current] != -1)
		return dynamic[visited][current];

	int result = infinity;
	unsigned int result_city = current;
	int candidate;

	// przejd� po wszystkich miastach kt�re nie zosta�y jeszcze odwiedzone
	for (unsigned int i = 0; i < n; i++)
		if (not (visited & (1ull << i)))
		{
			// rekurencyjne wywo�anie problemu dla �cie�ki do nast�pnego miasta i d�ugo�ci reszty �cie�ki
			// to jest sedno dynamicznego programowania
			candidate = distance[current][i] + salesman_dynamic(visited | (1ull << i), i, distance, dynamic);
			// je�li wyniki s� lepsze to zapisz je
			if (candidate < result)
			{
				result = candidate;
				result_city = i;
			}
		}

	// na sam koniec ustaw nowe warto�ci dla struktury reprezentuj�cej pe�ne rozwi�zanie
	dynamic[visited][current] = result;
	return result;
}

//Rozwi�zanie wykorzystuj�ce programowanie dynamiczne
vector<unsigned int> salesman_dynamic(vector<vector<int>> distance)
{
	unsigned int n = distance.size();
	unsigned long long m = (1ull << n);

	// dane do dynamicznej reprezentacji problemu
	// identyfikator pe�nego rozwi�zania - tj. przypadek gdy wszystkie miasta s� odwiedzone
	unsigned long long completed = m - 1ull;
	// macierz do przechowywania cz�stkowych rozwi�za� dynamicznych
	// next jest potrzebny do pilnowania �cie�ki bo je�li jest ona wyra�ona 
	// w double to b��dy numeryczne powoduj�, �e trudno j� odtworzy� z samego dynamic
	// tj. nigdy nie ufaj == dla warto�ci zmiennoprzecinkowych
	vector<vector<int>> dynamic(m);
	for (unsigned long long i = 0; i < m; i++)
	{
		dynamic[i] = vector<int>(n);
		for (unsigned int j = 0; j < n; j++)
			//warto�� -1 jest symboliczna, znaczy, �e problem dla tych warto�ci nie zosta� jeszcze policzony
			dynamic[i][j] = -1;
	}

	salesman_dynamic(1ull, 0u, distance, dynamic);


	// rekonstrukcja �cie�ki
	vector<unsigned int> r;
	r.push_back(0);
	unsigned int current = 0;
	unsigned long long visited = 1ull;

	constexpr int infinity = numeric_limits<int>::max();
	int best_path;
	int next = 0;
	int next_path;
	
	// je�eli jeszcze nie odwiedzili�my wszystkich miast 
	while (visited != completed)
	{
		best_path = infinity;
		for (unsigned int i = 0; i < n; i++)
			if (not (visited & (1ull << i)))
			{
				next_path = distance[current][i] + dynamic[visited | (1ull << i)][i];
				if (next_path < best_path)
				{
					best_path = next_path;
					next = i;
				}
			}
		current = next;
		visited = visited | (1ull << next);
		r.push_back(next);
	}

	return r;
}

vector<unsigned int> salesman_dynamic(vector<city> locations)
{
	return salesman_dynamic(precalculate_distances(locations));
}