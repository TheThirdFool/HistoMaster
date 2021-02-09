

// This is a Matrix class that is needed for the fitting algorithms
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>


#ifndef HMATRIX_H
#define HMATRIX_H



class HMatrix{


	public:

	//I assume:
	int columns;
	int rows;
	double **Mat;

	int FillMatrix(int i, int j, double dat){
		Mat[i][j] = dat;
		return 1;
	}


	int AddColumn(double * column){

		// Add a column 
		// / 0 1 2 3 \   / 2 \    / 0 1 2 3 2 \
		// | 3 2 1 0 | + | 2 | -> | 3 2 1 0 2 |
		// \ 2 8 9 2 /   \ 2 /    \ 2 8 9 2 2 /

		return 1;
	}



	int Transpose(){

		// Transpose matrix
		// i->j, j->i
		// (1,1) -> (1,1)
		// (1,2) -> (2,1)
		// etc

		// / 0 1 2 3 \     / 0 3 2 \
		// | 3 2 1 0 |  -> | 1 2 8 |
		// \ 2 8 9 2 /     | 2 1 9 |
		//                 \ 3 0 2 /

		return 1;

	}

	private:


};

#endif


