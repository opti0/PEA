#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;

struct city
{
	string name;
	double x = 0.0;
	double y = 0.0;
};
vector<city> generate_cities(unsigned int n,
	double x_min, double x_max,
	double y_min, double y_max,
	random_device& rd);
vector<city> read_cities(istream& in);
vector<vector<int>> read_distances(istream& in);
template<class T>
ostream& operator<< (ostream& out, const vector<T>& v)
{
	bool not_first = false;
	out << "[";
	for (T i : v)
	{
		if (not_first) out << ", ";
		out << i;
		not_first = true;
	}
	out << "]";
	return out;
}
ostream& operator<< (ostream& out, const city& c);