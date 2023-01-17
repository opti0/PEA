#pragma once
#include "salesman_bruteforce.h"
#include <algorithm>
#include <vector>
#include <iostream>

#include "city.h"
#include "salesman_utility.h"

//Rozwi�zanie bruteforce, bez optymalizacji, wykorzystuj�ce stl
//Dzia�a do ~12 elementow
vector<unsigned int> salesman_bruteforce(vector<vector<int>> distances)
{
	unsigned int n = distances.size();

	// bierz�ce rozwi�zanie
	vector<unsigned int> candidate(n);
	int candidate_path;
	for (unsigned int i = 0; i < n; i++)
		candidate[i] = i;

	// najlepsze rozwi�zanie
	// warto�� startowa
	vector<unsigned int> order = candidate;
	int order_path = path_length(distances, order);
	//cout << order << " = " << order_path << endl;

	// zawsze zaczynamy od miasta 0, ale potem kolejno�� mo�e by� dowolna
	while (next_permutation(candidate.begin() + 1, candidate.end()))
	{
		// liczymy now� �cie�k�
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