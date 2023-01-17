#pragma once
#include <cmath>
#include "city.h"

//Policz odleg³oœæ miêdzy punktami
double segment_length(const city& c1, const city& c2)
{
	//odleg³oœæ Euklidesa dla 2 wymiarów
	double seg = (c1.x - c2.x) * (c1.x - c2.x);
	seg += (c1.y - c2.y) * (c1.y - c2.y);
	return sqrt(seg);
}

//Policz d³ugoœæ œcie¿ki - tj. ewaluacja wyniku
double path_length(const vector<city>& locations, const vector<unsigned int>& order)
{
	// odleg³oœæ miêdzy ostatnim i pierwszym miastem, aby zakoñczyæ pêtlê
	double path = segment_length(locations[order[locations.size() - 1]], locations[order[0]]);
	// odleg³oœci miêdzy kolejnymi miastami na œcie¿ce
	for (unsigned int i = 1; i < locations.size(); i++)
		path += segment_length(locations[order[i - 1]], locations[order[i]]);
	return path;
}

//Policz d³ugoœæ œcie¿ki dla obliczonych wczeœniej odleg³oœci
double path_length(const vector<vector<double>>& distances, const vector<unsigned int>& order)
{
	double path = distances[order[order.size() - 1]][order[0]];
	for (unsigned int i = 1; i < order.size(); i++)
		path += distances[order[i - 1]][order[i]];
	return path;
}

//Policz macierz odleg³oœci aby uproœciæ obliczenia
vector<vector<double>> precalculate_distances(const vector<city>& locations)
{
	// zbuduj macierz
	vector<vector<double>> r(locations.size());
	for (unsigned int i = 0; i < locations.size(); i++)
		r[i] = vector<double>(locations.size());
	
	// macierz jest symetryczna wzglêdem przek¹tnej wiêc trzeba policzyæ tylko po³owê wartoœci
	for (unsigned int i = 0; i < locations.size(); i++)
		for (unsigned int j = i + 1; j < locations.size(); j++)
			r[i][j] = r[j][i] = segment_length(locations[i], locations[j]);

	return r;
}
