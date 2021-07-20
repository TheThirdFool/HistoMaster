// HGraph library

// == INCLUDES ==
#include "HGraph.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zlib.h"
//#include "lz4.h"
#include <vector>
#include <string.h>

#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>

// == CODE ==



// DFH - Constructor clearing the histo class
HGraph::HGraph(){
	X.clear();
	Y.clear();
	
	lName  = '\0';
	lTitle = '\0';
	Name[0]  = '\0';
	Title[0] = '\0';
	memcpy(Type,"????",4);

}


int HGraph::Draw(){

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts;
	for(int i=0; i < X.size(); i++) {
		//printf("%f, %f\n", X[i], Y[i]);
		xy_pts.push_back(std::make_pair(X[i], Y[i]));
	}


	double low = X[0];
	for(int i=0; i < X.size(); i++){
		if(X[i] < low) low = X[i];
	}

	double high = X[0];
	for(int i=0; i < X.size(); i++){
		if(X[i] < high) high = X[i];
	}
	

	// Find max y:
	double y_max = GetMax();

	// Find min y:
	double y_min = Y[0];
	for(int i=0; i < Y.size(); i++){
		if(Y[i] < y_min) y_min = Y[i];
	}

	y_max = y_max + 0.05 * (y_max - y_min); 
	y_min = y_min - 0.05 * (y_max - y_min); 



	char char_buffer [128];
	sprintf(char_buffer, "set xrange [%f:%f]\nset yrange [%f:%f]\n", low, high, y_min, y_max);

	gp << char_buffer; 
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines title 'X'\n";
	gp.send1d(xy_pts);
	//gp.send1d(Y);
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();

	return 1;

}

int HGraph::Draw(double low = -10101.01, double high = -10101.01){

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts;
	for(int i=0; i < X.size(); i++) {
		//printf("%f, %f\n", X[i], Y[i]);
		xy_pts.push_back(std::make_pair(X[i], Y[i]));
	}

	if(high == -10101.01){
		high = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] > high) high = X[i];
		}
	}

	if(low == -10101.01){
		low = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] < low) low = X[i];
		}
	}


	// Find max y:
	double y_max = 0.0;
	for(int i=0; i < Y.size(); i++){
		if(X[i] < low)  continue;
		if(X[i] > high) continue;
		if(Y[i] > y_max) y_max = Y[i];
	}


	// Find min y:
	double y_min = Y[0];
	for(int i=0; i < Y.size(); i++){
		if(X[i] < low)  continue;
		if(X[i] > high) continue;
		if(Y[i] < y_min) y_min = Y[i];
	}

	y_max = y_max + 0.05 * (y_max - y_min); 
	y_min = y_min - 0.05 * (y_max - y_min); 



	char char_buffer [128];
	sprintf(char_buffer, "set xrange [%f:%f]\nset yrange [%f:%f]\n", low, high, y_min, y_max);

	gp << char_buffer; 
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines title 'X'\n";

	gp.send1d(xy_pts);
	//gp.send1d(Y);
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();

	return 1;

}


int HGraph::Draw(double low = -10101.01, double high = -10101.01, int opt = 0){

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts;
	for(int i=0; i < X.size(); i++) {
		//printf("%f, %f\n", X[i], Y[i]);
		xy_pts.push_back(std::make_pair(X[i], Y[i]));
	}

	if(high == -10101.01){
		high = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] > high) high = X[i];
		}
	}

	if(low == -10101.01){
		low = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] < low) low = X[i];
		}
	}


	// Find max y:
	double y_max = Y[0];
	for(int i=0; i < Y.size(); i++){
		if(X[i] < low)  continue;
		if(X[i] > high) continue;
		if(Y[i] > y_max) y_max = Y[i];
	}

	// Find min y:
	double y_min = Y[0];
	for(int i=0; i < Y.size(); i++){
		if(X[i] < low)  continue;
		if(X[i] > high) continue;
		if(Y[i] < y_min) y_min = Y[i];
	}

	y_max = y_max + 0.05 * (y_max - y_min); 
	y_min = y_min - 0.05 * (y_max - y_min); 



	char char_buffer [128];
	sprintf(char_buffer, "set xrange [%f:%f]\nset yrange [%f:%f]\n", low, high, y_min, y_max);

	gp << char_buffer; 
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	if(opt == 1){
		gp << "replot '-' with lines title 'X'\n";
	} else {
		gp << "plot '-' with lines title 'X'\n";
	}

	gp.send1d(xy_pts);
	//gp.send1d(Y);
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();

	return 1;

}

int HGraph::SetPoint(double x_point, double y_point){

	for(int i = 0; i < X.size(); i++){
		if(X[i] == x_point){
			Y[i] = y_point;
			return 1;
		}
	}
	
	X.push_back(x_point);
	Y.push_back(y_point);
	
	return 1;
}


double HGraph::GetMax(){

	double max = Y[0];

	for(int i = 0; i < X.size(); i++){
		if(Y[i] > max) max = Y[i];
	}

	return max;
}


int HGraph::Normalise(){

	double max = GetMax();

	for(int i = 0; i < X.size(); i++){
		Y[i] = Y[i] / max;
	}

	return 1;
}










