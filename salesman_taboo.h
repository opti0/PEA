#pragma once
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
	long long limit = 10);
vector<unsigned int> salesman_taboo(
	vector<vector<int>> distances,
	random_device& rd,
	long long limit = 10);
