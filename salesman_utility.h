#pragma once
#include <cmath>
#include "city.h"

void first_permutation(vector<unsigned int>& order);
void new_permutation(vector<unsigned int>& order);
void copy_permutation(vector<unsigned int>& dest, vector<unsigned int>& src);

int segment_length(const city& c1, const city& c2);
int path_length(const vector<city>& locations, const vector<unsigned int>& order);
int path_length(const vector<vector<int>>& distances, const vector<unsigned int>& order);
vector<vector<int>> precalculate_distances(const vector<city>& locations);
