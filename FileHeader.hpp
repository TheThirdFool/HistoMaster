// FileHeader
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define CHUNK 16384

#ifndef FILEHEADER_H
#define FILEHEADER_H

class FileHeader {

	public: 

	FileHeader(){BytesRead = 0;}

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

	int HexToInt(unsigned char * Hex, int num){
		int fin = 0;
		for(int i = 1; i <= num; i++){
			fin += pow(16, (i-1)*2) * Hex[num - i]; 
		}
		return fin;
	}

//	27->27 [35->35] lname     = Number of bytes in the class name
//	28->.. [36->..] ClassName = Object Class Name
//	..->..          lname     = Number of bytes in the object name
//	..->..          Name      = lName bytes with the name of the object
//	..->..          lTitle    = Number of bytes in the object title
//	..->..          Title     = Title of the object
//	----->          DATA      = Data bytes associated to the object

	int Read(FILE * infile){
		unsigned char buffer[4] = "xx";
		unsigned char buffer2[2];
		buffer[0]  = '\0';
		buffer2[0] = '\0';

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
		ClassName[lClassname] = '\0';
	
		fread(buffer3,sizeof(buffer3),1,infile);
		lObjname = HexToInt(buffer3, 1); 
		BytesRead += 1;
		unsigned char ObjnameBuffer[lObjname];	
		fread(ObjnameBuffer,sizeof(ObjnameBuffer),1,infile);
		for(int j=0; j<lObjname; j++) ObjName[j] = ObjnameBuffer[j]; 
		BytesRead += lObjname;
		ObjName[lObjname] = '\0';

		fread(buffer2,sizeof(buffer2),1,infile);
		BytesRead += 2;
		lTitle = HexToInt(buffer2, 2); 
		unsigned char TitleBuffer[lTitle];
		fread(TitleBuffer,sizeof(TitleBuffer),1,infile);
		for(int j=0; j<lTitle; j++) Title[j] = TitleBuffer[j]; 
		BytesRead += lTitle;
	
		return BytesRead;

	}

	int GetBytesRead(){return BytesRead;}


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

	private:
	int BytesRead;

};


#endif
