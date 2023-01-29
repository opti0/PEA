#include "salesman_ant.h"
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <cmath>
#include <chrono>

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

	vector<bool> visited = vector<bool>(n);
	visited[0] = true;

	unsigned int current = 0;
	order[0] = 0;
	unsigned int next = 0;
	for (unsigned int node = 1; node < n; node++)
	{
		total_prob = 0.0;
		for (unsigned int i = 1; i < n; i++)
			if (not visited[i])
			{
				prob[i] = 
					pow(max(pheromone[current][i], 0.0001f), 0.9) / 
					pow(distances[current][i], 1.5);
				total_prob += prob[i];
			}
		slice_prob = uniform_real_distribution<double>(0.0, total_prob)(rd);
		for (unsigned int i  = 1; i < n; i++)
			if (not visited[i])
			{
				next = i;
				if (slice_prob <= prob[i]) break;
				slice_prob -= prob[i];
			}
		visited[next] = true;
		order[node] = current = next;
	}
}
vector<unsigned int> salesman_ant(
	vector<city> locations,
	unsigned long long time,
	unsigned long long ants,
	float pheromon,
	float evaporation)
{
	return salesman_ant(precalculate_distances(locations), time, ants, pheromon, evaporation);
}
vector<unsigned int> salesman_ant(
	vector<vector<int>> distances,
	unsigned long long time,
	unsigned long long ants,
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

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < time)
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