#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <chrono>

#include "city.h"
#include "salesman_utility.h"

vector<unsigned int> salesman_annealing(
	vector<city> locations,
	random_device& rd,
	long long limit = 10,
	double min_temp = 0.0,
	unsigned int samples = 10000,
	double cooling = 0.99);
vector<unsigned int> salesman_annealing(
	vector<vector<int>> distances,
	random_device& rd,
	long long limit = 10,
	double min_temp = 0.0,
	unsigned int samples = 10000,
	double cooling = 0.99);
