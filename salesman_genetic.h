#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>

#include "city.h"
#include "salesman_utility.h"

void mutation_a(vector<unsigned int>& genome, double p, random_device& rd);
void mutation_b(vector<unsigned int>& genome, double p, random_device& rd);
void crossover_a(
	const vector<unsigned int>& parent_a,
	const vector<unsigned int>& parent_b,
	vector<unsigned int>& child_a,
	vector<unsigned int>& child_b,
	random_device& rd);

vector<unsigned int> salesman_genetic(
	vector<city> locations,
	void(*mutation_type)(vector<unsigned int>& genome,double p, random_device& rd) = mutation_a,
	void(*crossover_type)(
		const vector<unsigned int>& parent_a,
		const vector<unsigned int>& parent_b,
		vector<unsigned int>& child_a,
		vector<unsigned int>& child_b,
		random_device& rd) = crossover_a,
	unsigned long long time = 60000,
	unsigned long long population = 512,
	double crossover = 0.8,
	double mutation = 0.01);
vector<unsigned int> salesman_genetic(
	vector<vector<int>> distances,
	void(*mutation_type)(vector<unsigned int>& genome, double p, random_device& rd) = mutation_a,
	void(*crossover_type)(
		const vector<unsigned int>& parent_a,
		const vector<unsigned int>& parent_b,
		vector<unsigned int>& child_a,
		vector<unsigned int>& child_b,
		random_device& rd) = crossover_a,
	unsigned long long time = 60000,
	unsigned long long population = 512,
	double crossover = 0.8,
	double mutation = 0.01);