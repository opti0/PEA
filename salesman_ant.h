#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <cmath>

#include "city.h"
#include "salesman_utility.h"

vector<unsigned int> salesman_ant(
	vector<city> locations,
	long long iterations = 1000,
	long long ants = 50,
	float pheromon = 10.0,
	float evaporation = 0.9);
vector<unsigned int> salesman_ant(
	vector<vector<int>> distances,
	long long iterations = 1000,
	long long ants = 50,
	float pheromon = 10.0,
	float evaporation = 0.9);