#include "salesman_genetic.h"
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>

#include "city.h"
#include "salesman_utility.h"

void mutation_a(vector<unsigned int>& genome, double p, random_device& rd)
{
	unsigned long long n = genome.size();
	uniform_real_distribution<double> chance(0.0, 1.0);
	uniform_int_distribution<unsigned long long> position(1, genome.size() - 1);
	for (unsigned long long i = 0; i < n; i++) if (chance(rd) < p)
	{
		//Wybraæ elementy do zamiany:
		unsigned long long a = position(rd);
		unsigned long long b = position(rd);
		//Zamieniæ elementy:
		swap(genome[a], genome[b]);
	}
}
void mutation_b(vector<unsigned int>& genome, double p, random_device& rd)
{
	unsigned long long n = genome.size();
	uniform_real_distribution<double> chance(0.0, 1.0);
	uniform_int_distribution<unsigned long long> position(1, genome.size() - 1);
	for (unsigned long long i = 0; i < n; i++) if (chance(rd) < p)
	{
		//Wybraæ segment do obrotu:
		unsigned long long a = position(rd);
		unsigned long long b = position(rd);
		unsigned long long start = min(a, b);
		unsigned long long end = max(a, b);
		//Obróæ segment:
		rotate(genome.begin() + start, genome.begin() + start + 1, genome.begin() + end + 1);
	}
}
void crossover_a(
	const vector<unsigned int>& parent_a,
	const vector<unsigned int>& parent_b,
	vector<unsigned int>& child_a,
	vector<unsigned int>& child_b,
	random_device& rd)
{
	unsigned long long n = parent_a.size();
	uniform_int_distribution<unsigned long long> position(1, n - 1);
	
	vector<bool> visited_a(n);
	visited_a[0] = true;
	vector<bool> visited_b(n);
	visited_b[0] = true;
	//Wybraæ odcinek do zamiany:
	unsigned long long a = position(rd);
	unsigned long long b = position(rd);
	unsigned long long start = min(a, b);
	unsigned long long end = max(a, b);

	unsigned long long done_a = 1;
	unsigned long long done_b = 1;

	//Przepisaæ pocz¹tki œcie¿ek:
	for (unsigned int i = 1; i < start; i++)
	{
		child_a[done_a] = parent_a[i];
		visited_a[child_a[done_a++]] = true;
		child_b[done_b] = parent_b[i];
		visited_b[child_b[done_b++]] = true;
	}
	//Przepisaæ czêœæ do zamiany:
	for (unsigned int i = start; i <= end; i++)
	{
		if (not visited_a[parent_b[i]])
		{
			child_a[done_a] = parent_b[i];
			visited_a[child_a[done_a++]] = true;
		}
		if (not visited_b[parent_a[i]])
		{
			child_b[done_b] = parent_a[i];
			visited_b[child_b[done_b++]] = true;
		}
	}
	//Przepisaæ koñcow¹ czêœæ:
	for (unsigned int i = 1; i < n; i++)
	{
		if (not visited_a[parent_a[i]])
		{
			child_a[done_a] = parent_a[i];
			visited_a[child_a[done_a++]] = true;
		}
		if (not visited_b[parent_b[i]])
		{
			child_b[done_b] = parent_b[i];
			visited_b[child_b[done_b++]] = true;
		}
	}
}

vector<unsigned int> salesman_genetic(
	vector<city> locations,
	void(*mutation_type)(vector<unsigned int>& genome, double p, random_device& rd),
	void(*crossover_type)(
		const vector<unsigned int>& parent_a,
		const vector<unsigned int>& parent_b,
		vector<unsigned int>& child_a,
		vector<unsigned int>& child_b,
		random_device& rd),
	unsigned long long time,
	unsigned long long population,
	double crossover,
	double mutation)
{
	return salesman_genetic(precalculate_distances(locations), mutation_type, crossover_type, time, population, crossover, mutation);
}
vector<unsigned int> salesman_genetic(
	vector<vector<int>> distances,
	void(*mutation_type)(vector<unsigned int>& genome, double p, random_device& rd),
	void(*crossover_type)(
		const vector<unsigned int>& parent_a,
		const vector<unsigned int>& parent_b,
		vector<unsigned int>& child_a,
		vector<unsigned int>& child_b,
		random_device& rd),
	unsigned long long time,
	unsigned long long population,
	double crossover,
	double mutation)
{
	//Wyrównanie pokolenia do wartoœci podzielnej przez 2:
	if (population % 2) population++;

	unsigned long long n = distances.size();
	constexpr int infinity = numeric_limits<int>::max();
	random_device rd;
	//Rozk³ad do wyboru osobnika oraz prawdopodobieñstwo mutacji
	uniform_int_distribution<unsigned long long> specimen(0, population - 1);
	uniform_real_distribution<double> chance(0.0, 1.0);

	//Najlepszy wynik:
	vector<unsigned int> best(n);
	int best_fitness = infinity;

	//Pokolenie + nastêpne pokolenie:
	vector<vector<unsigned int>> generation(population);
	vector<vector<unsigned int>> next_generation(population);
	//Ocena populacji:
	vector<int> generation_fitness(population);
	//Inicjalizacja populacji + wybór wstêpnego najlepszego osobnika:
	for (unsigned long long i = 0; i < population; i++)
	{
		generation[i] = vector<unsigned int>(n);
		next_generation[i] = vector<unsigned int>(n);
		first_permutation(generation[i]);
		new_permutation(generation[i]);
		generation_fitness[i] = path_length(distances, generation[i]);
		if (generation_fitness[i] < best_fitness)
		{
			best_fitness = generation_fitness[i];
			best = generation[i];
		}
	}

	//Iterujemy przez pokolenia przez zadany czas:
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < time)
	{
		//Zliczanie osobników do kolejnego pokolenia
		for (unsigned long long processed = 0; processed < population; processed += 2)
		{
			//Turniej elitarny aby ustaliæ rodziców:
			unsigned long long candidate_a1 = specimen(rd);
			unsigned long long candidate_a2 = specimen(rd);
			unsigned long long candidate_b1 = specimen(rd);
			unsigned long long candidate_b2 = specimen(rd);
			unsigned long long parent_a =
				generation_fitness[candidate_a1] > generation_fitness[candidate_a2] ?
				candidate_a1 :
				candidate_a2;
			unsigned long long parent_b =
				generation_fitness[candidate_b1] > generation_fitness[candidate_b2] ?
				candidate_b1 : 
				candidate_b2;

			if (chance(rd) < crossover)
			{
				//Dzieci przechodz¹ do nastêpnego pokolenia:
				crossover_type(
					generation[parent_a],
					generation[parent_b],
					next_generation[processed + 0],
					next_generation[processed + 1],
					rd
				);
			}
			else
			{
				//Rodzice przechodz¹ do kolejnego pokolenia:
				copy_permutation(next_generation[processed + 0], generation[parent_a]);
				copy_permutation(next_generation[processed + 1], generation[parent_b]);
			}
			//Dzieci/rodzice mog¹ podlegaæ mutacji:
			//if (chance(rd) < mutation)
				mutation_type(next_generation[processed + 0], mutation, rd);
			//if (chance(rd) < mutation)
				mutation_type(next_generation[processed + 1], mutation, rd);
		}
		//Przejœcie do nowego pokolenia + sprawdzenie czy jest nowe najlepsze rozwi¹zanie:
		for (unsigned long long i = 0; i < population; i++)
		{
			copy_permutation(generation[i], next_generation[i]);
			generation_fitness[i] = path_length(distances, generation[i]);
			if (generation_fitness[i] < best_fitness)
			{
				best_fitness = generation_fitness[i];
				best = generation[i];
			}
		}
	}
	//Zwracamy najlepsze znalezione rozwi¹zanie:
	return best;
}