
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
	//histTest.ReadTXT(infile); 
	histTest.ReadCNF(infile); 

	//============================================= Analyse Data

//	double peak_sum = histTest.Integrate(2.,4.); 	
//	double back_sum = histTest.Integrate(0.,2.); 	
	double peak_sum = histTest.Integrate(1000.,1013.); 	
	double back_sum = histTest.Integrate(2137.,2300.); 	

	double fullsum = histTest.Integrate(); 


	//histTest.Fit((char*)"gaus");

	printf("peak_sum == %f\nback_sum == %f\nfullsum == %f\n",peak_sum,back_sum,fullsum);

	histTest.Draw();

	fclose(infile);

	return 1;

}

#endif


































