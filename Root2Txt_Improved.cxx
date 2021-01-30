/*
 A ROOT file is a suite of consecutive data records (TKey's) with
 the following format (see also the TKey class). If the key is
 located past the 32 bit file limit (> 2 GB) then some fields will
 be 8 instead of 4 bytes:
    1->4            Nbytes    = Length of compressed object (in bytes)
    5->6            Version   = TKey version identifier
    7->10           ObjLen    = Length of uncompressed object
    11->14          Datime    = Date and time when object was written to file
    15->16          KeyLen    = Length of the key structure (in bytes)
    17->18          Cycle     = Cycle of key
    19->22 [19->26] SeekKey   = Pointer to record itself (consistency check)
    23->26 [27->34] SeekPdir  = Pointer to directory header
    27->27 [35->35] lname     = Number of bytes in the class name
    28->.. [36->..] ClassName = Object Class Name
    ..->..          lname     = Number of bytes in the object name
    ..->..          Name      = lName bytes with the name of the object
    ..->..          lTitle    = Number of bytes in the object title
    ..->..          Title     = Title of the object
    ----->          DATA      = Data bytes associated to the object

 The first data record starts at byte fBEGIN (currently set to kBEGIN).
 Bytes 1->kBEGIN contain the file description, when fVersion >= 1000000
 it is a large file (> 2 GB) and the offsets will be 8 bytes long and
 fUnits will be set to 8:
    1->4            "root"      = Root file identifier
    5->8            fVersion    = File format version
    9->12           fBEGIN      = Pointer to first data record
    13->16 [13->20] fEND        = Pointer to first free word at the EOF
    17->20 [21->28] fSeekFree   = Pointer to FREE data record
    21->24 [29->32] fNbytesFree = Number of bytes in FREE data record
    25->28 [33->36] nfree       = Number of free data records
    29->32 [37->40] fNbytesName = Number of bytes in TNamed at creation time
    33->33 [41->41] fUnits      = Number of bytes for file pointers
    34->37 [42->45] fCompress   = Compression level and algorithm
    38->41 [46->53] fSeekInfo   = Pointer to TStreamerInfo record
    42->45 [54->57] fNbytesInfo = Number of bytes in TStreamerInfo record
    46->63 [58->75] fUUID       = Universal Unique ID

===============================================================================

Dans Notes:

* The compression methods used: 
	- ZLIB is recommended to be used with compression level 1 [101]
	- LZMA is recommended to be used with compression level 7-8 (higher is better, 
  	  since in the case of LZMA we don't care about compression/decompression speed) [207 - 208]
	- LZ4 is recommended to be used with compression level 4 [404]
	- ZSTD is recommended to be used with compression level 5 [505]

  This means that the automatic setting [101] compresses the data using the ZLIB compression algorithm. 
  To decompress this we need to use the functions within "zlib.h" as detailed on their GitHub: https://github.com/madler/zlib 

* InflateData coopted from: https://gist.github.com/arq5x/5315739

* TDirectories are gonna be a massive pain...	

* Compile with:  gcc Root2Txt_Improved.C -lz -o R2T

* Run with: ./R2T RootFile.root

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#include "FileHeader.hpp"
#include "ROOTHeader.hpp"
#include "KeyHeader.hpp"
#include "Histo.hpp"

#define CHUNK 16384

unsigned char* InflateData(unsigned char ** Input, int LenInput, int LenOutput){ //THIS DOESN'T WORK - I have no idea why, have to investigate further...

	unsigned char Output[LenOutput];

	printf("The input  is %lu bytes long. Given as %i .\n",sizeof(*Input), LenInput);
	printf("The output is %lu bytes long. Given as %i .\n",sizeof(Output), LenOutput);
    printf("Input is: %.15s\n", *Input);

    // STEP 2.
    // inflate b into c
    // zlib struct
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in = (uInt)LenInput; // size of input
    infstream.next_in = (Bytef *)*Input; // input char array
    infstream.avail_out = (uInt)LenOutput; // size of output
    infstream.next_out = (Bytef *)Output; // output char array
     
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);
     
    printf("Uncompressed size is: %lu\n", sizeof(Output));
    printf("Uncompressed string is: %.15s\n", Output);
    printf("Full Output is:\n");
	for(int ik = 0; ik < 30; ik++) printf("%c", Output[ik]);
    printf("\n");

//	return Output;

	return NULL;
}





#ifndef __CINT__ 

int main(int argc,char **argv){

	//============================================= Open File
	FILE *infile  = fopen(argv[1], "rb"); 

	//============================================= Read Data

	int BytesTotal = 0;
	int BytesRead = 0;
//	unsigned char buffer[4];
	ROOTHeader RHead;

	BytesRead = RHead.Read(infile);
	RHead.Print();

//	int rend;
//	rend = RHead.GetfEnd();
	
	printf("Bytes read = %u\n", BytesRead);

	if(BytesRead < RHead.GetfBegin()){
		char junk[1];
		int num = RHead.GetfBegin();
		fread(junk, sizeof(junk), (num - BytesRead) , infile);
		//printf("things = %u\n", RHead.GetfBegin());
		//printf("num    = %u\n", num);
		BytesRead = num;
	}

	FileHeader Header;

	BytesTotal = BytesTotal + BytesRead;
	unsigned int tempbytes = BytesRead;
	BytesRead = Header.Read(infile);
	Header.Print();

	printf("Bytes read = %i\n", BytesRead);
	//printf("Bytes read = %lu\n", (sizeof(ROOTHeader) + sizeof(KeyHeader)));
	
	if(strcmp((const char*)Header.ClassName,"TFile")==0){
		int num2 = Header.NBytes;
		char junk2[1];
		fread(junk2, sizeof(junk2), (num2 - BytesRead), infile); 
		BytesRead = Header.NBytes;
		//printf("num2 = %i\n", Header.NBytes);
	}
	
	//char junk2[1];
	//fread(junk2, sizeof(junk2),21 , infile); 
	BytesTotal = BytesTotal + BytesRead;

	// READ HISTOGRAMS ==========

	int count =0;	
	for(int hist = 0; hist < 2; hist++){
		Histo histTest;
		BytesTotal += histTest.Read(infile, 0);
		count++;
	printf("TOTAL BYTES READ = %i / %i\n", BytesTotal,RHead.GetfEnd());

		if(BytesTotal >= RHead.GetfEnd()){
			printf("Breaking %i > %i\n", BytesTotal, RHead.GetfEnd());
			break;
		}

	}
	
	printf("TOTAL BYTES READ = %i\n", BytesTotal);
	printf("TOTAL HISTS READ = %i\n", count);


//	BytesRead = 0;
//	KeyHeader Header3;
//	Header3.Read(infile);
//	Header3.Print();
//	BytesTotal = BytesTotal + BytesRead;

//	printf("Bytes read = %i\n", BytesRead);	
//	printf("Total bytes read = %i\n\n", BytesTotal);	

	fclose(infile);

	return 1;

}

#endif
