#pragma once
#include <cmath>
#include "city.h"

//Policz odleg�o�� mi�dzy punktami
double segment_length(const city& c1, const city& c2)
{
	//odleg�o�� Euklidesa dla 2 wymiar�w
	double seg = (c1.x - c2.x) * (c1.x - c2.x);
	seg += (c1.y - c2.y) * (c1.y - c2.y);
	return sqrt(seg);
}

//Policz d�ugo�� �cie�ki - tj. ewaluacja wyniku
double path_length(const vector<city>& locations, const vector<unsigned int>& order)
{
	// odleg�o�� mi�dzy ostatnim i pierwszym miastem, aby zako�czy� p�tl�
	double path = segment_length(locations[order[locations.size() - 1]], locations[order[0]]);
	// odleg�o�ci mi�dzy kolejnymi miastami na �cie�ce
	for (unsigned int i = 1; i < locations.size(); i++)
		path += segment_length(locations[order[i - 1]], locations[order[i]]);
	return path;
}

//Policz d�ugo�� �cie�ki dla obliczonych wcze�niej odleg�o�ci
double path_length(const vector<vector<double>>& distances, const vector<unsigned int>& order)
{
	double path = distances[order[order.size() - 1]][order[0]];
	for (unsigned int i = 1; i < order.size(); i++)
		path += distances[order[i - 1]][order[i]];
	return path;
}

//Policz macierz odleg�o�ci aby upro�ci� obliczenia
vector<vector<double>> precalculate_distances(const vector<city>& locations)
{
	// zbuduj macierz
	vector<vector<double>> r(locations.size());
	for (unsigned int i = 0; i < locations.size(); i++)
		r[i] = vector<double>(locations.size());
	
	// macierz jest symetryczna wzgl�dem przek�tnej wi�c trzeba policzy� tylko po�ow� warto�ci
	for (unsigned int i = 0; i < locations.size(); i++)
		for (unsigned int j = i + 1; j < locations.size(); j++)
			r[i][j] = r[j][i] = segment_length(locations[i], locations[j]);

	return r;
}
