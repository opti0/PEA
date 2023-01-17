#pragma once
#include <algorithm>
#include <vector>
#include <iostream>

#include "city.h"
#include "salesman_utility.h"

vector<unsigned int> salesman_bruteforce(vector<vector<int>> distances);
vector<unsigned int> salesman_bruteforce(vector<city> locations);