#pragma once
#include <vector>
#include <iostream>
#include <limits>

#include "city.h"
#include "salesman_utility.h"

int salesman_dynamic(unsigned long long visited, unsigned int current,
	const vector<vector<int>>& distance,
	vector<vector<int>>& dynamic);
vector<unsigned int> salesman_dynamic(vector<vector<int>> distance);
vector<unsigned int> salesman_dynamic(vector<city> locations);