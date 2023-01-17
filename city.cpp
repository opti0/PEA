#pragma once
#include "city.h"
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;

// funkcja generuje losowy wektor miast, o zadanej d³ugoœci oraz zakresach wartoœci x i y
// funkcja opiera siê o przekazanie silnika pseudolosowego
vector<city> generate_cities(unsigned int n,
	double x_min, double x_max,
	double y_min, double y_max,
	random_device& rd)
{
	//przygotowujemy generatory pseudolosowe
	mt19937 gen(rd());
	uniform_real_distribution<double> x_dist(x_min, x_max);
	uniform_real_distribution<double> y_dist(y_min, y_max);
	//przygotowujemy listê zwrotn¹
	vector<city> r(n);
	//wype³niamy listê danymi
	for (city& c : r)
	{
		c.name = "random";
		c.x = x_dist(gen);
		c.y = y_dist(gen);
	}
	return r;
}

//funkcja wczyta wektor miast z Ÿródla typu istream
vector<city> read_cities(istream& in)
{
	if (not in)
	{
		throw exception("Blad IO - strumien do czytania nie jest poprawny");
	}
	unsigned int n;
	in >> n;
	vector<city> r(n);
	for (city& c : r)
	{
		in >> c.name;
		in >> c.x;
		in >> c.y;
	}
	if (not in)
	{
		throw exception("Blad IO - nastapil blad przy czytaniu z strumienia");
	}
	return r;
}

//czytanie macierzy odleglosci z elementu typu istream
vector<vector<int>> read_distances(istream& in)
{
	if (not in)
	{
		throw exception("Blad IO - strumien do czytania nie jest poprawny");
	}
	int n;
	in >> n;
	vector<vector<int>> distances(n);
	for (int i = 0; i < n; i++)
	{
		distances[i] = vector<int>(n);
		for (int j = 0; j < n; j++)
			in >> distances[i][j];
	}
	if (not in)
	{
		throw exception("Blad IO - nastapil blad przy czytaniu z strumienia");
	}
	return distances;
}

ostream& operator<< (ostream& out, const city& c)
{
	out << c.name << "(" << c.x << ", " << c.y << ")";
	return out;
}