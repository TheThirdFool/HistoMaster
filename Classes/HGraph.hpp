// A dumb stupid class because doing easy recursion is legit too hard for this stupid language.


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

#include "HMatrix.hpp"

#ifndef HGRAPH_H
#define HGRAPH_H



class HGraph{

	public:

	std::vector<double> X;
	std::vector<double> Y;

	unsigned char lName;
	char Name[256];
	unsigned char lTitle;
	char Title[256];
	
	char Type[4];

	// Destructor
	~HGraph(){}

	// FUNCTIONS ====
	HGraph(); // Declared in HGraph_Library.cpp

	int Draw();
	int Draw(double low, double high);
	int Draw(double low, double high, int opt);

	int SetPoint(double x_point, double y_point);

	double GetMax();
	int Normalise();
	
};


#endif
