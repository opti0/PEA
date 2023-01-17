#pragma once
#include "salesman_utility.h"
#include <cmath>
#include "city.h"

void first_permutation(vector<unsigned int>& order)
{
	for (unsigned int i = 0; i < order.size(); i++)
		order[i] = i;
}
void new_permutation(vector<unsigned int>& order)
{
	unsigned long long n = order.size();
	unsigned long long n_minus = n - 1;
	for (unsigned long long i = n_minus; i > 1; i--)
		swap(order[i], order[(unsigned long long)rand() % i + 1]);
}
void copy_permutation(vector<unsigned int>& dest, vector<unsigned int>& src)
{
	for (unsigned int i = 0; i < src.size(); i++)
		dest[i] = src[i];
}

//Policz odleg³oœæ miêdzy punktami
int segment_length(const city& c1, const city& c2)
{
	//odleg³oœæ Euklidesa dla 2 wymiarów
	double seg = (c1.x - c2.x) * (c1.x - c2.x);
	seg += (c1.y - c2.y) * (c1.y - c2.y);
	return max((int)sqrt(seg), 1);
}

//Policz d³ugoœæ œcie¿ki - tj. ewaluacja wyniku
int path_length(const vector<city>& locations, const vector<unsigned int>& order)
{
	// odleg³oœæ miêdzy ostatnim i pierwszym miastem, aby zakoñczyæ pêtlê
	int path = segment_length(locations[order[locations.size() - 1]], locations[order[0]]);
	// odleg³oœci miêdzy kolejnymi miastami na œcie¿ce
	for (unsigned int i = 1; i < locations.size(); i++)
		path += segment_length(locations[order[i - 1]], locations[order[i]]);
	return path;
}

//Policz d³ugoœæ œcie¿ki dla obliczonych wczeœniej odleg³oœci
int path_length(const vector<vector<int>>& distances, const vector<unsigned int>& order)
{
	int path = distances[order[order.size() - 1]][order[0]];
	for (unsigned int i = 1; i < order.size(); i++)
		path += distances[order[i - 1]][order[i]];
	return path;
}

//Policz macierz odleg³oœci aby uproœciæ obliczenia
vector<vector<int>> precalculate_distances(const vector<city>& locations)
{
	// zbuduj macierz
	vector<vector<int>> r(locations.size());
	for (unsigned int i = 0; i < locations.size(); i++)
		r[i] = vector<int>(locations.size());

	// macierz jest symetryczna wzglêdem przek¹tnej wiêc trzeba policzyæ tylko po³owê wartoœci
	for (unsigned int i = 0; i < locations.size(); i++)
		for (unsigned int j = i + 1; j < locations.size(); j++)
			r[i][j] = r[j][i] = segment_length(locations[i], locations[j]);

	return r;
}
