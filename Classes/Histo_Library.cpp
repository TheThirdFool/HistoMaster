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
// int Histo::ReadCNF(FILE * infile)        -  Read data from a CNF file
// int ReadString(char* String, int row, double * dat) - Split string into doubles 
// int Histo::HexToInt(unsigned char * Hex, int num)   - Read hex number - return int
// double Histo::CNF_Energy(int x, double * A)         - Return calibrated energy CNF
// double Integrate()                        - Return whole hist integral
// double Integrate(double low, double high) - Return integral within bounds

// int Histo::Fit(char * func, int noIterations = 5)
// int Histo::GS_Process(HMatrix * old_matrix)
// int Histo::Proj(double * A, double * B, int NoRows, double * ret)
// int Histo::ReadFlex(char* filename)

// int Histo::Draw()
// int Histo::Draw(double low, double high)
// int Histo::Draw(double low = -10101.01, double high = -10101.01, int opt = 0)









// == INCLUDES ==
#include "Histo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zlib.h"
//#include "lz4.h"
#include <vector>
#include <string.h>


#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>

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
	
//	printf("Bytes read = %i\n", BytesRead);	
//	printf("Total bytes read = %i\n\n", BytesTotal);	

	//================================================================================================
	//========================================================================= THIS AREA IS BROKEN ==
	//================================================================================================
		
	//	printf("H2.NBytes = %i\nH2.KeyLen = %i\n", Header2.NBytes, Header2.KeyLen);

	unsigned char junk[9];
	fread(&junk, sizeof(unsigned char), 9, infile);
		
		
	unsigned char Input[Header2.NBytes - Header2.KeyLen -9];
	//unsigned char JunkBuffer[16];
	
	//int check1 = fread(JunkBuffer, sizeof(JunkBuffer), 1, infile);
	//int check = fread(Input, sizeof(Input), 1, infile);
	int check = fread(&Input, sizeof(unsigned char), (Header2.NBytes - Header2.KeyLen - 9), infile);
	BytesRead = BytesRead + (Header2.NBytes - Header2.KeyLen);	
		
	if(strcmp((char*)Header2.ClassName, "TH1D") == 0){
		
	//	printf("Size of Input outside inflate = %lu\n", sizeof(Input));
	//	printf("Size of check = %i\n", check);
	//	printf("Input is:\n");
	//	for(int ik = 0; ik < 50; ik++) printf("%c", Input[ik]);
	//	printf("\n");
		
		//	InflateData(&Input, (Header2.NBytes - Header2.KeyLen), Header2.ObjLen);
		
		int LenInput  = Header2.NBytes - Header2.KeyLen;
		int LenOutput = Header2.ObjLen;// - Header2.KeyLen;
		
		unsigned char Output[LenOutput];
		Output[0] = '\0';
		
	//	printf("The input  is %lu bytes long. Given as %i .\n",sizeof(Input), LenInput);
	//	printf("The output is %lu bytes long. Given as %i .\n",sizeof(Output), LenOutput);
	//	printf("Input is:\n");
	//	for(int ik = 0; ik < 16; ik++){
	//		printf("%.2x ", Input[ik]);
			//buff[ik - 60] = Output[ik];
	//	}
	//	printf("\n");

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
	//	printf("ZLIB Output ->->-> %s\n", infstream.msg);
		
		// https://github.com/klauspost/compress 
		// My saviour??? We will see
		
	//	printf("ret = %i\n", ret);
		
		ret = inflate(&infstream, 4);
	//	printf("ret = %i\n", ret);
	//	printf("ZLIB Output ->->-> %s\n", infstream.msg);
		ret = inflateEnd(&infstream);
	//	printf("ret = %i\n", ret);
		 
	//	printf("ZLIB Output ->->-> %s\n", infstream.msg);

	//	LZ4_decompress_safe((const char*)Input, (char*)Output, LenInput, LenOutput);	
		
	//	printf("Uncompressed size is: %lu\n", sizeof(Output));
	//	printf("Uncompressed string is: %.15s\n", Output);
	//	printf("Full Output is:\n");

//		int okok = 0;
//		for(int ik = 0; ik < sizeof(Output); ik+=8){
//			printf("%.4i:%.4i: ",okok++, ik);
//			for(int ll=0; ll < 8; ll++){
//				printf("%.2x ", Output[ik + ll]);
//			}

//			printf("   |   ");
//			for(int ll=0; ll < 8; ll++){
//				if(Output[ik + ll] == '\n'){
//					printf(".");
//					continue;
//				}
//				printf("%c", Output[ik + ll]);
//			}
//			printf(" | \n");

//		}

		int ByteNumber = HexToInt(Output+8, 2);
		int NDoubles   = HexToInt(Output+ByteNumber+12, 2);

	//	printf("Byte[%i] claims %i doubles\n", ByteNumber, NDoubles); 

		//Bytes [8-9] = Byte number of number of doubles in array
		//Bytes [2-3] = Number of bytes in object (-4?)

		int DataStart = sizeof(Output) - (NDoubles * 8);
		int binNo = 0;

		//buff[0] = '\0';
		for(int ik = DataStart; ik < sizeof(Output); ik+=8){
		//	printf("%i ", HexToInt(Output + ik,8));//, Output[ik+1], Output[ik+2], Output[ik+3], Output[ik+4], Output[ik+5], Output[ik+6], Output[ik+7]);
			char buf[64];
			sprintf(buf, "0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x",Output[ik], Output[ik+1], Output[ik+2], Output[ik+3], Output[ik+4], Output[ik+5], Output[ik+6], Output[ik+7]);
			//printf("[%s] ", buf);

			double d = 0.0;	
			try{
			    *reinterpret_cast<unsigned long long*>(&d) = std::stoull(buf, nullptr, 16);
			}
			catch(...){}

//			printf(" %f \n", d);

			X.push_back(binNo++);
			Y.push_back(d);	

		}

//		double buff = *(double*)(Output + 592 + 8);
//		printf("\nBuff = %f\n", buff);

//		printf("\n");

	}
	
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





/*
double  Histo::Convert2Double(unsigned int a, unsigned int b){
	uint16_t  tmp16[2];
	tmp16[0] = b;
	tmp16[1] = a;

	return (*(float*)tmp16)/4.;
}
*/


// DFH - Return the calibrated energy from the CNF calibration
double Histo::CNF_Energy(int x, float * A){
	return A[0] + A[1]*x + A[2]*x*x + A[3]*x*x*x;
}


// AZ - Reads a CNF file 
int Histo::ReadCNF(FILE * infile){

	// Declare variables
    std::vector<int> data;
    int temp;
    size_t read;

    // Reads data from file in ints and adds to data(vector)
    while((read = fread(&temp,sizeof(int),1,infile))==1) {
        data.push_back(temp);
    }



	// DFH - Calibrate
	// ==============================================================================

	unsigned int param_offs;

	// Read the header to find the location of the calibration data
	for (int i=0;i < 5;i++){
		int oh = 28 + i * 12;

		if(data[oh] != 73728) continue;

		fseek(infile, ((oh * 4) + 10), SEEK_SET);
		fread(&param_offs,sizeof(uint32_t),1,infile);
		//printf("Parameter offset   = %u\n", param_offs);
		break;

	}

	// More finding the calibration parameters... They make it deliberately difficult
	fseek(infile, (param_offs + 34), SEEK_SET);
	int random_offset;
	fread(&random_offset,sizeof(uint16_t),1,infile);

	int cal_offs;
	cal_offs = param_offs + 48 + random_offset;
	//printf("Calibration offset = %u\n", cal_offs);

	// Go to the calibration location
	float Cal_Vals[4];
	fseek(infile, (cal_offs + 68), SEEK_SET);
	
	// Read cal vals 
	unsigned int bytes[2];
	uint16_t  tmp16[2];
	for(int i=0; i < 4; i++){ 
		fread(bytes, sizeof(uint16_t), 2, infile);
		// Convert Cal vals into floats (little endian shit)
		tmp16[0] = bytes[1];
		tmp16[1] = bytes[0];		
		Cal_Vals[i] = (*(float*)tmp16)/4.;
	}

	printf("Calibration = %f + %f * x + %f * x^2 + %f * x^3\n", Cal_Vals[0], Cal_Vals[1], Cal_Vals[2], Cal_Vals[3]);

	// ==============================================================================

    // Makes i-offset bins and sets to data[i] values
    int offset = data.size() - 4096;
    for(int i=offset +1;i < data.size();i++) {
        X.push_back(CNF_Energy(i-offset, Cal_Vals));
		Y.push_back(data[i]);
    }

    return 1;
}




// DFH - Flex read
int Histo::ReadFlex(char* filename){

	//============================================= Open File

	FILE *infile  = fopen(filename, "r"); 

	//============================================= Read Data
	//if infile name is .txt
	ReadTXT(infile); 

	//============================================= Analyse Data

	fclose(infile);

	return 1;

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


int Histo::Proj(double * A, double * B, int NoRows, double * ret){

	double A_total = 0;
	for(int i = 0; i < NoRows; i++){
		A_total =+ A[i] * A[i];
	}

	double B_total = 0;
	for(int i = 0; i < NoRows; i++){
		B_total =+ B[i] * B[i];
	}

	
	double scaler = sqrt(B_total) / sqrt(A_total);
	
	for(int i = 0; i < NoRows; i++){
		ret[i] = scaler * A[i];
	}
	
	return 1;

} 


int Histo::GS_Process(HMatrix * old_matrix){

	old_matrix->Print();

//  === SUDO CODE ===
	HMatrix new_matrix;

	int noRows = old_matrix->GetNoRows();

	double * old_column;
	old_matrix->GetColumn(old_column, 0);
	
	double new_column[noRows];
	memcpy(new_column, old_column, sizeof(double)*noRows);
	new_matrix.AddColumn(new_column, 0);


	// THIS WILL NEED SOME FIXING HOWEVER THE LOGIC IS SOUND

	int count = 0;
	for(int i=1; i < old_matrix->GetNoColumns(); i++) {
		old_matrix->GetColumn(old_column, i);
	
		*new_column = *old_column;
	
		double total[noRows];
		*total  = *old_column;
		for(int j = i - 1; j >= 0; j--){
			double ret[noRows];
			double ret_col[noRows];
			new_matrix.GetColumn(ret_col, j);
			Proj(ret_col, old_column, noRows, ret);
			for(int k=0; k < noRows; k++) total[k] -= ret[k];
		}
		
//		new_column.normalise();
	
		new_matrix.AddColumn(new_column, count);
		count++;

	}

	new_matrix.Print();

//	R = new_matrix.transpose() [MATRIX MULT] Old_Matrix 


	return 1;
}



// LETS FIT THIS BITCH
// DFH - Fitting function for the gaussian atm.
int Histo::Fit(char * func, int noIterations){

	if(strcmp(func,"gaus") !=0) return 0;


	// AUTOMATE THIS
	double a_j = 0.5;  // A - Scaling  - maybe put into a pointer "beta"
	double b_j = 2.0;  // B - Mean
	double c_j = 0.25; // C - Std Dev
	double d_j = 0.0;  // D - Background

	// JACOBIAN
	double J[X.size()][4];
	// Turn into HMatrix

	// ITERATE AND IMPROVE
	for(int i=0; i < noIterations; i++){


		// Fill Jacobian with data
		for(int j=0; j < X.size(); j++){
			double x_i = X[i];
			double y_i = Y[i];

			//exp()	
	
			double temp = (x_i - b_j) * (x_i - b_j);
			double temp2 = (x_i - b_j) / (c_j * c_j);

			J[j][0] = exp((-1.0 * temp) / (2.0 * c_j * c_j));
			J[j][1] = a_j * exp((-1.0 * temp) / (2.0 * c_j * c_j)) * temp2;
			J[j][2] = a_j * exp((-1.0 * temp) / (2.0 * c_j * c_j)) * temp2 * (x_i - b_j) / c_j;
			J[j][3] = 1.0;
			
		}


		// GRAM-SCHMIDT PROCESS
		HMatrix J_Mat(X.size(), 4);
		J_Mat.Print();
		J_Mat.setData((double **)J);


		int J_MP = GS_Process(&J_Mat);

		// AMMEND FIT

		// Back propogation etc here!

		// for(int k=0; k < 4; k++){

			// Find beta[i] + (J_MP * r(beta))[i]

		// }

	}



	return 1;
}


// ================== DRAW X & Y (with gnuplot)

int Histo::Draw(){

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts;
	for(int i=0; i < X.size(); i++) {
		//printf("%f, %f\n", X[i], Y[i]);
		xy_pts.push_back(std::make_pair(X[i], Y[i]));
	}


	// Find max y:
	double y_max = 0.0;
	for(int i=0; i < Y.size(); i++){
		if(Y[i] > y_max) y_max = Y[i];
	}

	y_max = y_max + 0.05 * y_max; 

	char char_buffer [50];
	sprintf(char_buffer, "set xrange [-2:%f]\nset yrange [-2:%f]\n",X[X.size()-1],y_max);

	gp << char_buffer; 
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "set style fill solid\n";
	gp << "plot '-' with boxes title 'X'\n";
	gp.send1d(xy_pts);
	//gp.send1d(Y);
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();

	return 1;

}

int Histo::Draw(double low = -10101.01, double high = -10101.01){

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts;
	for(int i=0; i < X.size(); i++) {
		//printf("%f, %f\n", X[i], Y[i]);
		xy_pts.push_back(std::make_pair(X[i], Y[i]));
	}

	if(high == -10101.01){
		high = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] > high) high = X[i];
		}
	}

	if(low == -10101.01){
		low = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] < low) low = X[i];
		}
	}


	// Find max y:
	double y_max = 0.0;
	for(int i=0; i < Y.size(); i++){
		if(X[i] < low)  continue;
		if(X[i] > high) continue;
		if(Y[i] > y_max) y_max = Y[i];
	}

	y_max = y_max + 0.05 * y_max; 


	char char_buffer [128];
	sprintf(char_buffer, "set xrange [%f:%f]\nset yrange [-2:%f]\n", low, high, y_max);
	printf("YMAX = %f\n", y_max);

	gp << char_buffer; 
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "set style fill solid\n";
	gp << "plot '-' with boxes title 'X'\n";

	gp.send1d(xy_pts);
	//gp.send1d(Y);
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();

	return 1;

}


int Histo::Draw(double low = -10101.01, double high = -10101.01, int opt = 0){

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts;
	for(int i=0; i < X.size(); i++) {
		//printf("%f, %f\n", X[i], Y[i]);
		xy_pts.push_back(std::make_pair(X[i], Y[i]));
	}

	if(high == -10101.01){
		high = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] > high) high = X[i];
		}
	}

	if(low == -10101.01){
		low = X[0];
		for(int i=1; i < X.size(); i++){
			if(X[i] < low) low = X[i];
		}
	}


	// Find max y:
	double y_max = 0.0;
	for(int i=0; i < Y.size(); i++){
		if(X[i] < low)  continue;
		if(X[i] > high) continue;
		if(Y[i] > y_max) y_max = Y[i];
	}

	y_max = y_max + 0.05 * y_max; 


	char char_buffer [128];
	sprintf(char_buffer, "set xrange [%f:%f]\nset yrange [-2:%f]\n", low, high, y_max);
	printf("YMAX = %f\n", y_max);

	gp << char_buffer; 
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	if(opt == 1){
		gp << "set style fill solid\n";
		gp << "replot '-' with boxes title 'X'\n";
	} else {
		gp << "set style fill solid\n";
		gp << "plot '-' with boxes title 'X'\n";
	}

	gp.send1d(xy_pts);
	//gp.send1d(Y);
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();

	return 1;

}









// ADDITIONAL CODE


/*


double GetZ(double l, double low, double high){

	double a = exp(-low / l);
	double b = exp(-high / l);

	return a - b;

}

double GetP(double l){
	return 1.0;
}


TGraph * MakeProbGraph(std::vector<double> data, double low, double high, double low_l, double high_l, int max){

	int steps = 10000;
	double dl = ( high_l - low_l ) / steps;
	double sum = 0.0;
	double N;

	int infs = 0;
	int nans = 0;
	
	if(max == -1){
		N = data.size();
	} else { 
		N = max;
	}

	for(int i = 0; i < N; i++){
		if(data[i] > high) continue;
		if(data[i] < low)  continue;
		sum += data[i];
	//	printf("Da = %f\n", data[i]);
	}


	TGraph * gra = new TGraph(steps);

	for(int hold = 0; hold < steps; hold++){

		double l = hold * dl + low_l; 
		double A = exp( - sum / l) * GetP(l);
		double Z = GetZ(l, low, high);

		double prob = A * 1.0 / pow((l * Z),N);

		if(prob!= prob){ 
//			printf("l = %f\nA = %f\nZ = %f\n", l, A, Z);
//			printf("Prob = %f\n", prob);
			nans++;
			continue;
		}

		if(isinf(prob)){
//			printf("l = %f\nA = %f\nZ = %f\n", l, A, Z);
//			printf("Prob = %f\n", prob);
			infs++;
			continue;
		}

//		if(prob!= 0){ 
//			printf("l = %f\nA = %f\nZ = %f\n", l, A, Z);
//			printf("Prob = %f\n", prob);
//		}


		gra->SetPoint(hold,l, prob);

	}

//	printf("sum = %f\nN = %f\n", sum, N);
	printf("Infs = %i\nNaNs = %i\n", infs, nans);

	return gra;

}

*/







