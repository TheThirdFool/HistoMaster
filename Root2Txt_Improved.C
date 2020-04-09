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

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int BytesRead = 0;


int HexToInt(unsigned char * Hex, int num){

	int fin = 0;

	for(int i = 1; i <= num; i++){
	//	printf("thing = %i\n", Hex[num - i]);
	//	printf("thing = %x\n", Hex[num - i]);
		fin += pow(16, (i-1)*2) * Hex[num - i]; 
	//	printf("fin = %i\n\n", fin);
	}

	return fin;
}


class ROOTHeader {
//	char buffer;

	public:

	int GetfBegin() {return fBegin;}

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
		printf("fCompress   = %f\n", fCompress);
		printf("fSeekInfo   = %u\n", fSeekInfo);
		printf("fNbytesInfo = %u\n", fNbytesInfo);
		printf("fUUID       = %lu\n", fUUID);
		printf("\n");
	}

	void Read(FILE * infile){
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
		fCompress = (float) HexToInt(buffer, 4); 
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
	float fCompress;
	unsigned int fSeekInfo;
	unsigned int fNbytesInfo;
	long fUUID;

};


struct FileHeader {

	unsigned int NBytes;
	unsigned short Version;
	unsigned int ObjLen;
	unsigned int Datime;
	unsigned short KeyLen;
	unsigned short Cycle;
	unsigned int SeekKey;
	unsigned int SeekPdir;
	unsigned char lClassname;
	unsigned char ClassName[256];
	unsigned char lObjname;
	unsigned char ObjName[256];
	unsigned short lTitle;
	unsigned char Title[256];

	void Print(){
		printf("\n"); 
		printf("NBytes     = %u\n", NBytes);
		printf("Version    = %u\n", Version);
		printf("ObjLen     = %u\n", ObjLen);
		printf("Datime     = %u\n", Datime);
		printf("KeyLen     = %u\n", KeyLen);
		printf("Cycle      = %u\n", Cycle);
		printf("SeekKey    = %u\n", SeekKey);
		printf("SeekPdir   = %u\n", SeekPdir);
		printf("lClassname = %u\n", lClassname);
		printf("ClassName  = %s\n", ClassName);
		printf("lObjname   = %u\n", lObjname);
		printf("ObjName    = %s\n", ObjName);
		printf("lTitle     = %u\n", lTitle);
		printf("Title      = %s\n", Title);
		printf("\n");
	}

//	27->27 [35->35] lname     = Number of bytes in the class name
//	28->.. [36->..] ClassName = Object Class Name
//	..->..          lname     = Number of bytes in the object name
//	..->..          Name      = lName bytes with the name of the object
//	..->..          lTitle    = Number of bytes in the object title
//	..->..          Title     = Title of the object
//	----->          DATA      = Data bytes associated to the object

	void Read(FILE * infile){
		unsigned char buffer[4];
		unsigned char buffer2[2];
			
		fread(buffer,sizeof(buffer),1,infile);
		//NBytes = (int) buffer;
		NBytes = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer2,sizeof(buffer2),1,infile);
		Version = (unsigned short) HexToInt(buffer2, 2);
		BytesRead += 2;
		
		fread(buffer,sizeof(buffer),1,infile);
		ObjLen = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		Datime = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer2,sizeof(buffer2),1,infile);
		KeyLen = (unsigned short) HexToInt(buffer2, 2);
		BytesRead += 2;
		
		fread(buffer2,sizeof(buffer2),1,infile);
		Cycle = (unsigned short) HexToInt(buffer2, 2);
		BytesRead += 2;
		
		fread(buffer,sizeof(buffer),1,infile);
		SeekKey = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		SeekPdir = (float) HexToInt(buffer, 4); 
		BytesRead += 4;

		unsigned char buffer3[1];

		fread(buffer3,sizeof(buffer3),1,infile);
		lClassname = HexToInt(buffer3, 1); 
		BytesRead += 1;
		unsigned char nameBuffer[lClassname];	
		fread(nameBuffer,sizeof(nameBuffer),1,infile);
		for(int j=0; j<lClassname; j++) ClassName[j] = nameBuffer[j]; 
		BytesRead += lClassname;
	
		fread(buffer3,sizeof(buffer3),1,infile);
		lObjname = HexToInt(buffer3, 1); 
		BytesRead += 1;
		unsigned char ObjnameBuffer[lObjname];	
		fread(ObjnameBuffer,sizeof(ObjnameBuffer),1,infile);
		for(int j=0; j<lObjname; j++) ObjName[j] = ObjnameBuffer[j]; 
		BytesRead += lObjname;

		fread(buffer2,sizeof(buffer2),1,infile);
		BytesRead += 2;
		lTitle = HexToInt(buffer2, 2); 
		unsigned char TitleBuffer[lTitle];
		fread(TitleBuffer,sizeof(TitleBuffer),1,infile);
		for(int j=0; j<lTitle; j++) Title[j] = TitleBuffer[j]; 
		BytesRead += lTitle;

	}
};

struct KeyHeader {

	unsigned int NBytes;
	unsigned short Version;
	unsigned int ObjLen;
	unsigned int Datime;
	unsigned short KeyLen;
	unsigned short Cycle;
	unsigned int SeekKey;
	unsigned int SeekPdir;
	unsigned char lClassname;
	unsigned char ClassName[256];
	unsigned char lObjname;
	unsigned char ObjName[256];
	unsigned char lTitle;
	unsigned char Title[256];

	void Print(){
		printf("\n"); 
		printf("NBytes     = %u\n", NBytes);
		printf("Version    = %u\n", Version);
		printf("ObjLen     = %u\n", ObjLen);
		printf("Datime     = %u\n", Datime);
		printf("KeyLen     = %u\n", KeyLen);
		printf("Cycle      = %u\n", Cycle);
		printf("SeekKey    = %u\n", SeekKey);
		printf("SeekPdir   = %u\n", SeekPdir);
		printf("lClassname = %u\n", lClassname);
		printf("ClassName  = %s\n", ClassName);
		printf("lObjname   = %u\n", lObjname);
		printf("ObjName    = %s\n", ObjName);
		printf("lTitle     = %u\n", lTitle);
		printf("Title      = %s\n", Title);
		printf("\n");
	}

//	27->27 [35->35] lname     = Number of bytes in the class name
//	28->.. [36->..] ClassName = Object Class Name
//	..->..          lname     = Number of bytes in the object name
//	..->..          Name      = lName bytes with the name of the object
//	..->..          lTitle    = Number of bytes in the object title
//	..->..          Title     = Title of the object
//	----->          DATA      = Data bytes associated to the object

	void Read(FILE * infile){
		unsigned char buffer[4];
		unsigned char buffer2[2];
			
		fread(buffer,sizeof(buffer),1,infile);
		//NBytes = (int) buffer;
		NBytes = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer2,sizeof(buffer2),1,infile);
		Version = (unsigned short) HexToInt(buffer2, 2);
		BytesRead += 2;
		
		fread(buffer,sizeof(buffer),1,infile);
		ObjLen = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		Datime = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer2,sizeof(buffer2),1,infile);
		KeyLen = (unsigned short) HexToInt(buffer2, 2);
		BytesRead += 2;
		
		fread(buffer2,sizeof(buffer2),1,infile);
		Cycle = (unsigned short) HexToInt(buffer2, 2);
		BytesRead += 2;
		
		fread(buffer,sizeof(buffer),1,infile);
		SeekKey = HexToInt(buffer, 4);
		BytesRead += 4;
		
		fread(buffer,sizeof(buffer),1,infile);
		SeekPdir = (float) HexToInt(buffer, 4); 
		BytesRead += 4;

		unsigned char buffer3[1];

		fread(buffer3,sizeof(buffer3),1,infile);
		lClassname = HexToInt(buffer3, 1); 
		BytesRead += 1;
		unsigned char nameBuffer[lClassname];	
		fread(nameBuffer,sizeof(nameBuffer),1,infile);
		for(int j=0; j<lClassname; j++) ClassName[j] = nameBuffer[j]; 
		BytesRead += lClassname;
	
		fread(buffer3,sizeof(buffer3),1,infile);
		lObjname = HexToInt(buffer3, 1); 
		BytesRead += 1;
		unsigned char ObjnameBuffer[lObjname];	
		fread(ObjnameBuffer,sizeof(ObjnameBuffer),1,infile);
		for(int j=0; j<lObjname; j++) ObjName[j] = ObjnameBuffer[j]; 
		BytesRead += lObjname;

		fread(buffer3,sizeof(buffer3),1,infile);
		BytesRead += 1;
		lTitle = HexToInt(buffer3, 1); 
		unsigned char TitleBuffer[lTitle];
		fread(TitleBuffer,sizeof(TitleBuffer),1,infile);
		for(int j=0; j<lTitle; j++) Title[j] = TitleBuffer[j]; 
		BytesRead += lTitle;

	}
};

#ifndef __CINT__ 

int main(int argc,char **argv){

	//============================================= Open File
	FILE *infile  = fopen(argv[1], "rb"); 

	//============================================= Read Data

//	unsigned char buffer[4];
	ROOTHeader RHead;
	FileHeader Header;

	RHead.Read(infile);
	RHead.Print();

	printf("Bytes read = %u\n", BytesRead);

	if(BytesRead < RHead.GetfBegin()){
		char junk[1];
		int num = RHead.GetfBegin();
		fread(junk, sizeof(junk), (num - BytesRead) , infile);
		//printf("things = %u\n", RHead.GetfBegin());
		//printf("num    = %u\n", num);
		BytesRead = num;
	}


	unsigned int tempbytes = BytesRead;
	BytesRead = 0;
	Header.Read(infile);
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

	BytesRead = 0;
	KeyHeader Header2;
	Header2.Read(infile);
	Header2.Print();

	printf("Bytes read = %i\n", BytesRead);	
/*
	BytesRead = 0;
	KeyHeader Header3;
	Header3.Read(infile);
	Header3.Print();

	if(strcmp((const char*)Header2.ClassName,"TH1D")==0){
		int num3 = Header2.NBytes;
		double Data[num3 / 8];
		unsigned char DataBuffer[8];

		for(int k=0; k < (num3 / 8); k++){
			fread(DataBuffer, sizeof(DataBuffer), 1, infile); 
		//	Data[k] = (double) HexToInt(DataBuffer, 8);
			printf("%s\n", DataBuffer);
		}
		BytesRead = Header2.NBytes;

		for(int count=0; count < (num3 / 8); count++){
			printf("%f\n", Data[count]);
		}

	}
*/

	printf("Bytes read = %i\n", BytesRead);	

	fclose(infile);

    return 0;
}

#endif
