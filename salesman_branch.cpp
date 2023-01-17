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

	// jeœli sprawdziliœmy wszystkie miasta...
	if (visited == completed)
	{
		// zamykamy pêtlê...
		current_path += distance[current][0];
		// i sprawdzamy czy znaleziona œcie¿ka jest lepsza ni¿ górna granica
		if (current_path < upper_bound_path)
		{
			upper_bound_path = current_path;
			candidate = partial;
		}
		return;
	}

	for (unsigned int i = 0; i < n; i++)
		// sprawdzamy miasto tylko jeœli jeszcze tam nie byliœmy...
		if (not (visited & (1ull << i)))
			// oraz jeœli dotychczasowa œcie¿ka jest mniejsza ni¿ nasza górna granica...
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
	// macierz odleg³oœci
	vector<vector<int>> distances = precalculate_distances(locations);

	// dane do dynamicznej reprezentacji problemu
	// identyfikator pe³nego rozwi¹zania - tj. przypadek gdy wszystkie miasta s¹ odwiedzone
	unsigned int n = locations.size();
	unsigned long long m = (1ull << n);
	unsigned long long completed = m - 1ull;

	// startowe rozwi¹zanie
	vector<unsigned int> candidate(n);
	if (version)
	{
		// z³o¿one rozwi¹zanie startowe, gdzie sotrujemy miasta wed³ug k¹ta
		// znajdujemy œrodek ciê¿koœci miast
		double xc = 0.0;
		double yc = 0.0;
		for (unsigned int i = 0; i < n; i++)
		{
			xc += locations[i].x;
			yc += locations[i].y;
		}
		xc /= n;
		yc /= n;

		// uzyskujemy k¹t ka¿dego miasta wzglêdem œrodka ciê¿koœci
		vector<pair<unsigned int, double>> order(n);
		for (unsigned int i = 0; i < n; i++)
		{
			order[i].first = i;
			order[i].second = atan2(xc - locations[i].x, yc - locations[i].y);
		}
		// sortujemy wed³ug wartoœci k¹ta
		sort(order.begin(), order.end(), order_comparator);
		// przepisujemy do kolejnoœci
		for (unsigned int i = 0; i < n; i++)
		{
			candidate[i] = order[i].first;
		}
	}
	else
	{
		// proste rozwi¹zanie startowe, odwiedzamy miasta w kolejnoœci w jakiej
		// wypisane s¹ w pliku
		for (unsigned int i = 0; i < n; i++)
			candidate[i] = i;
	}
	// górna granica rozwi¹zania
	int upper_bound_path = path_length(distances, candidate);

	// uruchamiamy algorytm rozpoczynaj¹æ od pierwszego miasta
	return salesman_branch(distances, candidate, upper_bound_path);
}

vector<unsigned int> salesman_branch(vector<vector<int>> distances)
{
	unsigned int n = distances.size();

	// startowe rozwi¹zanie
	vector<unsigned int> candidate(n);
	for (unsigned int i = 0; i < n; i++)
		candidate[i] = i;
	// górna granica rozwi¹zania
	int upper_bound_path = path_length(distances, candidate);

	// uruchamiamy algorytm rozpoczynaj¹æ od pierwszego miasta
	return salesman_branch(distances, candidate, upper_bound_path);
}