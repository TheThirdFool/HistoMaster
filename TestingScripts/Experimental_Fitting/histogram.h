
/*
histogram.h contains the class definition for the Gaussian Histogram Fitter
See histogram_class.cpp for function declarations
1) histogram(double* data, int size);
	Pass the 1d data array and the number of elements it contains.
	The class will ask for a user input for histogram bin width and output the histgram data and fit to Hist.txt
2) histogram(double* data, int size, double binsize);
	Pass the 1d data array, the number of elements the array contains, and the histogram bin width.
	The class will output  the histgram data and fit to Hist.txt
3) histogram(double* data, int size, char* fname);
	Pass the 1d data array, the number of elements the array contains, and a file name to output the data and fit.
	The class will ask for a user input for histogram bin width and output the fit and data to the passed file name.
4) histogram(double* data, int size, double binsize, char* fname);
	Pass the 1d data array, the number of elements the array contains, the histogram bin width, and a file name to output the data and fit.
	The class will utput the fit and data to the passed file name.
	
	Jonathan Petticrew, University of Sheffield, 2017.
*/
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

class histogram{
	private:
		double* dataset;
		int size;
		double max;
		double min;		
		double binsize;
		double* binEdges;
		double* binCenters;
		double* binValues;
		double mean;
		double meansquare;
		double sdev; //standard deviation
		//Gaussian in the form f(x) = a exp(-((x-b)/c)^2)
		double a;
		double b;
		double c;
		double FWHM;
		char* filename;
		int bins;
		int charstored;
		
		void binner(); //Bins the DataSet and calculates mean and standard deviation
		void fit(); //Fits Gaussian Probability Density Function in the form f(x) = a exp(-((x-b)/c)^2)
		void max_min(); //finds maximum and minimum in dataset
		void print(); //Prints Histogram Data to Hist.txt Used if no string is passed in constructor
		void print_custom(); //Prints Histogram Data to custom file
	public:
		histogram(double* data, int size); //constructor for histogram class Ask for Bin Size
		histogram(double* data, int size, double binsize); //constructor for histogram class passed Bin Size
		histogram(double* data, int size, char* fname); //constructor for histogram class Ask for Bin Size and passes string for printing Hist to file.
		histogram(double* data, int size, double binsize, char* fname); //constructor for histogram class passed Bin Size and passes string for printing Hist to file.
		~histogram();
		double Get_FWHM(){return FWHM;}; //Returns Full-Width @ Half-Maximum
		double Get_Mean(){return mean;}; //Returns Mean of Histogram
		double Get_SDev(){return sdev;}; //Returns Standard Deviation of Histogram
		void show_fit();// displays Probability Density function of gaussian
};
#endif
