#include "salesman_annealing.h"
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>

#include "city.h"
#include "salesman_utility.h"

double init_temperature(vector<vector<int>> distances, unsigned int samples, double cooling)
{
	//Ta funkcja liczy temperaturê startow¹ na podstawie próbkowania przestrzeni rozwi¹zañ
	double temp = 0.0;
	unsigned long long n = distances.size();
	vector<unsigned int> order(n);
	first_permutation(order);
	int path_length_a;
	int path_length_b;

	for (unsigned int i = 0; i < samples; i++)
	{
		new_permutation(order);
		path_length_a = path_length(distances, order);
		swap(order[rand() % n], order[rand() % n]);
		path_length_b = path_length(distances, order);
		temp += abs(path_length_a - path_length_b);
	}
	return (-temp / samples) / log(cooling);
}

vector<unsigned int> salesman_annealing(
	vector<city> locations, 
	random_device& rd,
	long long limit,
	double min_temp,
	unsigned int samples,
	double cooling)
{
	return salesman_annealing(precalculate_distances(locations), rd, limit, min_temp, samples, cooling);
}
vector<unsigned int> salesman_annealing(vector<vector<int>> distances, 
	random_device& rd,
	long long limit,
	double min_temp,
	unsigned int samples,
	double cooling)
{
	unsigned long long n = distances.size();

	mt19937 gen(rd());
	srand(gen());

	vector<unsigned int> permutation(n);
	vector<unsigned int> candidate(n);
	vector<unsigned int> best(n);
	first_permutation(permutation);
	copy_permutation(best, permutation);
	int best_path = path_length(distances, best);

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point now;

	int index_a;
	int index_b;
	int candidate_path;
	int permutation_path;
	long long elapsed_time;
	double init_temp = init_temperature(distances, samples, cooling);
	
	// Metoda dzia³a przez zadany czas
	start = chrono::high_resolution_clock::now();
	// Rozpoczynamy nowy cykl
	// Ka¿dy cykl rozpoczyna siê od nowego, losowego punktu startowego
	double temp = init_temp;
	new_permutation(permutation);
	permutation_path = path_length(distances, permutation);
	copy_permutation(candidate, permutation);
	// Jeœli temperatura spadnie poni¿ej tej granicy rozpoczniemy nowy cykl
	while (temp >= min_temp)
	{
		// W ramach cyklu próbujemy kilka (n) losowych skoków
		for (unsigned int i = 0; i < n; i++)
		{
			index_a = rand() % n;
			index_b = rand() % n;
			swap(candidate[index_a], candidate[index_b]);
			candidate_path = path_length(distances, candidate);

			// Jeœli wynik jest najlepszy jako do tej pory mieliœmy to
			// - bêdzie to nowy wynik funkcji
			// - bêdzie to punkt startowy do kolejnych prób w cyklu
			if (candidate_path < best_path)
			{
				best_path = candidate_path;
				permutation_path = candidate_path;
				copy_permutation(best, candidate);
				swap(permutation[index_a], permutation[index_b]);
				break;
			}
			else if (candidate_path < permutation_path)
			{
				permutation_path = candidate_path;
				swap(permutation[index_a], permutation[index_b]);
				break;
			}
			// Nawet jeœli nie jest lepszy, to mo¿emy przesun¹c punkt startowy do dalszych obliczeñ
			else if (exp(((long long)permutation_path - candidate_path) / temp) > ((double)rand() / RAND_MAX))
			{
				swap(permutation[index_a], permutation[index_b]);
				break;
			}
			// W ostatecznoœci przywracamy poprzedni punkt startowy
			else
			{
				swap(candidate[index_a], candidate[index_b]);
			}
			// Jeœli czas siê skoñczy³ to podajemy najlepszy wynik
			now = chrono::high_resolution_clock::now();
			elapsed_time = chrono::duration_cast<chrono::milliseconds>(now - start).count();
			if (elapsed_time >= limit)
				return best;
		}
		temp *= cooling;
	}
	return best;
}