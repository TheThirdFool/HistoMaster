/// File base stuff
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define CHUNK 16384

#ifndef ROOTHEADER_H
#define ROOTHEADER_H

class ROOTHeader {
//	char buffer;

	public:

	ROOTHeader(){BytesRead = 0;}
	~ROOTHeader(){}


	int GetfBegin() {return fBegin;}
	int GetfEnd() {
		printf("fEnd =====================> %i\n",fEnd);
		return fEnd;
	}
	int GetBytesRead(){return BytesRead;}

	void Print(){
		printf("\n");
		printf("RootDeclare = %.4s\n", DeclareROOT);
		printf("fVersion    = %u\n", fVersion);
		printf("fBegin      = %u\n", fBegin);
		printf("fEnd        = %u\n", fEnd);
		printf("fSeekFree   = %u\n", fSeekFree);
		printf("fNbytesFree = %u\n", fNbytesFree);
		printf("nfree       = %u\n", nfree);
		printf("fNbytesName = %u\n", fNbytesName);
		printf("fUnits      = %u\n", fUnits);
		printf("fCompress   = %i\n", fCompress);
		printf("fSeekInfo   = %u\n", fSeekInfo);
		printf("fNbytesInfo = %u\n", fNbytesInfo);
		printf("fUUID       = %lu\n", fUUID);
		printf("\n");
	}

	int HexToInt(unsigned char * Hex, int num){
		int fin = 0;
		for(int i = 1; i <= num; i++){
			fin += pow(16, (i-1)*2) * Hex[num - i]; 
		}
		return fin;
	}

	int Read(FILE * infile){
		unsigned char buffer[4];
		
		fread(DeclareROOT,sizeof(buffer),1,infile);		
		BytesRead += 4;

		fread(buffer,sizeof(buffer),1,infile);
		fVersion = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fBegin = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fEnd = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fSeekFree = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fNbytesFree = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		nfree = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fNbytesName = HexToInt(buffer, 4);
		BytesRead += 4;
		
		unsigned char buffer2[1];
		
		fread(&buffer2,sizeof(buffer2),1,infile);
		fUnits = HexToInt(buffer2, 1);
		BytesRead += 1;
		
		fread(buffer,sizeof(buffer),1,infile);
		fCompress = HexToInt(buffer, 4); 
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fSeekInfo = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		fNbytesInfo = HexToInt(buffer, 4);
		BytesRead += 4;
		
		unsigned char buffer3[18];
		
		fread(buffer3,sizeof(buffer3),1,infile);
		fUUID = (long) HexToInt(buffer3, 18);
		BytesRead += 18;

		return BytesRead;	

	}

	private:

	char DeclareROOT[4];
	unsigned int fVersion;
	unsigned int fBegin;
	//char fBegin[4];
	unsigned int fEnd;
	unsigned int fSeekFree;
	unsigned int fNbytesFree;
	unsigned int nfree;
	unsigned int fNbytesName;
	char fUnits;
	int fCompress;
	unsigned int fSeekInfo;
	unsigned int fNbytesInfo;
	long fUUID;
	int BytesRead;

};


#endif

