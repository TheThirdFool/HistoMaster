
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
	histTest.Draw();
	double peak_sum = histTest.Integrate(); 	
	printf("Integral = %f\n", peak_sum);


	histTest.Draw(1000.,1500.);
	peak_sum = histTest.Integrate(1000., 1500.); 	
	printf("Integral = %f\n", peak_sum);


	histTest.Draw(1400.,1500.);
	peak_sum = histTest.Integrate(1400., 1500.); 	
	printf("Integral = %f\n", peak_sum);

	//============================================= Analyse Data

	double back_sum = histTest.Integrate(0.,2.); 	

	double fullsum = histTest.Integrate(); 


	//histTest.Fit((char*)"gaus");

	printf("peak_sum == %f\nback_sum == %f\nfullsum == %f\n",peak_sum,back_sum,fullsum);


	fclose(infile);

	return 1;

}

#endif


































