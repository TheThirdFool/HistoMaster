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
// int Histo::MakeProbGraph(double low, double high, double low_l, double high_l)
// double Histo::GetP(double l)
// double Histo::GetZ(double l, double low, double high)
// int Histo::SetPoint(double x_point, double y_point)









// == INCLUDES ==
#include "Histo.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zlib.h"
//#include "lz4.h"
#include <vector>
#include <string.h>
#include "HistoGUI_Library.cpp"


#include "gnuplot-iostream.h"
#include <boost/tuple/tuple.hpp>

// == CODE ==



// DFH - Constructor clearing the histo class
Histo::Histo(){
	X.clear();
	Y.clear();
	
	lName  = '\0';
	lTitle = '\0';
	Name[0]  = '\0';
	Title[0] = '\0';
	memcpy(Type,"????",4);

	x_axis_max = -10101;
	x_axis_min = 10101;

}


// DFH - Read in a hexidecimal number and return an integer
int Histo::HexToInt(unsigned char * Hex, int num){
	int fin = 0;
	for(int i = 1; i <= num; i++){
	//	printf("%.2x ", Hex[num-i]);
		fin += pow(16, (i-1)*2) * Hex[num - i]; 
	}
	//printf("\n");
	return fin;
}


int Histo::Read(FILE *infile, int BytesTotal){ 

	int BytesRead = 0;
	KeyHeader Header2;
	BytesRead = Header2.Read(infile);
//	Header2.Print();

	Name[0]  = '\0';
	Title[0] = '\0';

	lName = Header2.lObjname; 
	memcpy(Name, Header2.ObjName, lName);
	Name[lName] = '\0';
	
	lTitle = Header2.lTitle;
	memcpy(Title, Header2.Title, lTitle);
	Title[lTitle] = '\0';
	
	unsigned char junk[9];
	fread(&junk, sizeof(unsigned char), 9, infile);
				
	unsigned char Input[Header2.NBytes - Header2.KeyLen -9];

	int check = fread(&Input, sizeof(unsigned char), (Header2.NBytes - Header2.KeyLen - 9), infile);
	BytesRead = BytesRead + (Header2.NBytes - Header2.KeyLen);	
		
	memcpy(Type,(char*)Header2.ClassName,4);
	//printf("%s\n", Name);
		
	if(strcmp((char*)Header2.ClassName, "TH1D") == 0){
		
	//	memcpy(Type,"TH1D",4);
		int LenInput  = Header2.NBytes - Header2.KeyLen;
		int LenOutput = Header2.ObjLen;// - Header2.KeyLen;
		
		unsigned char Output[LenOutput];
		Output[0] = '\0';

		z_stream infstream;
		infstream.zalloc = 0;
		infstream.zfree  = 0;
		infstream.opaque = 0;
		
		
		infstream.avail_in = (uInt)LenInput; // size of input
		infstream.next_in = Input; // input char array
		infstream.avail_out = (uInt)LenOutput; // size of output
		infstream.next_out = Output; // output char array
		
		
		int wbits = 18;
		
		int ret;
		
		// the actual DE-compression work.
		ret = inflateInit(&infstream);//, wbits);	
		ret = inflate(&infstream, 4);
		ret = inflateEnd(&infstream);

//		for(int ii = 0; ii < LenOutput; ii+=8){
//			printf("%i | %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x\n", ii, Output[ii], Output[ii+1], Output[ii+2], Output[ii+3], Output[ii+4],Output[ii+5],Output[ii+6],Output[ii+7]);
//		}

		int ByteNumber = HexToInt(Output+8, 2);
		int NDoubles   = HexToInt(Output+ByteNumber+12, 2);

		//Bytes [8-9] = Byte number of number of doubles in array
		//Bytes [2-3] = Number of bytes in object (-4?)

		int DataStart = sizeof(Output) - (NDoubles * 8);
		int binNo = 0;

	//	printf("ByteNumber = %i\n", ByteNumber);
	//	printf("nDoubles   = %i\n", NDoubles);
	//	printf("Data start = %i\n", DataStart);

		//buff[0] = '\0';
		for(int ik = DataStart; ik < sizeof(Output); ik+=8){
			char buf[64];
			sprintf(buf, "0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x",Output[ik], Output[ik+1], Output[ik+2], Output[ik+3], Output[ik+4], Output[ik+5], Output[ik+6], Output[ik+7]);

			double d = 0.0;	
			try{
			    *reinterpret_cast<unsigned long long*>(&d) = std::stoull(buf, nullptr, 16);
			}
			catch(...){}

			X.push_back(binNo++);
			Y.push_back(d);	
		}

	} else if(strcmp((char*)Header2.ClassName, "TH2D") == 0){
	//	memcpy(Type,"TH1D",4);

		int LenInput  = Header2.NBytes - Header2.KeyLen;
		int LenOutput = Header2.ObjLen;// - Header2.KeyLen;
	
		
		unsigned char Output[LenOutput];
		Output[0] = '\0';

		z_stream infstream;
		infstream.zalloc = 0;
		infstream.zfree  = 0;
		infstream.opaque = 0;
		
		
		infstream.avail_in = (uInt)LenInput; // size of input
		infstream.next_in = Input; // input char array
		infstream.avail_out = (uInt)LenOutput; // size of output
		infstream.next_out = Output; // output char array
		
		
		int wbits = 18;
		
		int ret;
		
		// the actual DE-compression work.
		ret = inflateInit(&infstream);//, wbits);	
		ret = inflate(&infstream, 4);
		ret = inflateEnd(&infstream);

		//for(int ii = 0; ii < 10+4+69+4+69+100; ii+=8){
		//	printf("%i | %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x\n", ii, Output[ii], Output[ii+1], Output[ii+2], Output[ii+3], Output[ii+4],Output[ii+5],Output[ii+6],Output[ii+7]);
		//}
		int ByteNumber = HexToInt(Output+8, 2);

		//Read first block
		//Read position 20-21  = [off1]
		int offset_sum = 20;
		int off1 = HexToInt(Output + offset_sum, 2);

		//Read 20+off1 = off2
		offset_sum += off1 + 4;
		off1 = HexToInt(Output + offset_sum, 2);

		//Read 20 + off1 + off2  = off3
		offset_sum += off1 + 4;
		off1 = HexToInt(Output + offset_sum, 2);

		//Read 20 + off1 + pff2 + off3 = off4 (add+4)
		offset_sum += off1 + 4;
		off1 = HexToInt(Output + offset_sum, 2);
		offset_sum += off1 + 4 + 4;
		
		int lengthOfAxisChunk = HexToInt(Output + offset_sum, 2);
		int startOfAxisChunk  = offset_sum + 2;

		offset_sum += 6;
		off1 = HexToInt(Output + offset_sum, 2);

		offset_sum += off1 + 4 + 38 + 2;
		int XBins = HexToInt(Output + offset_sum, 2);
		

		offset_sum = lengthOfAxisChunk + startOfAxisChunk + 8;
		off1 = HexToInt(Output + offset_sum, 2);

		offset_sum += off1 + 4 + 38 + 2;
		int YBins = HexToInt(Output + offset_sum, 2);
		
		int NDoubles   = XBins * YBins;

		//Bytes [8-9] = Byte number of number of doubles in array
		//Bytes [2-3] = Number of bytes in object (-4?)

		int DataStart = sizeof(Output) - (NDoubles * 8);
		int binNoX = 0;
		int binNoY = 0;
		for(int x_i = 0; x_i < XBins; x_i++){
			std::vector<double> temp_vect;
			temp_vect.resize(YBins);
			Z.push_back(temp_vect);
		}
		X.resize(XBins);
		Y.resize(YBins);
		
	//	printf("%s:\n", Name);
	//	printf("ByteNumber = %i\n", ByteNumber);
	//	printf("Data start = %i\n", DataStart);
	//	printf("XBins = %i\n", XBins);
	//	printf("YBins = %i\n", YBins);
		
		//buff[0] = '\0';
		int counter = 0;
		
		for(int ik = DataStart; ik < sizeof(Output); ik+=8){
			char buf[64];
			sprintf(buf, "0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x",Output[ik], Output[ik+1], Output[ik+2], Output[ik+3], Output[ik+4], Output[ik+5], Output[ik+6], Output[ik+7]);
	//		printf("0x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x",Output[ik], Output[ik+1], Output[ik+2], Output[ik+3], Output[ik+4], Output[ik+5], Output[ik+6], Output[ik+7]);

			double d = 0.0;	
			try{
			    *reinterpret_cast<unsigned long long*>(&d) = std::stoull(buf, nullptr, 16);
			}
			catch(...){}

	//		printf("double = %f\n", d);

			binNoX = div(counter, XBins).rem;
			binNoY = div(counter, XBins).quot;
	//		printf("bin [%i] [%i]\n", binNoX, binNoY);
			counter++;

			X[binNoX] = binNoX;
			Y[binNoY] = binNoY;	
	//		printf("some things are set!\n");
			Z[binNoX][binNoY] = d;
	//		printf("All things are set!\n");
		}

	}
		
	BytesTotal = BytesTotal + BytesRead;
	//printf("Total bytes read = %i\n\n", BytesRead);	
	printf("Name [%s] read\n", Name);

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

	fseek(infile, (cal_offs + 0x5c), SEEK_SET);
	fread(energy_unit, sizeof(char), 3, infile);

	printf("Calibration = %f + %f * x + %f * x^2 + %f * x^3 [%s]\n", Cal_Vals[0], Cal_Vals[1], Cal_Vals[2], Cal_Vals[3], energy_unit);

	// Find the number of bins
	char n_channels_read;
	fseek(infile, (param_offs + 0x00ba), SEEK_SET);
	fread(&n_channels_read, sizeof(char), 8, infile);
	int n_channels = (int) n_channels_read * 256;
	printf("nbins = %i\n", n_channels);

	

	// ==============================================================================
    // Makes i-offset bins and sets to data[i] values
    int offset = data.size() - n_channels;
    for(int i=offset +1;i < data.size();i++) {
        X.push_back(CNF_Energy((i-offset), Cal_Vals));
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

double Histo::GetMaxX(){
	if(x_axis_max != -10101) {
		return x_axis_max;
	} else {

		x_axis_max = X[0];
		for(int i = 1; i < X.size(); i++){
			if(X[i] > x_axis_max) x_axis_max = X[i];
		}

		return x_axis_max;
	}
}

double Histo::GetMinX(){
	if(x_axis_min != 10101) {
		return x_axis_min;
	} else {

		x_axis_min = X[0];
		for(int i = 1; i < X.size(); i++){
			if(X[i] < x_axis_min) x_axis_min = X[i];
		}

		return x_axis_min;
	}
}


// ================== DRAW X & Y (with gnuplot)

int Histo::Draw(){

/*
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

*/

	HistoGUI gui;

	//std::vector<double> x;
	//std::vector<double> y;

	//for(int i = 0; i < 25; i++){
	//	x.push_back( i );
	//	y.push_back( pow(i, 0.5) );
	//}

	if(strcmp(Type, "TH2D") == 0){
		gui.Set2D(true);
		gui.SetData2D(X,Y,Z);
	} else {
		gui.Set2D(false);
		gui.SetData(X,Y);
	}

	gui.Init();
	gui.Loop();
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
	gui.Close();


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

int Histo::SetPoint(double x_point, double y_point){

	for(int i = 0; i < X.size(); i++){
		if(X[i] == x_point){
			Y[i] = y_point;
			return 1;
		}
	}
	
	X.push_back(x_point);
	Y.push_back(y_point);
	
	return 1;
}






// ADDITIONAL CODE


double Histo::GetZ(double l, double low, double high){

	double a = exp(-low / l);
	double b = exp(-high / l);

	return a - b;

}

double Histo::GetP(double l){
	return 1.0;
}


int Histo::MakeProbGraph(double low, double high, double low_l, double high_l, double scale){

	int steps = 10000;
	double dl = ( high_l - low_l ) / steps;
	double sum = 0.0;
	double N = X.size();
	double newN = 0.0;

	int infs = 0;
	int nans = 0;
	

	for(int i = 0; i < N; i++){
		if(X[i] * scale > high) continue;
		if(X[i] * scale < low)  continue;
		sum += Y[i] * X[i] * scale;
		newN += Y[i];
	}


	HGraph probGraph;

	for(int hold = 0; hold < steps; hold++){

		double l = hold * dl + low_l; 
		double A = exp( - sum / l) * GetP(l);
		double Z = GetZ(l, low, high);

		double prob = A * 1.0 / pow((l * Z),newN);

		if(prob!= prob){ 
			nans++;
			continue;
		}

		if(isinf(prob)){
			infs++;
			continue;
		}
		//printf("%f\n", prob * 10E60);
		probGraph.SetPoint(l, prob * 10E34);

	}

//	printf("sum = %f\nN = %f\n", sum, N);
	printf("Infs = %i\nNaNs = %i\n", infs, nans);

	
	probGraph.Normalise();
	probGraph.Draw(-10101.01, -10101.01);

	return 1;

}







