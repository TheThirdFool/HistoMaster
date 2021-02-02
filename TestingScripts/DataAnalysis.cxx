
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

	histTest.ReadTXT(infile); 

	double peak_sum = histTest.Integrate(2.,4.); 	
	double back_sum = histTest.Integrate(0.,2.); 	

	double fullsum = histTest.Integrate(); 

	printf("peak_sum == %f\nback_sum == %f\nfullsum == %f\n",peak_sum,back_sum,fullsum);

	fclose(infile);

	return 1;

}

#endif
