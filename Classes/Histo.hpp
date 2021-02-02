
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

#include "KeyHeader.hpp"

#ifndef HISTO_H
#define HISTO_H



class Histo{

	public:

	// Empty Constructor
	Histo(){}


	// Destructor
	~Histo(){}


	// FUNCTIONS ====

	Histo(double * a, double * b, int no);
	int Read(FILE * infile, int BytesTotal);
	


	private:
		
		double * X;
		double * Y;
		int NoBins;

};


#endif


