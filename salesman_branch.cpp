#pragma once
#include "salesman_branch.h"
#include <vector>
#include <iostream>

#include "city.h"
#include "salesman_utility.h"

void salesman_branch(unsigned long long visited, unsigned int current,
	double current_path,
	const unsigned int n, const unsigned long long m,
	const vector<vector<int>>& distance,
	vector<unsigned int>& candidate,
	vector<unsigned int> partial,
	int& upper_bound_path,
	const unsigned long long completed)
{
	partial.push_back(current);

	// je�li sprawdzili�my wszystkie miasta...
	if (visited == completed)
	{
		// zamykamy p�tl�...
		current_path += distance[current][0];
		// i sprawdzamy czy znaleziona �cie�ka jest lepsza ni� g�rna granica
		if (current_path < upper_bound_path)
		{
			upper_bound_path = current_path;
			candidate = partial;
		}
		return;
	}

	for (unsigned int i = 0; i < n; i++)
		// sprawdzamy miasto tylko je�li jeszcze tam nie byli�my...
		if (not (visited & (1ull << i)))
			// oraz je�li dotychczasowa �cie�ka jest mniejsza ni� nasza g�rna granica...
			if (current_path + distance[current][i] < upper_bound_path)
				salesman_branch(visited | (1ull << i), i, current_path + distance[current][i], n, m,
					distance, candidate, partial, upper_bound_path, completed);

}

vector<unsigned int> salesman_branch(vector<vector<int>> distances,
	vector<unsigned int>& candidate,
	int& upper_bound_path)
{
	unsigned int n = distances.size();
	unsigned long long m = (1ull << n);
	unsigned long long completed = m - 1ull;
	salesman_branch(
		1ull, 0u, 0.0, n, m,
		distances, candidate, vector<unsigned int>(), upper_bound_path, completed
	);

	return candidate;
}

bool order_comparator(const pair<unsigned int, double>& a, const pair<unsigned int, double>& b)
{
	return a.second < b.second;
}
vector<unsigned int> salesman_branch(vector<city> locations, bool version)
{
	// macierz odleg�o�ci
	vector<vector<int>> distances = precalculate_distances(locations);

	// dane do dynamicznej reprezentacji problemu
	// identyfikator pe�nego rozwi�zania - tj. przypadek gdy wszystkie miasta s� odwiedzone
	unsigned int n = locations.size();
	unsigned long long m = (1ull << n);
	unsigned long long completed = m - 1ull;

	// startowe rozwi�zanie
	vector<unsigned int> candidate(n);
	if (version)
	{
		// z�o�one rozwi�zanie startowe, gdzie sotrujemy miasta wed�ug k�ta
		// znajdujemy �rodek ci�ko�ci miast
		double xc = 0.0;
		double yc = 0.0;
		for (unsigned int i = 0; i < n; i++)
		{
			xc += locations[i].x;
			yc += locations[i].y;
		}
		xc /= n;
		yc /= n;

		// uzyskujemy k�t ka�dego miasta wzgl�dem �rodka ci�ko�ci
		vector<pair<unsigned int, double>> order(n);
		for (unsigned int i = 0; i < n; i++)
		{
			order[i].first = i;
			order[i].second = atan2(xc - locations[i].x, yc - locations[i].y);
		}
		// sortujemy wed�ug warto�ci k�ta
		sort(order.begin(), order.end(), order_comparator);
		// przepisujemy do kolejno�ci
		for (unsigned int i = 0; i < n; i++)
		{
			candidate[i] = order[i].first;
		}
	}
	else
	{
		// proste rozwi�zanie startowe, odwiedzamy miasta w kolejno�ci w jakiej
		// wypisane s� w pliku
		for (unsigned int i = 0; i < n; i++)
			candidate[i] = i;
	}
	// g�rna granica rozwi�zania
	int upper_bound_path = path_length(distances, candidate);

	// uruchamiamy algorytm rozpoczynaj�� od pierwszego miasta
	return salesman_branch(distances, candidate, upper_bound_path);
}

vector<unsigned int> salesman_branch(vector<vector<int>> distances)
{
	unsigned int n = distances.size();

	// startowe rozwi�zanie
	vector<unsigned int> candidate(n);
	for (unsigned int i = 0; i < n; i++)
		candidate[i] = i;
	// g�rna granica rozwi�zania
	int upper_bound_path = path_length(distances, candidate);

	// uruchamiamy algorytm rozpoczynaj�� od pierwszego miasta
	return salesman_branch(distances, candidate, upper_bound_path);
}