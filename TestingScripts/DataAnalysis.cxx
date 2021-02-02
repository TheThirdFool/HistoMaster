
// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Histomaster includes: 
#include "../Classes/Histo.hpp"


// == FUNCTIONS ==




#ifndef __CINT__ 

// == MAIN ==

int main(int argc,char **argv){

	//============================================= Open File
	FILE *infile  = fopen(argv[1], "rb"); 

	//============================================= Read Data


	Histo histTest;

	fclose(infile);

	return 1;

}

#endif
