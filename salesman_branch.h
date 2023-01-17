#pragma once
#include <vector>
#include <iostream>

#include "city.h"
#include "salesman_utility.h"

void salesman_branch(unsigned long long visited, unsigned int current,
	double current_path,
	const unsigned int n, const unsigned long long m,
	const vector<vector<int>>& distance,
	vector<unsigned int>& candidate,
	vector<unsigned int> partial,
	int& upper_bound_path,
	const unsigned long long completed);
vector<unsigned int> salesman_branch(vector<vector<int>> distances,
	vector<unsigned int>& candidate,
	int& upper_bound_path);
bool order_comparator(const pair<unsigned int, double>& a, const pair<unsigned int, double>& b);
vector<unsigned int> salesman_branch(vector<city> locations, bool version = 0);
vector<unsigned int> salesman_branch(vector<vector<int>> distances);