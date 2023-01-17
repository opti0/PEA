#pragma once
#include "salesman_bruteforce.h"
#include <algorithm>
#include <vector>
#include <iostream>

#include "city.h"
#include "salesman_utility.h"

//Rozwi¹zanie bruteforce, bez optymalizacji, wykorzystuj¹ce stl
//Dzia³a do ~12 elementow
vector<unsigned int> salesman_bruteforce(vector<vector<int>> distances)
{
	unsigned int n = distances.size();

	// bierz¹ce rozwi¹zanie
	vector<unsigned int> candidate(n);
	int candidate_path;
	for (unsigned int i = 0; i < n; i++)
		candidate[i] = i;

	// najlepsze rozwi¹zanie
	// wartoœæ startowa
	vector<unsigned int> order = candidate;
	int order_path = path_length(distances, order);
	//cout << order << " = " << order_path << endl;

	// zawsze zaczynamy od miasta 0, ale potem kolejnoœæ mo¿e byæ dowolna
	while (next_permutation(candidate.begin() + 1, candidate.end()))
	{
		// liczymy now¹ œcie¿kê
		candidate_path = path_length(distances, candidate);
		// sprawdzamy czy poprawia to wynik, jak tak to go zapisujemy
		if (candidate_path < order_path)
		{
			order = candidate;
			order_path = candidate_path;
			//cout << order << " = " << order_path << endl;
		}
	}

	return order;
}

vector<unsigned int> salesman_bruteforce(vector<city> locations)
{
	return salesman_bruteforce(precalculate_distances(locations));
}