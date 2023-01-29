#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <cmath>
#include <chrono>

#include "city.h"
#include "salesman_utility.h"

vector<unsigned int> salesman_ant(
	vector<city> locations,
	unsigned long long time = 60000,
	unsigned long long ants = 50,
	float pheromon = 10.0,
	float evaporation = 0.9);
vector<unsigned int> salesman_ant(
	vector<vector<int>> distances,
	unsigned long long time = 60000,
	unsigned long long ants = 50,
	float pheromon = 10.0,
	float evaporation = 0.9);