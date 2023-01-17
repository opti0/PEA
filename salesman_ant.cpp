#include "salesman_ant.h"
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <cmath>

#include "city.h"
#include "salesman_utility.h"

void ant_new_permutation(
	vector<vector<int>>& distances, 
	vector<vector<float>>& pheromone, 
	vector<unsigned int>& order,
	random_device& rd)
{
	unsigned int n = distances.size();
	vector<double> prob(n);
	double total_prob;
	double slice_prob;

	unsigned long long visited = 1ul;
	unsigned long long all = (1ull << n) - 1;

	unsigned int current = 0;
	order[0] = 0;
	unsigned int next;
	for (unsigned int node = 1; node < n; node++)
	{
		total_prob = 0.0;
		for (unsigned int i = 0; i < n; i++)
			if (not (visited & (1ull << i)))
			{
				prob[i] = 
					pow(max(pheromone[current][i], 0.0001f), 0.9) / 
					pow(distances[current][i], 1.5);
				total_prob += prob[i];
			}
		slice_prob = uniform_real_distribution<double>(0.0, total_prob)(rd);
		for (unsigned int i  = 0; i < n; i++)
			if (not (visited & (1ull << i)))
			{
				next = i;
				if (slice_prob <= prob[i]) break;
				slice_prob -= prob[i];
			}
		visited |= (1ull << next);
		order[node] = current = next;
	}
}
vector<unsigned int> salesman_ant(
	vector<city> locations,
	long long iterations,
	long long ants,
	float pheromon,
	float evaporation)
{
	return salesman_ant(precalculate_distances(locations), iterations, ants, pheromon, evaporation);
}
vector<unsigned int> salesman_ant(
	vector<vector<int>> distances,
	long long iterations,
	long long ants,
	float pheromon,
	float evaporation)
{
	unsigned long long n = distances.size();
	random_device rd;

	vector<unsigned int> best(n);
	first_permutation(best);
	int best_path = path_length(distances, best);
	int permutation_path;
	float delta;

	vector<vector<unsigned int>> ant_permutations(ants);
	for (unsigned int i = 0; i < ants; i++)
		ant_permutations[i] = vector<unsigned int>(n);
	vector<vector<float>> ant_pheromone(n);
	for (unsigned int i = 0; i < n; i++)
		ant_pheromone[i] = vector<float>(n, 0.0f);

	for (unsigned int iteration = 0; iteration < iterations; iteration++)
	{
		for (unsigned int i = 0; i < ants; i++)
		{
			ant_new_permutation(distances, ant_pheromone, ant_permutations[i], rd);
			permutation_path = path_length(distances, ant_permutations[i]);
			if (permutation_path < best_path)
			{
				best_path = permutation_path;
				copy_permutation(best, ant_permutations[i]);
			}
		}

		for (unsigned int i = 0; i < ants; i++)
		{
			delta = (float)(pheromon / path_length(distances, ant_permutations[i]));
			for (unsigned int j = 1; j < n; j++)
				ant_pheromone[ant_permutations[i][j - 1]][ant_permutations[i][j]] += delta;
			ant_pheromone[ant_permutations[i][n - 1]][ant_permutations[i][0]] += delta;
		}
		for (unsigned int i = 0; i < n; i++)
			for (unsigned int j = 0; j < n; j++)
				ant_pheromone[i][j] *= evaporation;
	}

	return best;
}