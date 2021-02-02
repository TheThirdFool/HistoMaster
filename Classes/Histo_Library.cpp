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

// Histo(double * a, double * b, int no)  -  Constructor
// Read(FILE * infile, int BytesTotal)    -  Read data from file












// == INCLUDES ==
#include "Histo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zlib.h"

// == CODE ==




// DFH - Constructor copying across pointers into Coord
Histo::Histo(double * a, double * b, int no){
	X = (double*)malloc(no * sizeof(double));
	Y = (double*)malloc(no * sizeof(double));

	memcpy(X, a, no * sizeof(double));
	memcpy(Y, b, no * sizeof(double));
	NoBins = no;
}


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
	
//	unsigned char Output[LenOutput];
	
//	printf("The input  is %lu bytes long. Given as %i .\n",sizeof(Input), LenInput);
//	printf("The output is %lu bytes long. Given as %i .\n",sizeof(Output), LenOutput);
//	printf("Input is: %.15s\n", Input);
	
	// STEP 2.
	// inflate b into c
	// zlib struct
//	z_stream infstream;
//	infstream.zalloc = 0;
//	infstream.zfree  = 0;
//	infstream.opaque = 0;
//	
//	
//	// setup "b" as the input and "c" as the compressed output
//	infstream.avail_in = (uInt)LenInput; // size of input
//	infstream.next_in = Input; // input char array
//	infstream.avail_out = (uInt)LenOutput; // size of output
//	infstream.next_out = Output; // output char array
//	
//	
//	int wbits = 18;
//	
//	int ret;
//	
//	// the actual DE-compression work.
//	ret = inflateInit2(&infstream, -15);//, wbits);
//	printf("ZLIB Output ->->-> %s\n", infstream.msg);
//	
//	// https://github.com/klauspost/compress 
//	// My saviour??? We will see
//	
//	printf("ret = %i\n", ret);
//	
//	inflate(&infstream, 4);
//	printf("ZLIB Output ->->-> %s\n", infstream.msg);
//	inflateEnd(&infstream);
//	 
//	printf("ZLIB Output ->->-> %s\n", infstream.msg);
	
//	printf("Uncompressed size is: %lu\n", sizeof(Output));
//	printf("Uncompressed string is: %.15s\n", Output);
//	printf("Full Output is:\n");
//	for(int ik = 0; ik < 30; ik++) printf("%c", Output[ik]);
//	printf("\n");
	
	//	printf("%s",Output);
	
	//================================================================================================
	//================================================================================================
	//================================================================================================
	BytesTotal = BytesTotal + BytesRead;
	
	printf("Total bytes read = %i\n\n", BytesRead);	

	return BytesRead;

}















