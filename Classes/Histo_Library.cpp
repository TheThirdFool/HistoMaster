// This is the Histo Class Library. Add Functions as you come up with them

// THE RULES:
// ==========
//
// 1) Work on your own branch before making perminant changes
// 2) Document everything you do - both in comments and in the user manual
// 3) Be careful with repetition. Try to optimise for speed!
// 4) Sign everything you do!
//
// Thanks - Dan


// FUNCTION LIST:

// Histo()  -  Constructor
// int Read(FILE * infile, int BytesTotal)  -  Read data from file
// int ReadTXT(FILE * infile)               -  Read data from a txt file
// int ReadString(char* String, int row, double * dat) - Split string into doubles 
// int Histo::HexToInt(unsigned char * Hex, int num)   - Read hex number - return int
// double Integrate()                        - Return whole hist integral
// double Integrate(double low, double high) - Return integral within bounds










// == INCLUDES ==
#include "Histo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zlib.h"
#include <vector>

// == CODE ==



// DFH - Constructor clearing the histo class
Histo::Histo(){
	X.clear();
	Y.clear();
}


// DFH - Read in a hexidecimal number and return an integer
int Histo::HexToInt(unsigned char * Hex, int num){
	int fin = 0;
	for(int i = 1; i <= num; i++){
		fin += pow(16, (i-1)*2) * Hex[num - i]; 
	}
	return fin;
}


// DFH - Read from the infile pointer. Can't decode data yet - but large memory leak fix may have cleared the way for this.
int Histo::Read(FILE *infile, int BytesTotal){ 

	int BytesRead = 0;
	KeyHeader Header2;
	BytesRead = Header2.Read(infile);
	Header2.Print();
	
	printf("Bytes read = %i\n", BytesRead);	
	printf("Total bytes read = %i\n\n", BytesTotal);	


	//================================================================================================
	//========================================================================= THIS AREA IS BROKEN ==
	//================================================================================================
	
	//	printf("H2.NBytes = %i\nH2.KeyLen = %i\n", Header2.NBytes, Header2.KeyLen);
	
	
	unsigned char Input[Header2.NBytes - Header2.KeyLen];
	//unsigned char JunkBuffer[16];
	
	//int check1 = fread(JunkBuffer, sizeof(JunkBuffer), 1, infile);
	//int check = fread(Input, sizeof(Input), 1, infile);
	int check = fread(&Input, sizeof(unsigned char), (Header2.NBytes - Header2.KeyLen), infile);
	BytesRead = BytesRead + (Header2.NBytes - Header2.KeyLen);	


	printf("Size of Input outside inflate = %lu\n", sizeof(Input));
	printf("Size of check = %i\n", check);
	printf("Input is:\n");
	for(int ik = 0; ik < 20; ik++) printf("%c", Input[ik]);
	printf("\n");
	
	//	InflateData(&Input, (Header2.NBytes - Header2.KeyLen), Header2.ObjLen);
	
	int LenInput  = Header2.NBytes - Header2.KeyLen;
	int LenOutput = Header2.ObjLen - Header2.KeyLen;
	
	unsigned char Output[LenOutput];
	Output[0] = '\0';
  
	printf("The input  is %lu bytes long. Given as %i .\n",sizeof(Input), LenInput);
	printf("The output is %lu bytes long. Given as %i .\n",sizeof(Output), LenOutput);
	printf("Input is: %.15s\n", Input);
  
  // STEP 2.
  // inflate b into c
  // zlib struct
	z_stream infstream;
	infstream.zalloc = 0;
	infstream.zfree  = 0;
	infstream.opaque = 0;
	
	
	// setup "b" as the input and "c" as the compressed output
	infstream.avail_in = (uInt)LenInput; // size of input
	infstream.next_in = Input; // input char array
	infstream.avail_out = (uInt)LenOutput; // size of output
	infstream.next_out = Output; // output char array
	
	
	int wbits = 18;
	
	int ret;
	
	// the actual DE-compression work.
	ret = inflateInit(&infstream);//, wbits);
	printf("ZLIB Output ->->-> %s\n", infstream.msg);
	
	// https://github.com/klauspost/compress 
	// My saviour??? We will see
	
	printf("ret = %i\n", ret);
	
	inflate(&infstream, 4);
	printf("ZLIB Output ->->-> %s\n", infstream.msg);
	inflateEnd(&infstream);
	 
	printf("ZLIB Output ->->-> %s\n", infstream.msg);
  
	printf("Uncompressed size is: %lu\n", sizeof(Output));
	printf("Uncompressed string is: %.15s\n", Output);
	printf("Full Output is:\n");
	for(int ik = 0; ik < LenOutput; ik++) printf(" | %i:%u ",ik, Output[ik]);
	printf("\n");
	
	//	printf("%s",Output);
	
	//================================================================================================
	//================================================================================================
	//================================================================================================
	BytesTotal = BytesTotal + BytesRead;
	
	printf("Total bytes read = %i\n\n", BytesRead);	

	return BytesRead;

}


// DFH - Read a "x,y" string and return X and Y 
int Histo::ReadString(char* String, int row, double * dat){
	char * Holder = NULL;
	char *pEnd;

	dat[0] = strtod(String, &pEnd);
	pEnd++;
	dat[1] = strtod(pEnd, NULL);
	return 1;
}


// DFH - Read a .txt file and fill vectors
int Histo::ReadTXT(FILE * infile){
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	// Loop over all the lines in the file
	int  i = 0;
	while((read = getline(&line, &len, infile)) != -1) {
		// Reat the trace data
		double dat[2];
		ReadString(line, 1, dat);	

		X.push_back(dat[0]);
		Y.push_back(dat[1]);	
		i++;
	}

	return i;
}




// ==== DATA ANALYSIS ====


// PD - Return the integral of the entire Histo
double Histo::Integrate(){
	double sum = 0.0; 
	for(int i =0; i<X.size();i++){
		sum += Y[i]; 
	}
	return sum; 
}


// PD - Return the integral of the Histo within bounds low - high
double Histo::Integrate(double low, double high){
	int start = 0;  
	double sum_r = 0.0; 

	for(int j = 0; j<X.size() ;j++){ 
		if(X[j] >= low){
			start = j; 
			printf("[%.02f] at bin = %i\n",low,j);
			break;
		}
	}
	for(int j = start; j<X.size();j++){ 
		
		sum_r += Y[j]; 

		if(X[j] >= high){
			printf("[%.02f] at bin = %i\n",high,j);
			break;  	
		}
	}
	
	return sum_r; 
}







