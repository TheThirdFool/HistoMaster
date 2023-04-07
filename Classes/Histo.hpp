
// HISTO CLASS

// RULES:
// ======
//
// 1) No public data - we only access the data in a secure manner.
//
// 2) Be spareing. At some point this will likely have to be made 
//    more effient - if it can be a long, make it a long. Save 
//    memory when possible!
// 
// 3) Sign everything you do. This is the main Histo class. We need
//    this to work well!
//
// TY - DFH

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

#include "HGraph.hpp"
//#include "HGraph_Library.cpp"
#include "KeyHeader.hpp"
#include "HMatrix.hpp"
//#include "HistoGUI.hpp"
//#include "HistoGUI_Library.cpp"

#ifndef HISTO_H
#define HISTO_H



class Histo{

	public:

	std::vector<double> X;
	std::vector<double> Y;
	std::vector< std::vector<double> > Z;

	unsigned char lName;
	char Name[256];
	unsigned char lTitle;
	char Title[256];
	
	char Type[4];
	
	double x_axis_max; 
	double x_axis_min; 

	// Destructor
	~Histo(){}

	// FUNCTIONS ====
	Histo(); // Declared in Histo_Library.cpp
	int Read(FILE * infile, int BytesTotal);
	int ReadTXT(FILE * infile);
	int ReadCNF(FILE * infile);
	int ReadFlex(char* filename);

	int Fit(char * func, int noIterations = 5);
	
	double Integrate();
	double Integrate(double low, double high);
	int Draw();
	int Draw(double low, double high);
	int Draw(double low, double high, int opt);
	int MakeProbGraph(double low, double high, double low_l, double high_l, double scale);
	int SetPoint(double x_point, double y_point);
	
	double GetMaxX();
	double GetMinX();
	int GetNBins(){ return NoBins; }

	private:
		
	int NoBins;


	double CNF_Energy(int x, float * A);
	int ReadString(char* String, int row, double * dat); 
	int HexToInt(unsigned char * Hex, int num);
	int GS_Process(HMatrix * old_matrix);
	int Proj(double * A, double * B, int NoRows, double * ret);
	double GetP(double l);
	double GetZ(double l, double low, double high);

};


#endif


