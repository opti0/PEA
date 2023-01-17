#include "salesman_taboo.h"
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>

#include "city.h"
#include "salesman_utility.h"

vector<unsigned int> salesman_taboo(
	vector<city> locations,
	random_device& rd,
	long long limit)
{
	return salesman_taboo(precalculate_distances(locations), rd, limit);
}
vector<unsigned int> salesman_taboo(
	vector<vector<int>> distances,
	random_device& rd,
	long long limit)
{
	unsigned int n = distances.size();

	mt19937 gen(rd());
	srand(gen());

	vector<vector<int>> taboo;
	vector<unsigned int> permutation(n);
	vector<unsigned int> candidate(n);
	vector<unsigned int> best(n);
	first_permutation(permutation);
	copy_permutation(best, permutation);
	int best_path = path_length(distances, best);
	int permutation_path;
	int candidate_path;

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point now;

	int index_a;
	int index_b;
	long long elapsed_time;

	start = chrono::high_resolution_clock::now();
	while (true)
	{
		new_permutation(permutation);
		taboo = vector<vector<int>>(n, vector<int>(n));
		index_a = 0;
		index_b = 0;

		for (int step = 0; step < 15 * n; step++)
		{
			copy_permutation(candidate, permutation);
			candidate_path = path_length(distances, permutation);

			for (int a = 0; a < n; a++)
			{
				for (int b = a + 1; b < n; b++)
				{
					swap(permutation[a], permutation[b]);
					permutation_path = path_length(distances, permutation);

					if (permutation_path < best_path)
					{
						best_path = permutation_path;
						copy_permutation(best, permutation);
					}

					if (permutation_path < candidate_path)
						if (taboo[b][a] < step)
						{
							candidate_path = permutation_path;
							copy_permutation(candidate, permutation);
							index_a = a;
							index_b = b;
						}

					swap(permutation[a], permutation[b]);

					now = chrono::high_resolution_clock::now();
					elapsed_time = chrono::duration_cast<chrono::milliseconds>(now - start).count();
					if (elapsed_time >= limit)
						return best;
				}
			}
			copy_permutation(permutation, candidate);
			taboo[index_a][index_b] += n;
		}
	}
	return best;
}