
// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// Histomaster includes: 
#include "Classes/Histo.hpp"


// ROOT includes:
#include <TH1D.h>
#include <TFile.h>



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


	int nbins  = histTest.X.size();
	double min = histTest.GetMinX();	
	double max = histTest.GetMaxX();	
	double peak_sum = histTest.Integrate(); 	
	printf("Integral = %f\n", peak_sum);
	printf("Max = %f\n", max);
	printf("Min = %f\n", min);
	printf("NBins = %i\n", nbins);

	histTest.Draw();


	char fname[64];
	// You cna do better naming schemes here if you want to get the name from the input file.
	sprintf(fname, "OutputFile_CNF2ROOT.root");

	printf("Now making the ROOT file '%s'...\n", fname);

	TFile * outFile = new TFile(fname, "RECREATE");

	char    hname[64];
	sprintf(hname, "outputHist_HM");
	char    htitle[64];
	sprintf(htitle, "CNF to ROOT from HM");

	TH1D * output_h = new TH1D(hname, htitle, nbins, min, max);
		
	for(int i = 0; i < histTest.X.size(); i++){
		output_h -> SetBinContent(i, histTest.Y[i]);
	}

	output_h->Write();
	outFile->Close();
	fclose(infile);

	return 1;

}

#endif


































